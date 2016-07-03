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
#include <memory>
#include "pugiconfig.hpp"
#include "pugixml.hpp"


#define KEY_UP (27 << 16 | 91 << 8 | 65)
#define KEY_DOWN (27 << 16 | 91 << 8 | 66)
#define KEY_RIGNT (27 << 16 | 91 << 8 | 67)
#define KEY_LEFT (27 << 16 | 91 << 8 | 68)
#define KEY_ENTER (10)

using namespace std;
namespace okssh {

void initKeyboard();

void restoreKeyboard();

int32_t getKeyDown();

class Item {
public:
    virtual const string &GetShellCommand() = 0;

    virtual const string &getDescription() = 0;
};

class Window;

class Host : public Item {
    friend class Window;
private:
    string description;
    string target;
    string keyfile;
    string cmd;
    string port;

public:
    const string &GetShellCommand();

    const string &getDescription();
};


class Window {
private:
    int32_t curr_item_idx = 0;
    vector<shared_ptr<Item>> item_refs;

    void RenderItem(bool selected, string str);

    void ResetCursor();

public:
    void SelectPreviousItem();

    void SelectNextItem();

    shared_ptr<Item> GetSelectedItemPtr();

    void render();

    void load_config(string path);
};

}
#endif //OKSSH_OKSSH_H
