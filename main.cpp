#include <stdio.h>
#include <unistd.h>

int main() {


    //write some input
    printf("hello\nworld\n");
    fflush(stdout);

    //wait one second to change line above
    sleep(1);

    // move up 1 line
    printf("\033[1A222");

    //remove line
//    printf("\b");
    fflush(stdout);

    sleep(1);
//    fputs("\033[A\033[2K", stdout);
//    rewind(stdout);

    //write new line
    fputs("bye\n", stdout);

    return 0;
}