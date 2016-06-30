#include <ncurses.h>
#include <iostream>

int main() {
    initscr();            /* Start curses mode 		  */

    while (true) {
//        printw("Hello World !!!");    /* Print Hello World		  */
        refresh();            /* Print it on to the real screen */
        std::cout << getch() << std::endl;            /* Wait for user input */
    }
    endwin();            /* End curses mode		  */

    return 0;
}
