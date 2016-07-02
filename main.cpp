#include <cstdio>
#include <unistd.h>
#include <termios.h>
#include <functional>
#include <iostream>
#include <cstdint>

#include "okssh.h"


class Window {
private:
    int32_t const rowSize;
    int32_t currentRow;

    void renderSingleRow(int32_t n) {

    }

    void clearSingleRow(int32_t n) {

    }

    void moveCursorUp(int32_t n) {

    }

    void moveCursorToBegining() {

    }


public:
    Window(int32_t row) : rowSize(row) { }

    void render() {
        moveCursorUp(0);
    }

    void getTerminalWidth() {

    }

};

std::function<void(void)> voidFunc;



int32_t main() {

    initKeyboard();
    int32_t keycode;

    do {
        keycode = getKeyDown();
        switch (keycode) {
            case KEY_DOWN:
                printf("DOWN");
                break;
            case KEY_UP:
                printf("UP");
                break;
            case KEY_LEFT:
                printf("LEFT");
                break;
            case KEY_RIGNT:
                printf("RIGHT");
                break;

        }
        printf("%d\n", keycode);
    } while (keycode != 'q');

    restoreKeyboard();


    return 0;
}
