#include <stdio.h>
#include <sys/time.h>
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>

int set_ticker(int);
void countdown(int);

int main()
{
     void countdown(int); //함수 선언 가독성과 관련된 선택

     signal(SIGALRM, countdown); //SIGALRM 발생하면 countdown 함수를 호출하게 함
     if(set_ticker(500) == -1) //set_ticker 함수를 호출하여 500밀리초 간격으로 SIGALRM 발생하도록 설정
        perror("set_ticker");
    else    
        while(1)
            pause(); //신호가 발생할 때까지 대기 SIGALRM 발생하면 countdown()함수 호출
    return 0;
}
/* 실행흐름
set_ticker(500)이 호출되면서, 500밀리초마다 SIGALRM 신호가 발생한다.
SIGALRM 신호가 발생하면 countdown 함수가 호출되어 숫자를 출력하고 감소시킨다.
숫자가 0이되면 done을 출력하고 프로그램 종료시킨다. */

void countdown(int signum)
{
    static int num = 10;
    printf("%d..", num--);
    fflush(stdout);
    if(num < 0){
        printf("DONE!\n");
        exit(0);
    }
}

int set_ticker(int n_msecs) //주기적으로 신호 발생시키는 함수
{
    struct itimerval new_timeset;
    long n_sec, n_usecs;
    
    n_sec = n_msecs/1000;
    n_usecs = (n_msecs % 1000) * 1000L;

    new_timeset.it_interval.tv_sec = n_sec; //interval
    new_timeset.it_interval.tv_usec = n_usecs;
    new_timeset.it_value.tv_sec = n_sec; //value
    new_timeset.it_value.tv_usec = n_usecs;
    //처음 500ms 후 시작하고 그 후 매 500ms 마다 반복하도록 만든다.

    return setitimer(ITIMER_REAL, &new_timeset, NULL); //setitimer 통해서 주기적으로 SIGALRM 신호 발생시킴
    //ITIMER_REAL 사용해 실시간 타이머를 설정, 타이머가 완료될때마다 SIGALRM 발생
}