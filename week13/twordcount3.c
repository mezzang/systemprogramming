/* twordcount3.c - 두 파일에 대해 단어를 세는 스레드 기반 단어 카운터 */
/* Version 3: 파일별로 개별 카운터를 사용 */

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <ctype.h>

struct arg_set {
    char *fname; /* 처리할 파일 이름 */
    int count;   /* 해당 파일의 단어 수 */
};

int main(int ac, char *av[]) {
    pthread_t t1, t2; /* 두 개의 스레드 */
    struct arg_set args1, args2; /* 두 개의 파일 정보 */
    void *count_words(void *); /* 단어를 세는 함수 */

    if (ac != 3) { /* 인자 개수 확인 */
        printf("usage: %s file1 file2\n", av[0]); /* 사용법 출력 */
        exit(1); /* 프로그램 종료 */
    }

    args1.fname = av[1]; /* 첫 번째 파일 정보 초기화 */
    args1.count = 0;
    pthread_create(&t1, NULL, count_words, (void *)&args1); /* 첫 번째 스레드 생성 */

    args2.fname = av[2]; /* 두 번째 파일 정보 초기화 */
    args2.count = 0;
    pthread_create(&t2, NULL, count_words, (void *)&args2); /* 두 번째 스레드 생성 */

    pthread_join(t1, NULL); /* 첫 번째 스레드 종료 대기 */
    pthread_join(t2, NULL); /* 두 번째 스레드 종료 대기 */

    printf("%5d: %s\n", args1.count, args1.fname); /* 첫 번째 파일의 단어 수 출력 */
    printf("%5d: %s\n", args2.count, args2.fname); /* 두 번째 파일의 단어 수 출력 */
    printf("%5d: total words\n", args1.count + args2.count); /* 총 단어 수 출력 */

    return 0;
}

void *count_words(void *a) {
    struct arg_set *args = a; /* 전달된 인자를 올바른 타입으로 캐스팅 */
    FILE *fp; /* 파일 포인터 */
    int c, prevc = '\0'; /* 현재 문자와 이전 문자 */

    if ((fp = fopen(args->fname, "r")) != NULL) { /* 파일 열기 */
        while ((c = getc(fp)) != EOF) { /* 파일 끝까지 읽기 */
            if (isalnum(c) && !isalnum(prevc)) /* 단어의 끝을 발견하면 */
                args->count++; /* 해당 파일의 단어 수 증가 */
            prevc = c; /* 현재 문자를 이전 문자로 저장 */
        }
        fclose(fp); /* 파일 닫기 */
    } else {
        perror(args->fname); /* 파일 열기 오류 출력 */
    }
    return NULL;
}
