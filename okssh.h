//
// Created by fengzh on 7/2/2016 AD.
//

#ifndef OKSSH_OKSSH_H
#define OKSSH_OKSSH_H

#include <cstdio>
#include <unistd.h>
#include <termios.h>
#include <functional>
#include <iostream>


#define KEY_UP (27 << 16 | 91 << 8 | 65)
#define KEY_DOWN (27 << 16 | 91 << 8 | 66)
#define KEY_RIGNT (27 << 16 | 91 << 8 | 67)
#define KEY_LEFT (27 << 16 | 91 << 8 | 68)
#define KEY_ENTER (10)


void initKeyboard();

void restoreKeyboard();

int32_t getKeyDown();



class Window {
private:
    int32_t const rowSize;
    int32_t currentRow = 1;

    void RenderNormalRow(std::string str);
    void RenderSelectedRow(std::string str);
    void ResetCursor();

public:
    Window(int32_t row);
    void SelectPreviousRow();
    void SelectNextRow();
    void render();
};


#endif //OKSSH_OKSSH_H
