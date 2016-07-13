//
// Created by fengzh on 7/2/2016 AD.
//

#include "okssh.h"
#include <yaml-cpp/yaml.h>

static struct termios old_tio, new_tio;

const string CONFIG_TEMPLATE = R"B246A588(
hosts:
    -
        description: example description
        keyfile: /absolute/path/for/keyfile
        target: user@hostname
    -
        description: example description with mosh and specific port
        keyfile: /absolute/path/for/keyfile
        target: user@hostname
        client: mosh  # if you're using mosh
        port: 2222
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
    const static string DEFAULT_CONFIG_PATH = "~/.okssh/config.yaml";
    bool is_default_config_path = DEFAULT_CONFIG_PATH == path;

    if (!path.empty() && path[0] == '~') {
        path = getenv("HOME") + path.substr(1);
    }

    ifstream inf(path);
    if (!inf && is_default_config_path) {
        cerr << "Can't find config.yaml from " + DEFAULT_CONFIG_PATH << endl;
        cerr << "Generate " << DEFAULT_CONFIG_PATH << " with default template" << endl;
        ofstream ouf(path);
        if (ouf) {
            ouf << CONFIG_TEMPLATE << endl;
            ouf.close();
            exit(EXIT_FAILURE);
        }
    }

    YAML::Node config = YAML::LoadFile(path);


    YAML::Node hosts = config["hosts"];

    for (YAML::iterator host = hosts.begin(); host != hosts.end(); host++) {
        shared_ptr<Host> host_sptr = make_shared<Host>();
        host_sptr->description = "abc";
        host_sptr->description = (*host)["description"].as<string>();
        host_sptr->keyfile = (*host)["keyfile"].as<string>();
        host_sptr->target = (*host)["target"].as<string>();
        if ((*host)["client"]) {
            host_sptr->client = (*host)["client"].as<string>();
        } else {
            host_sptr->client = "ssh";
        }
        if ((*host)["port"]) {
            host_sptr->port = (*host)["port"].as<string>();
        }
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
        if (client.empty()) {
            client = "ssh";
        }
        cmd = client + " -i " + keyfile + " " + target;
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