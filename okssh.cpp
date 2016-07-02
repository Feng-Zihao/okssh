//
// Created by fengzh on 7/2/2016 AD.
//

#include "okssh.h"

static struct termios old_tio, new_tio;

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

Window::Window(int32_t _itemSize) : itemSize(_itemSize) { }


void Window::RenderNormalItem(std::string str) {
    printf("\e[0m%.80s\n", str.c_str());
}

void Window::RenderSelectedItem(std::string str) {
    printf("\e[7m%.80s\n", str.c_str());
}

void Window::ResetCursor() {
    // Move the cursor up [currentItemIdx] lines and return to the head of line
    printf("\033[%dA", itemSize);
}


void Window::SelectPreviousItem() {
    if (currentItemIdx > 0) {
        currentItemIdx--;
    }
    ResetCursor();
    render();
}

void Window::SelectNextItem() {
    currentItemIdx++;
    if (currentItemIdx == itemSize) {
        currentItemIdx = itemSize - 1;
    }
    ResetCursor();
    render();
}

void Window::render() {
    for (int i = 0; i < itemSize; ++i) {
        if (i == currentItemIdx) {
            RenderSelectedItem(std::to_string(i) + " ===");
        } else {
            RenderNormalItem(std::to_string(i) + " ===");
        }
    }
    printf("\e[0m");
    fflush(stdout);
}

void Window::loadConfig(std::string path) {
    if (!path.empty() && path[0] == '~') {
        path = getenv("HOME") + path.substr(1);
    }
    std::ifstream inf(path);
    std::stringstream buffer;
    if (inf) {
        buffer << inf.rdbuf();
        std::cout << buffer.str();
    }
}