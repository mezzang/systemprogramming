/* pipe.c
 * 프로세스 간 파이프라인을 생성하는 방법을 보여줌
 * - 두 개의 명령을 받아 첫 번째 명령의 출력을 두 번째 명령의 입력으로 연결
 * 사용법: pipe command1 command2
 * 동작: command1 | command2
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/wait.h>

#define oops(m, x) { perror(m); exit(x); }  // 오류 출력 및 종료

int main(int ac, char **av) {
    int thepipe[2];  // 파이프용 두 개의 파일 디스크립터
    int pid;

    if (ac != 3) {   // 인자가 두 개가 아니면 오류 출력
        fprintf(stderr, "사용법: pipe cmd1 cmd2\n");
        exit(1);
    }

    if (pipe(thepipe) == -1) {  // 파이프 생성 실패 시
        oops("파이프를 생성할 수 없습니다.", 1);
    }

    /* 프로세스 생성 */
    if ((pid = fork()) == -1) {
        oops("프로세스를 생성할 수 없습니다.", 2);
    }

    if (pid > 0) {  // 부모 프로세스
        close(thepipe[1]);  // 파이프의 쓰기 끝을 닫음
        if (dup2(thepipe[0], 0) == -1) {  // 파이프 읽기 끝을 표준 입력에 연결
            oops("표준 입력으로 리다이렉션할 수 없습니다.", 3);
        }
        close(thepipe[0]);  // 파이프 읽기 끝 닫기
        execlp(av[2], av[2], NULL);  // 두 번째 명령 실행
        oops(av[2], 4);  // execlp 실행 실패 시
    } else {  // 자식 프로세스
        close(thepipe[0]);  // 파이프의 읽기 끝을 닫음
        if (dup2(thepipe[1], 1) == -1) {  // 파이프 쓰기 끝을 표준 출력에 연결
            oops("표준 출력으로 리다이렉션할 수 없습니다.", 4);
        }
        close(thepipe[1]);  // 파이프 쓰기 끝 닫기
        execlp(av[1], av[1], NULL);  // 첫 번째 명령 실행
        oops(av[1], 5);  // execlp 실행 실패 시
    }

    return 0;
}
