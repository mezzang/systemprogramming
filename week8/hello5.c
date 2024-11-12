#include <unistd.h>
#include <curses.h>

#define LEFTEDGE 10
#define RIGHTEDGE 30
#define ROW 10

int main()
{
    char message[] = "Hello";
    char blank[] = "     ";
    int dir = +1;
    int pos = LEFTEDGE;

    initscr();
    clear();
    while(1){
        move(ROW, pos); //(10,10)에서 시작
        addstr(message); //draw string
        move(LINES-1, COLS-1);
        refresh();
        sleep(1); //1초 간격으로 이동
        move(ROW, pos);
        addstr(blank); //지우기
        pos += dir; //오른쪽으로 이동
        if(pos >= RIGHTEDGE)
            dir = -1; //오른쪽 끝에 다다르면 왼쪽으로 이동
        if(pos <= LEFTEDGE)
            dir = +1; //왼쪼 끝에 다다르면 다시 오른쪽으로 이동
    }
    return 0;

}