/* stdinredir2.c
 * 표준 입력을 리다이렉트하는 두 가지 방법을 보여줍니다.
 * #define을 사용하여 하나의 방법을 선택하세요.
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

// 아래 중 하나를 활성화하세요:
// #define CLOSE_DUP   // open, close, dup, close 방법
/* #define USE_DUP2 */ // open, dup2, close 방법

int main() {
    int fd;        // 파일 디스크립터
    int newfd;     // 새 파일 디스크립터
    char line[100]; // 데이터를 저장할 버퍼

    /* 표준 입력에서 3줄 읽고 출력 */
    fgets(line, 100, stdin); printf("%s", line);
    fgets(line, 100, stdin); printf("%s", line);
    fgets(line, 100, stdin); printf("%s", line);

    /* 입력 리다이렉트 */
    fd = open("/etc/passwd", O_RDONLY);  // 디스크 파일을 열기
#ifdef CLOSE_DUP
    close(0);            // 표준 입력(file descriptor 0) 닫기
    newfd = dup(fd);      // fd를 복사하여 0에 설정
#else
    newfd = dup2(fd, 0);  // fd를 0에 복사하고 기존 0을 닫음
#endif
    if (newfd != 0) {    // 복사가 실패하면 에러 처리
        fprintf(stderr, "fd를 0으로 복사할 수 없습니다.\n");
        exit(1);
    }
    close(fd);           // 원래 fd 닫기

    /* 새로 설정된 입력에서 3줄 읽고 출력 */
    fgets(line, 100, stdin); printf("%s", line);
    fgets(line, 100, stdin); printf("%s", line);
    fgets(line, 100, stdin); printf("%s", line);

    return 0;
}
