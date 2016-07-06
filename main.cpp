
#include <csignal>
#include "okssh.h"
#include <yaml-cpp/yaml.h>

using namespace okssh;

static Window window;

void cleanup(int sig) {
    window.SelectNone();
    restoreKeyboard();
    exit(EXIT_FAILURE);
}

int main() {
    initKeyboard();
    int32_t keycode;


    window.load_config("~/.okssh/config.yaml");
    window.render();

    signal (SIGINT, cleanup);


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

    window.SelectNone();
    restoreKeyboard();


    return 0;
}
