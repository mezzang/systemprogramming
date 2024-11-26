#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/wait.h>

int main() {
    int pid;
    int fd;

    printf("who 명령어 출력을 userlog에 추가하려고 합니다.\n");

    if ((pid = fork()) == -1) {  // fork 실패 시
        perror("fork");
        exit(1);
    }

    if (pid == 0) {  // 자식 프로세스
        close(1);  // 표준 출력 닫기
        // 파일을 열 때 O_APPEND로 열기
        fd = open("userlog", O_WRONLY | O_CREAT | O_APPEND, 0644);
        if (fd == -1) {
            perror("파일 열기 실패");
            exit(1);
        }
        execlp("who", "who", NULL);  // who 명령 실행
        perror("execlp 실행 실패");
        exit(1);
    }

    if (pid > 0) {  // 부모 프로세스
        wait(NULL);  // 자식 프로세스 종료 대기
        printf("who 명령 실행 완료. 결과는 userlog에 추가되었습니다.\n");
    }

    return 0;
}
