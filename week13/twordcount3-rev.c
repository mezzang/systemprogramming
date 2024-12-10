#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <ctype.h>

struct arg_set {
    char *fname; /* 처리할 파일 이름 */
    int count;   /* 해당 파일의 단어 수 */
};

int main(int ac, char *av[]) {
    pthread_t threads[ac-1];
    int total_words = 0;
    struct arg_set args[ac - 1];

    void *count_words(void *); /* 단어를 세는 함수 */

    if (ac <2 ) { /* 인자 개수 확인 */
        printf("usage: %s file1 file2 ... filen\n", av[0]); /* 사용법 출력 */
        exit(1); /* 프로그램 종료 */
    }
    

    for (int i = 1; i < ac; i++) {
        args[i-1].fname = av[i];
        args[i-1].count = 0;
        pthread_create(&threads[i - 1], NULL, count_words, (void *)&args[i - 1]);
    }

    for (int i = 1; i < ac; i++) {
            pthread_join(threads[i-1], NULL); 

    }

    for (int i = 1; i < ac; i++) {
        printf("%5d: %s\n", args[i-1].count, args[i-1].fname); /* 첫 번째 파일의 단어 수 출력 */
        total_words += args[i-1].count;
    }
    printf("%5d: total words\n", total_words); /* 총 단어 수 출력 */

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

// tw2-rev는 스레드 수가 많아질수록 모든 스레드가 전역 변수 total_words에 접근하려고 경쟁하기 때문에 뮤텍스 함수 요청이
// 빈번해져 오버헤드가 발생할 수 있습니다.
// tw3-rev는 구조체 배열을 사용하여 각 스레드별로 데이터를 독립적으로 관리하기 때문에 동기화가 불필요하며
// 스레드 수가 증가함에 따른 오버헤드도 없습니다.
// tw3-rev 가 확장성 및 데이터 관리 면에서 효율적입니다.
