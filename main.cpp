#include <cstdio>
#include <unistd.h>
#include <termios.h>

void initKeyboard();

void restoreKeyboard();

static struct termios old_tio, new_tio;


class Window {
private:
    int const rowSize;
    int currentRow;

    void renderSingleRow(int n) {

    }

    void clearSingleRow(int n) {

    }

    void moveCursorUp(int n) {

    }

    void moveCursorToBegining() {

    }


public:
    Window(int row) : rowSize(row) { }

    void render() {
        moveCursorUp(0);
    }

    void getTerminalWidth() {

    }

};


int main() {
    initKeyboard();
    int c;

    do {
        c = getchar();
        printf("%d\n", c);

        // handle arrow key
        if (c == 27) {
            getchar();

            c = getchar();
            switch (c) {
                case 65:
                    printf("key up\n");
                    break;
                case 66:
                    printf("key down\n");
                    break;
                case 68:
                    printf("key left\n");
                    break;
                case 67:
                    printf("key right\n");
                    break;
                default:
                    // not handled
                    break;
            }

        }
    } while (c != 'q');

    restoreKeyboard();


    return 0;
}

void initKeyboard() {
    /* get the terminal settings for stdin */
    tcgetattr(STDIN_FILENO, &old_tio);
    /* we want to keep the old setting to restore them a the end */
    new_tio = old_tio;
    /* disable canonical mode (buffered i/o) and local echo */
    new_tio.c_lflag &= (~ICANON & ~ECHO);
    /* set the new settings immediately */
    tcsetattr(STDIN_FILENO, TCSANOW, &new_tio);

}

void restoreKeyboard() {
    /* restore the former settings */
    tcsetattr(STDIN_FILENO, TCSANOW, &old_tio);

}