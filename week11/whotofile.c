/* whotofile.c
 * 프로그램의 출력을 파일로 리다이렉트하는 방법
 * 동작 아이디어: fork() -> 자식 프로세스에서 출력 리다이렉트 -> exec 실행
 */

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/wait.h>

int main() {
    int pid;
    int fd;

    printf("who 명령어를 파일로 리다이렉트하려고 합니다.\n");

    /* 새로운 프로세스를 생성하거나 실패 시 종료 */
    if ((pid = fork()) == -1) {
        perror("fork"); 
        exit(1);
    }

    /* 자식 프로세스에서 작업 수행 */
    if (pid == 0) {  
        close(1);  // 표준 출력 닫기
        fd = creat("userlist", 0644);  // "userlist"라는 파일 생성
        if (fd == -1) { 
            perror("파일 생성 실패");
            exit(1);
        }
        execlp("who", "who", NULL);  // who 명령 실행
        perror("execlp 실행 실패");
        exit(1);
    }

    /* 부모 프로세스: 자식 프로세스 종료를 기다림 */
    if (pid != 0) {
        wait(NULL);  // 자식 프로세스가 종료될 때까지 대기
        printf("who 명령어 실행 완료. 결과는 userlist에 저장되었습니다.\n");
    }

    return 0;
}
