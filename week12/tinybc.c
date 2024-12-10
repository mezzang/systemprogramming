#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

#define oops(m, x)    { perror(m); exit(x); }

void be_dc(int *, int *); // 계산기 프로세스 함수
void be_bc(int *, int *); // 사용자 인터페이스 프로세스 함수
void fatal(char *);       // 오류 처리 함수

int main() {
    int pid, todc[2], fromdc[2]; /* 장비 준비 */

    /* 두 개의 파이프 생성 */
    if (pipe(todc) == -1 || pipe(fromdc) == -1)
        oops("pipe failed", 1);

    /* 사용자 인터페이스를 위한 프로세스 생성 */
    if ((pid = fork()) == -1)
        oops("cannot fork", 2);
    if (pid == 0){
        
        be_dc(todc, fromdc);  /* 자식 프로세스는 계산기 */
    }
    else {
        be_bc(todc, fromdc);  /* 부모 프로세스는 사용자 인터페이스 */
        wait(NULL);           /* 자식 프로세스를 기다림 */
    }

    return 0;
}

void fatal(char *mess) {
    fprintf(stderr, "Error: %s\n", mess); // 오류 메시지 출력
    exit(1);                             // 프로그램 종료
}

void be_bc(int todc[2], int fromdc[2]) {
/*
    표준 입력에서 읽어서 RPN으로 변환한 후, 파이프로 전송
    다른 파이프에서 결과를 읽어 사용자에게 출력
    파일 디스크립터를 스트림으로 변환하기 위해 fdopen() 사용
*/

    int num1, num2;
    char operation[BUFSIZ], message[BUFSIZ], *fgets();
    FILE *fpout, *fpin, *fdopen();

    /* 설정 작업 */
    close(todc[0]);  /* 계산기로부터 읽지 않음 */
    close(fromdc[1]); /* 계산기로 쓰지 않음 */

    fpout = fdopen(todc[1], "w");  /* 파일 디스크립터를 출력 스트림으로 변환 */
    fpin = fdopen(fromdc[0], "r"); /* 파일 디스크립터를 입력 스트림으로 변환 */
    if (fpout == NULL || fpin == NULL)
        fatal("Error converting pipes to streams");

    /* 메인 루프 */
    while (printf("tinybc: "), fgets(message, BUFSIZ, stdin) != NULL) {
        /* 입력 파싱 */
        if (sscanf(message, "%d%[^0-9]%d", &num1, operation, &num2) != 3) {
            printf("syntax error\n");
            continue;
        }

        if (fprintf(fpout, "%d %d %c p\n", num1, num2, *operation) == EOF)
            fatal("Error writing");
        fflush(fpout);

        if (fgets(message, BUFSIZ, fpin) == NULL){
            perror("Error reading from pipe");
            break;
        }
        printf("%d %c %d = %s", num1, *operation, num2, message);
    }

    fclose(fpout); /* 출력 스트림 닫기 */
    fclose(fpin);  /* 입력 스트림 닫기 */
}

void be_dc(int in[2], int out[2]) {
/*
    표준 입력과 표준 출력을 설정한 후, dc 실행
*/

    /* pipein에서 표준 입력 설정 */
    if (dup2(in[0], 0) == -1) {
    perror("dup2 stdin");
    exit(1);
    }
    close(in[0]);  /* fd 0으로 이동했으므로 닫기 */
    close(in[1]);  /* 쓰지 않을 파이프 닫기 */

    /* pipeout에서 표준 출력 설정 */
    if (dup2(out[1], 1) == -1) {
    perror("dup2 stdout");
    exit(1);
    }
    close(out[1]); /* fd 1으로 이동했으므로 닫기 */
    close(out[0]); /* 읽지 않을 파이프 닫기 */

    printf("Executing dc with execlp...\n");

    /* dc 실행 (- 옵션 사용) */
    execlp("dc", "dc", "-", NULL);
    oops("Cannot run dc", 5);
}

