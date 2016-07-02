//
// Created by fengzh on 7/2/2016 AD.
//

#include "okssh.h"

using json = nlohmann::json;

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
    ifstream inf(path);
    stringstream buffer;
    if (inf) {
        buffer << inf.rdbuf();
        inf.close();
    }
    string text = buffer.str();
    if (text.empty()) {
        fprintf(stderr, "can't load config file from %s", path.data());
        exit(-1);
    }
    json json_config = json::parse(text);
    json json_hosts = json_config["hosts"];
    for_each(json_hosts.begin(), json_hosts.end(), [this](json &host_config) {
        shared_ptr<Host> host_sptr = make_shared<Host>();
        host_sptr->description = host_config["description"];
        host_sptr->user = host_config["user"];
        host_sptr->key = host_config["key"];
        host_sptr->hostname = host_config["hostname"];
        item_refs.push_back(host_sptr);
    });
}

}