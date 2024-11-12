#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <time.h>

time_t start_time; //시작된 시간 저장

void handle_sigint (int sig){ //ctrl + c 시그널 발생하면 호출
    time_t current_time;
    time(&current_time);
    int elapsed = (int)difftime(current_time, start_time);
    printf("\nCurrently elapsed time: %d sec(s)\n", elapsed);
}

int main(){
    time(&start_time);
    signal(SIGINT, handle_sigint);
    printf("you can't stop me!\n");

    while(1){
        printf("haha\n");
        sleep(1);
    }
    return 0;
}