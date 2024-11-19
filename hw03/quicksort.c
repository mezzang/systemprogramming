#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <curses.h>
#include <signal.h>

#define BUFSIZE 20
#define DELAY 100000  // 0.1초 지연 시간

volatile sig_atomic_t paused = 0;

void initialize_array(int array[], int size);
void display_array(int array[], int size);
void bubble_sort(int array[], int size);
void handle_stop(int sig);
void handle_end(int sig);


int main() {
    //SIGTSTP 신호 처리
    signal(SIGQUIT, handle_stop);
    signal(SIGINT, handle_end);
    

    int array[BUFSIZE];

    // curses 초기화
    initscr();          // curses 모드 시작
    noecho();           // 입력 문자를 화면에 표시하지 않음
    curs_set(FALSE);    // 커서를 보이지 않게 설정

    initialize_array(array, BUFSIZE);

    // 버블 정렬 시작
    bubble_sort(array, BUFSIZE);

    // 종료 메시지
    refresh();               // 화면에 출력 반영
    getch();                 // 종료 전 키 입력 대기

    

    return 0;
}

// 배열을 임의의 값으로 초기화
void initialize_array(int array[], int size) {
    for (int i = 0; i < size; i++) {
        array[i] = rand() % size + 1;  // 1부터 size까지의 랜덤 값
    }
}

// 배열의 현재 상태를 curses로 시각화하여 출력
void display_array(int array[], int size) {
    clear();                // 화면 지우기
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < array[i]; j++) {
            move(size - j, i * 2);    // 커서를 각 요소 위치로 이동
            printw("|");              // '|'를 출력하여 배열 요소의 크기를 시각화
        }
    }
    refresh();               // 변경 사항을 화면에 반영
    usleep(DELAY);           // 지연 시간 설정
}

// 버블 정렬 알고리즘을 수행하며 시각화
void bubble_sort(int array[], int size) {
    for (int i = 0; i < size - 1; i++) {
        for (int j = 0; j < size - i - 1; j++) {

            while(paused){
                pause();
            }
            if (array[j] > array[j + 1]) {
                // 요소 교환
                int temp = array[j];
                array[j] = array[j + 1];
                array[j + 1] = temp;

                // 교환 후 배열의 상태 시각화
                display_array(array, size);
            }
        }
    }
}

void handle_stop(int sig)
{   
    if(paused){
        paused = 0;
    }
    else{
        paused = 1;
    }

}


void handle_end(int sig)
{   
    // curses 종료
    endwin();
    exit(0);
}