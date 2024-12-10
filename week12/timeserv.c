/* timeserv.c - 소켓 기반의 현재 시간 제공 서버 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <netdb.h>
#include <time.h>
#include <strings.h>

#define PORTNUM 11231  /* 현재 시간 서비스를 제공하는 포트 번호 */
#define HOSTLEN 256
#define oops(msg) { perror(msg); exit(1); }

int main(int ac, char *av[]) {
    struct sockaddr_in saddr;   /* 주소 정보를 저장할 구조체 */
    struct hostent *hp;         /* 호스트 이름과 관련된 정보 */
    char hostname[HOSTLEN];     /* 호스트 이름 */
    int sock_id, sock_fd;       /* 소켓 ID 및 파일 디스크립터 */
    FILE *sock_fp;              /* 소켓을 스트림으로 사용할 포인터 */
    char *ctime();              /* 시간을 문자열로 변환하는 함수 */
    time_t thetime;             /* 현재 시간을 저장 */

    /*
     * 1단계: 커널에서 소켓을 요청
     */
    sock_id = socket(PF_INET, SOCK_STREAM, 0); /* 소켓 생성 */
    if (sock_id == -1)
        oops("socket");

    /*
     * 2단계: 소켓에 주소를 바인딩. 주소는 호스트와 포트를 포함
     */
    bzero((void *)&saddr, sizeof(saddr)); /* 구조체를 초기화 */
    gethostname(hostname, HOSTLEN);      /* 현재 호스트 이름 가져오기 */
    hp = gethostbyname(hostname);        /* 호스트 이름에 대한 정보 가져오기 */
    
    bcopy((void *)hp->h_addr, (void *)&saddr.sin_addr, hp->h_length); /* 주소 복사 */
    saddr.sin_port = htons(PORTNUM);     /* 포트 번호 설정 */
    saddr.sin_family = AF_INET;          /* 주소 체계 설정 */

    if (bind(sock_id, (struct sockaddr *)&saddr, sizeof(saddr)) != 0)
        oops("bind");

    /*
     * 3단계: 소켓을 연결 대기 상태로 설정 (큐 크기: 1)
     */
    if (listen(sock_id, 1) != 0)
        oops("listen");

    /*
     * 메인 루프: accept(), write(), close()
     */
    while (1) {
        sock_fd = accept(sock_id, NULL, NULL); /* 클라이언트 연결 요청 대기 */
            printf("Wow! got a call!\n");         /* 연결 확인 메시지 출력 */
        if (sock_fd == -1)
            oops("accept");                   /* 연결 실패 시 오류 처리 */

        sock_fp = fdopen(sock_fd, "w");       /* 소켓을 스트림으로 변환 */
        if (sock_fp == NULL)
            oops("fdopen");                   /* 스트림 변환 실패 시 오류 처리 */

        thetime = time(NULL);                 /* 현재 시간 가져오기 */
        fprintf(sock_fp, "The time here is ..\n");
        fprintf(sock_fp, "%s", ctime(&thetime)); /* 시간 문자열 출력 */
        fclose(sock_fp);                      /* 소켓 스트림 닫기 */
    }
}
