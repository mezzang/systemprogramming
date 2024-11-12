#include <stdio.h>
#include <curses.h>
#include <unistd.h>

int main()
{
    initscr();
    clear();

    
    while(1){
        move(10,20);
        addstr("Hello, world"); // (10,20)에 문자열 출력
        refresh();
        sleep(1);
        move(10,20);
        addstr("            ");
        refresh();
        sleep(1);
    }
    endwin();

    
    return 0;
}