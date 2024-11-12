#include <stdio.h>
#include <curses.h>
#include <unistd.h>

int main()
{
    int i;
    initscr();
    clear();
    for(i=0;i<LINES;i++){
        move(i,i+1);
        if (i % 2 == 1) //홀수일때에만 강조해서 출력하고 char 출력 후 해제 해준다.
            standout(); //강조된 텍스트
        addstr("Hello, world");
        if(i % 2 == 1)
            standend(); //강조모드 해제
    }

    refresh();
    sleep(3);
    endwin();

    return 0;
}