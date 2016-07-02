//
// Created by fengzh on 7/2/2016 AD.
//

#include <cstdio>
#include <unistd.h>
#include <termios.h>
#include <functional>
#include <iostream>

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

Window::Window(int32_t row) : rowSize(row) { }


void Window::RenderNormalRow(std::string str) {
    printf("\e[0m%.80s\n", str.c_str());
}

void Window::RenderSelectedRow(std::string str) {
    printf("\e[7m%.80s\n", str.c_str());
}

void Window::ResetCursor() {
    // Move the cursor up [currentRow] lines and return to the head of line
    printf("\033[%dA", rowSize);
}


void Window::SelectPreviousRow() {
    if (currentRow > 0) {
        currentRow--;
    }
    ResetCursor();
    render();
}

void Window::SelectNextRow() {
    currentRow++;
    if (currentRow == rowSize) {
        currentRow = rowSize - 1;
    }
    ResetCursor();
    render();
}

void Window::render() {
    for (int i = 0; i < rowSize; ++i) {
        if (i == currentRow) {
            RenderSelectedRow(std::to_string(i) + " ===");
        } else {
            RenderNormalRow(std::to_string(i) + " ===");
        }
    }
    printf("\e[0m");
    fflush(stdout);
}

