#include <stdio.h>
#include <unistd.h>
#include <signal.h>

// #define SHHHH

int main()
{
    void wakeup(int);

    printf("about to sleep for 4seconds\n"); //1
    signal(SIGALRM, wakeup); //우분투에서 동작
    alarm(4); //4로 알람 설정 4초후 신호가 온다.
    pause(); //freeze
    printf("Morning so soon?\n"); //3 2와 3은 거의 동시에 알람을 커널로 받으면 바로 수행된다.

    return 0;
}
void wakeup(int signum)
{
    #ifndef SHHHH
        printf("Alarm received from kernel\n"); //2 4초 후 커널이 알람을 보낸다
    #endif
}