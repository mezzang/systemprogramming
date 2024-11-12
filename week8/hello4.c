#include <stdio.h>
#include <unistd.h>
#include <curses.h>

int main()
{
    int i;
    initscr();
    clear();
    for(i = 0; i<LINES; i++){
        move(i, i+1);
        if ( i % 2 == 1)
            standout();
        addstr("Hello, world"); //출력되고
        if(i % 2 == 1)
            standend();
        refresh();
        sleep(1);
        move(i, i+1);
        addstr("            "); //지워진다. 같은 위치에 빈 스트링을 출력하였기 때문.
    }
    endwin();

    return 0;
}