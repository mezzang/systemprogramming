/* rls.c - 원격 디렉토리 목록 서비스를 위한 클라이언트
 * 사용법: rls hostname directory
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <string.h>

#define oops(msg) { perror(msg); exit(1); }
#define PORTNUM 15000  /* 서버가 사용하는 포트 번호 */

int main(int ac, char *av[]) {
    struct sockaddr_in servadd; /* 연결할 서버의 주소 */
    struct hostent *hp;         /* 호스트 이름을 통해 IP를 얻기 위한 구조체 */
    int sock_id, sock_fd;       /* 소켓 ID 및 파일 디스크립터 */
    char buffer[BUFSIZ];        /* 메시지를 받을 버퍼 */
    int n_read;                 /* 읽은 메시지의 길이 */

    if (ac != 3) exit(1);       /* 매개변수 부족 시 종료 */

    /*
     * 1단계: 소켓 생성
     */
    sock_id = socket(AF_INET, SOCK_STREAM, 0); /* 소켓 생성 */
    if (sock_id == -1)
        oops("socket"); /* 소켓 생성 실패 시 오류 출력 */

    /*
     * 2단계: 서버에 연결
     */
    bzero(&servadd, sizeof(servadd)); /* 주소 구조체 초기화 */
    hp = gethostbyname(av[1]);        /* 호스트 이름의 IP 조회 */
    if (hp == NULL)
        oops(av[1]);                 /* 호스트 이름 조회 실패 시 종료 */

    bcopy(hp->h_addr, (struct sockaddr *)&servadd.sin_addr, hp->h_length); /* IP 복사 */
    servadd.sin_port = htons(PORTNUM); /* 포트 번호 설정 */
    servadd.sin_family = AF_INET;      /* 주소 체계 설정 */

    if (connect(sock_id, (struct sockaddr *)&servadd, sizeof(servadd)) != 0)
        oops("connect"); /* 연결 실패 시 오류 출력 */

    /*
     * 3단계: 디렉토리 이름 전송 및 결과 읽기
     */
    if (write(sock_id, av[2], strlen(av[2])) == -1) /* 디렉토리 이름 전송 */
        oops("write");

    if (write(sock_id, "\n", 1) == -1) /* 줄바꿈 전송 */
        oops("write");

    while ((n_read = read(sock_id, buffer, BUFSIZ)) > 0) { /* 서버로부터 데이터 읽기 */
        if (write(1, buffer, n_read) == -1) /* 표준 출력으로 데이터 쓰기 */
            oops("write");
    }

    close(sock_id); /* 소켓 닫기 */

    return 0;
}
