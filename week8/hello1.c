#include <stdio.h>
#include <curses.h>

int main()
{
    initscr();
    clear();
    move(10,20);
    addstr("Hello, world\n"); // (10,20)에 문자열 출력
    move(LINES-1,0); //화면 마지막 줄의 첫 번째 열 커서가 이동!!

    refresh();
    getch();

    endwin();

    return 0;
}