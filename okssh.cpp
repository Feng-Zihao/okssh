//
// Created by fengzh on 7/2/2016 AD.
//

#include "okssh.h"

static struct termios old_tio, new_tio;

const string CONFIG_TEMPLATE = R"B246A588(
<config>
    <hosts>
        <host>
            <description>example config in ~/okssh/config.xml</description> <!-- literal text for -->
            <keyfile>/whatever/my/keyfile</keyfile> <!-- absolute path for keyfile -->
            <target>root@111.11.11.11</target> <!-- user@host or just host if login as current user -->
            <port>11111</port>  <!-- 22 by default if not present -->
        </host>
        <!-- append more <host></host> as you need -->
    </hosts>
</config>
)B246A588";

namespace okssh {

void initKeyboard() {
    // get the terminal settings for stdin
    tcgetattr(STDIN_FILENO, &old_tio);
    // copy and set flags
    new_tio = old_tio;
    new_tio.c_lflag &= (~ICANON & ~ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &new_tio);

}

void restoreKeyboard() {
    /* restore the former settings */
    tcsetattr(STDIN_FILENO, TCSANOW, &old_tio);

}

int getKeyDown() {
    int32_t c = getchar();

    // handle arrow key
    if (c == 27) {
        // getchar twice
        c = c << 8 | getchar();
        c = c << 8 | getchar();
    }
    return c;

}

void Window::RenderItem(bool selected, string str) {
    if (selected) {
        printf("\e[7m%.80s\n", str.data());
    } else {
        printf("\e[0m%.80s\n", str.data());
    }
}

void Window::ResetCursor() {
    // Move the cursor up [curr_item_idx] lines and return to the head of line
    printf("\033[%dA", (int) item_refs.size());
}


void Window::SelectPreviousItem() {
    if (curr_item_idx > 0) {
        curr_item_idx--;
    }
    ResetCursor();
    render();
}

void Window::SelectNextItem() {
    curr_item_idx++;
    if (curr_item_idx == item_refs.size()) {
        curr_item_idx = (int32_t) (item_refs.size() - 1);
    }
    ResetCursor();
    render();
}

void Window::SelectNone() {
    curr_item_idx = -1;
    ResetCursor();
    render();
}

shared_ptr<Item> Window::GetSelectedItemPtr() {
    return item_refs[curr_item_idx];
}

void Window::render() {
    for (int i = 0; i < item_refs.size(); ++i) {
        RenderItem(i == curr_item_idx, to_string(i) + " " + item_refs[i]->getDescription());
    }
    printf("\e[0m");
    fflush(stdout);
}

void Window::load_config(string path) {
    const static string DEFAULT_CONFIG_PATH = "~/.okssh/config.xml";
    bool is_default_config_path = DEFAULT_CONFIG_PATH == path;

    if (!path.empty() && path[0] == '~') {
        path = getenv("HOME") + path.substr(1);
    }

    ifstream inf(path);
    if (!inf && is_default_config_path) {
        cerr << "Can't find config.xml from " + DEFAULT_CONFIG_PATH << endl;
        cerr << "Generate " << DEFAULT_CONFIG_PATH << " with default template" << endl;
        ofstream ouf(path);
        if (ouf) {
            ouf << CONFIG_TEMPLATE << endl;
            ouf.close();
            exit(EXIT_FAILURE);
        }
    }


    pugi::xml_document doc;

    pugi::xml_parse_result result = doc.load_file(path.data());
    if (!result) {
        cerr << "Cant' load config " << path << endl;
        exit(EXIT_FAILURE);
    }
    pugi::xml_node hosts_node = doc.child("config").child("hosts");

    for (pugi::xml_node host = hosts_node.child("host"); host; host = host.next_sibling()) {
        shared_ptr<Host> host_sptr = make_shared<Host>();
        host_sptr->description = host.child_value("description");
        host_sptr->keyfile = host.child_value("keyfile");
        host_sptr->target = host.child_value("target");
        host_sptr->port = host.child_value("port");
        item_refs.push_back(host_sptr);
    }

    if (item_refs.empty()) {
        cerr << "Empty hosts. Exit." << endl;
        exit(EXIT_FAILURE);
    }

    curr_item_idx = 0;

}

const string &Host::GetShellCommand() {
    if (cmd.empty()) {
        cmd.resize(1024);
        cmd = "ssh -i " + keyfile + " " + target;
    }
    if (!port.empty()) {
        cmd += " -p " + port;
    }
    return cmd;
}

const string &Host::getDescription() {
    return description;
}


}