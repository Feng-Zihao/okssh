#include <cstdio>
#include <unistd.h>
#include <termios.h>
#include <functional>
#include <iostream>
#include <cstdint>
#include <string>

#include "json.hpp"
#include "okssh.h"


int main() {
    initKeyboard();
    int32_t keycode;

    Window window(3);

    window.render();

    do {
        keycode = getKeyDown();
        switch (keycode) {
            case KEY_DOWN:
                window.SelectNextRow();
                break;
            case KEY_UP:
                window.SelectPreviousRow();
                break;
        }
    } while (keycode != 'q');

    restoreKeyboard();


    return 0;
}
