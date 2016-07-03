
#include "okssh.h"

using namespace okssh;

int main() {
    initKeyboard();
    int32_t keycode;

    Window window;
    window.load_config("~/.okssh/config.xml");
    window.render();
    do {
        keycode = getKeyDown();
        switch (keycode) {
            case KEY_DOWN:
                window.SelectNextItem();
                break;
            case KEY_UP:
                window.SelectPreviousItem();
                break;
            case KEY_ENTER:
                cout << window.GetSelectedItemPtr()->GetShellCommand().data() << endl;
                system(window.GetSelectedItemPtr()->GetShellCommand().data());
                return 0;
        }
    } while (keycode != 'q');

    restoreKeyboard();


    return 0;
}
