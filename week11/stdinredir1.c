/* stdinredir1.c
 * 목적: 파일 디스크립터 0(표준 입력)을 파일로 연결하여 리다이렉트하는 방법을 보여줌.
 * 동작: 
 * 1. 표준 입력으로부터 3줄을 읽어 출력.
 * 2. fd 0(표준 입력)을 닫고 디스크 파일을 열어 리다이렉트.
 * 3. 새로 리다이렉트된 입력에서 3줄을 읽어 출력.
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

int main() {
    int fd;              // 파일 디스크립터
    char line[100];      // 줄 데이터를 저장할 버퍼

    /* 표준 입력에서 3줄 읽고 출력 */
    fgets(line, 100, stdin); printf("%s", line);
    fgets(line, 100, stdin); printf("%s", line);
    fgets(line, 100, stdin); printf("%s", line);

    /* 표준 입력 리다이렉트 */
    close(0);   // 표준 입력(file descriptor 0) 닫기
    fd = open("/etc/passwd", O_RDONLY);  // /etc/passwd 파일을 읽기 전용으로 열기
    if (fd != 0) {  // fd 0으로 열리지 않았다면 에러 처리
        fprintf(stderr, "파일을 fd 0으로 열 수 없습니다.\n");
        exit(1);
    }

    /* 새로 리다이렉트된 입력에서 3줄 읽고 출력 */
    fgets(line, 100, stdin); printf("%s", line);
    fgets(line, 100, stdin); printf("%s", line);
    fgets(line, 100, stdin); printf("%s", line);

    return 0;
}
