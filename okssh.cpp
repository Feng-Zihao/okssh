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