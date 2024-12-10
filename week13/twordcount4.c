/* twordcount4.c - 두 파일에 대해 단어를 세는 스레드 기반 단어 카운터 */
/* Version 4: 조건 변수를 사용하여 카운터 함수가 결과를 즉시 보고 가능 */

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <ctype.h>

struct arg_set {
    char *fname; /* 처리할 파일 이름 */
    int count;   /* 해당 파일의 단어 수 */
};

struct arg_set *mailbox = NULL; /* 데이터 전달을 위한 공유 공간 */
pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER; /* 뮤텍스 초기화 */
pthread_cond_t flag = PTHREAD_COND_INITIALIZER; /* 조건 변수 초기화 */

int main(int ac, char *av[]) {
    pthread_t t1, t2; /* 두 개의 스레드 */
    struct arg_set args1, args2; /* 두 파일의 데이터 */
    void *count_words(void *); /* 단어를 세는 함수 */
    int reports_in = 0; /* 결과 보고 카운터 */
    int total_words = 0; /* 총 단어 수 */

    if (ac != 3) { /* 인자 개수 확인 */
        printf("usage: %s file1 file2\n", av[0]);
        exit(1);
    }

    args1.fname = av[1];
    args1.count = 0;
    pthread_create(&t1, NULL, count_words, (void *)&args1); /* 첫 번째 스레드 생성 */

    args2.fname = av[2];
    args2.count = 0;
    pthread_create(&t2, NULL, count_words, (void *)&args2); /* 두 번째 스레드 생성 */

    while (reports_in < 2) {
        printf("MAIN: waiting for flag to go up\n");
        pthread_mutex_lock(&lock); /* 락 획득 */
        pthread_cond_wait(&flag, &lock); /* 조건 변수가 신호를 받을 때까지 대기 */
        printf("MAIN: Wow! flag was raised, I have the lock\n");

        printf("%5d: %s\n", mailbox->count, mailbox->fname); /* 결과 출력 */
        total_words += mailbox->count; /* 총 단어 수 갱신 */

        if (mailbox == &args1) /* 현재 결과가 args1이라면 */
            pthread_join(t1, NULL); /* 첫 번째 스레드 종료 대기 */
        else if (mailbox == &args2) /* 현재 결과가 args2라면 */
            pthread_join(t2, NULL); /* 두 번째 스레드 종료 대기 */

        mailbox = NULL; /* 공유 공간 초기화 */
        pthread_cond_signal(&flag); /* 상태 변경 알림 */
        pthread_mutex_unlock(&lock); /* 락 해제 */
        reports_in++;
    }

    printf("%5d: total words\n", total_words); /* 총 단어 수 출력 */

    return 0;
}

void *count_words(void *a) {
    struct arg_set *args = a; /* 전달된 인자를 올바른 타입으로 캐스팅 */
    FILE *fp;
    int c, prevc = '\0';

    if ((fp = fopen(args->fname, "r")) != NULL) {
        while ((c = getc(fp)) != EOF) {
            if (isalnum(c) && !isalnum(prevc)) /* 단어의 끝을 발견하면 */
                args->count++;
            prevc = c;
        }
        fclose(fp);
    } else {
        perror(args->fname); /* 파일 열기 오류 */
    }

    printf("COUNT: waiting to get lock\n");
    pthread_mutex_lock(&lock); /* 락 획득 */
    printf("COUNT: have lock, storing data\n");

    while (mailbox != NULL) { /* 공유 공간이 비어있지 않으면 대기 */
        printf("COUNT: oops...mailbox not empty, wait for signal\n");
        pthread_cond_wait(&flag, &lock);
    }

    mailbox = args; /* 결과 저장 */
    printf("COUNT: raising flag\n");
    pthread_cond_signal(&flag); /* 상태 변경 알림 */
    printf("COUNT: unlocking box\n");
    pthread_mutex_unlock(&lock); /* 락 해제 */

    return NULL;
}
