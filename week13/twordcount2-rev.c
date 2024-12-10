#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <ctype.h>

int total_words; /* 단어 수를 저장하는 변수와 해당 변수에 대한 락 */
pthread_mutex_t counter_lock = PTHREAD_MUTEX_INITIALIZER; /* Mutex 초기화 */

int main(int ac, char *av[]) {
    
    void *count_words(void *); /* 단어를 세는 함수 */

    if (ac < 2) { /* 최소 한 개의 파일이 필요 */
        printf("Usage: %s file1 [file2 ... fileN]\n", av[0]);
        exit(1);
    }
    pthread_t threads[ac-1];
    total_words = 0; /* 총 단어 수 초기화 */

    for (int i = 0; i < ac-1; i++) {
        pthread_create(&threads[i], NULL, count_words, (void *)av[i]);
    }

    for (int i = 0; i < ac-1; i++) {
        pthread_join(threads[i], NULL);
    }

    
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
