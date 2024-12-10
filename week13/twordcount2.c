/* twordcount2.c - 두 파일에 대해 단어를 세는 스레드 기반 단어 카운터 */
/* version 2: Mutex를 사용하여 total_words에 Lock을 적용 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <ctype.h>

int total_words; /* 단어 수를 저장하는 변수와 해당 변수에 대한 락 */
pthread_mutex_t counter_lock = PTHREAD_MUTEX_INITIALIZER; /* Mutex 초기화 */

int main(int ac, char *av[]) {
    pthread_t t1, t2; /* 두 개의 스레드 */
    void *count_words(void *); /* 단어를 세는 함수 */

    if (ac != 3) { /* 인자 개수 확인 */
        printf("usage: %s file1 file2\n", av[0]); /* 사용법 출력 */
        exit(1); /* 프로그램 종료 */
    }
    total_words = 0; /* 총 단어 수 초기화 */
    pthread_create(&t1, NULL, count_words, (void *)av[1]); /* 첫 번째 스레드 생성 */
    pthread_create(&t2, NULL, count_words, (void *)av[2]); /* 두 번째 스레드 생성 */
    pthread_join(t1, NULL); /* 첫 번째 스레드 종료 대기 */
    pthread_join(t2, NULL); /* 두 번째 스레드 종료 대기 */
    printf("%5d: total words\n", total_words); /* 총 단어 수 출력 */

    return 0;
}

void *count_words(void *f) {
    char *filename = (char *)f; /* 파일 이름 */
    FILE *fp; /* 파일 포인터 */
    int c, prevc = '\0'; /* 현재 문자와 이전 문자 */

    if ((fp = fopen(filename, "r")) != NULL) { /* 파일 열기 */
        while ((c = getc(fp)) != EOF) { /* 파일 끝까지 읽기 */
            if (!isalnum(c) && isalnum(prevc)) { /* 이전 문자가 단어의 끝이라면 */
                pthread_mutex_lock(&counter_lock); /* total_words를 보호하기 위한 Mutex Lock */
                total_words++; /* 단어 수 증가 */
                pthread_mutex_unlock(&counter_lock); /* Mutex Unlock */
            }
            prevc = c; /* 현재 문자를 이전 문자로 저장 */
        }
        fclose(fp); /* 파일 닫기 */
    } else {
        perror(filename); /* 파일 열기 오류 출력 */
    }
    return NULL;
}
