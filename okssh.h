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
#include <vector>
#include <fstream>
#include <sstream>


#define KEY_UP (27 << 16 | 91 << 8 | 65)
#define KEY_DOWN (27 << 16 | 91 << 8 | 66)
#define KEY_RIGNT (27 << 16 | 91 << 8 | 67)
#define KEY_LEFT (27 << 16 | 91 << 8 | 68)
#define KEY_ENTER (10)


void initKeyboard();

void restoreKeyboard();

int32_t getKeyDown();

class Item {
private:
    std::string description;
    std::string hostname;
    std::string user;
    std::string key;
    std::string cmd;
public:

    const std::string& GetShellCommand() {
        if (cmd.empty()) {
            cmd.resize(1024);
            cmd = "ssh -i " + key + " " + user + "@" + hostname;
        }
        return cmd;
    }

    const std::string& getDescription() {
        return description;
    }
};


class Window {
private:
    int32_t const itemSize;
    int32_t currentItemIdx = 0;

    void RenderNormalItem(std::string str);
    void RenderSelectedItem(std::string str);
    void ResetCursor();

public:
    Window(int32_t _itemSize);
    void SelectPreviousItem();
    void SelectNextItem();
    void render();
    void loadConfig(std::string path);
};


#endif //OKSSH_OKSSH_H
