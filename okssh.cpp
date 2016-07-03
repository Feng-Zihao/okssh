//
// Created by fengzh on 7/2/2016 AD.
//

#include "okssh.h"

static struct termios old_tio, new_tio;

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
    if (!path.empty() && path[0] == '~') {
        path = getenv("HOME") + path.substr(1);
    }

    pugi::xml_document doc;

    pugi::xml_parse_result result = doc.load_file(path.data());
    if (!result) {
        cerr << "Cant' load config " << path;
        exit(-1);
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