/* rlsd.c - 원격 ls 서버 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <time.h>
#include <ctype.h>
#include <strings.h>

#define PORTNUM 15000          /* 원격 ls 서버 포트 번호 */
#define HOSTLEN 256
#define oops(msg) { perror(msg); exit(1); }

void sanitize(char *);         /* 입력을 정리하는 함수 */

int main(int ac, char *av[]) {
    struct sockaddr_in saddr;  /* 주소를 설정하는 구조체 */
    struct hostent *hp;        /* 호스트 이름 정보 */
    char hostname[HOSTLEN];    /* 호스트 이름 */
    int sock_id, sock_fd;      /* 소켓 ID, 파일 디스크립터 */
    FILE *sock_fpi, *sock_fpo; /* 입력 및 출력 스트림 */
    FILE *pipe_fp;             /* popen을 사용해 ls 실행 */
    char dirname[1024];      /* 클라이언트로부터 받은 디렉토리 이름 */
    char command[BUFSIZ];      /* popen에서 사용할 명령 */
    int dirlen, c;             /* 디렉토리 이름 길이, 캐릭터 변수 */

    /*
     * 1단계: 커널에 소켓 요청
     */
    sock_id = socket(PF_INET, SOCK_STREAM, 0); /* 소켓 생성 */
    if (sock_id == -1)
        oops("socket");

    /*
     * 2단계: 소켓에 주소 바인딩 (호스트 및 포트 설정)
     */
    bzero((void *)&saddr, sizeof(saddr));  /* 구조체 초기화 */
    gethostname(hostname, HOSTLEN);       /* 현재 호스트 이름 얻기 */
    hp = gethostbyname(hostname);         /* 호스트 이름 정보 얻기 */
    bcopy((void *)hp->h_addr, (void *)&saddr.sin_addr, hp->h_length); /* 주소 복사 */
    saddr.sin_port = htons(PORTNUM);      /* 포트 번호 설정 */
    saddr.sin_family = AF_INET;           /* 주소 체계 설정 */

    if (bind(sock_id, (struct sockaddr *)&saddr, sizeof(saddr)) != 0)
        oops("bind");

    /*
     * 3단계: 소켓을 연결 대기 상태로 설정 (큐 크기 1)
     */
    if (listen(sock_id, 1) != 0)
        oops("listen");

    /*
     * 메인 루프: accept(), write(), close()
     */
    while (1) {
        sock_fd = accept(sock_id, NULL, NULL); /* 연결 대기 */
        if (sock_fd == -1)
            oops("accept");

        /* 읽기 스트림 열기 */
        if ((sock_fpi = fdopen(sock_fd, "r")) == NULL)
            oops("fdopen reading");

        if (fgets(dirname, sizeof(dirname), sock_fpi) == NULL)
            oops("reading dirname");
        sanitize(dirname); /* 디렉토리 이름 정리 */

        /* 쓰기 스트림 열기 */
        if ((sock_fpo = fdopen(sock_fd, "w")) == NULL)
            oops("fdopen writing");

        /* 명령 생성 및 실행 */
        sprintf(command,"ls %s", dirname);
        
        if ((pipe_fp = popen(command, "r")) == NULL)
            oops("popen");

        /* ls 결과를 소켓으로 전송 */
        while ((c = getc(pipe_fp)) != EOF)
            putc(c, sock_fpo);
        pclose(pipe_fp);
        fclose(sock_fpo);
        fclose(sock_fpi);
    }

    return 0;
}

void sanitize(char *str) {
/*
 * 만약 사용자가 "ls ; rm *" 같은 명령을 입력하면
 * "ls ; rm *" 명령이 실행될 위험이 있음.
 * 따라서 슬래시('/')와 알파벳/숫자 이외의 문자를 모두 제거.
 * 더 나은 방법은 연습 문제에서 다룰 수 있음.
 */
    char *src, *dest;
    for (src = dest = str; *src; src++) {
        if (*src == '/' || isalnum(*src))
            *dest++ = *src;
    }
    *dest = '\0';
}
