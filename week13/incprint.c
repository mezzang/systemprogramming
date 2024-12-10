#include <stdio.h>
#include <unistd.h>
#include <pthread.h>

#define NUM 5 // 반복 횟수 정의

/* incprint.c - 한 스레드는 값을 증가시키고, 다른 스레드는 출력합니다. */

void *print_count(void *m);

int counter = 0; // 전역 변수 counter

int main()
{
    pthread_t t1; /* 하나의 스레드 생성 */
    void *print_count(void *); /* 스레드의 함수 선언 */
    int i;

    pthread_create(&t1, NULL, print_count, NULL); // 스레드 생성 및 print_count 함수 호출
    for(i = 0; i < NUM; i++) {
        counter++; // counter 값 증가
        sleep(1); // 1초 대기
    }
    pthread_join(t1, NULL); // 스레드가 종료될 때까지 대기

    return 0;
}

void *print_count(void *m)
{
    int i;
    for(i = 0; i < NUM; i++) {
        printf("count = %d\n", counter); // counter 값 출력
        sleep(1); // 1초 대기
    }
    return NULL;
}
