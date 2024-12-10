/* timeclnt.c - timeserv.c 서버용 클라이언트
 * 사용법: timeclnt hostname portnumber
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <strings.h>

#define oops(msg) { perror(msg); exit(1); }

int main(int ac, char *av[]) {
    struct sockaddr_in servadd; /* 호출할 서버의 주소 */
    struct hostent *hp;         /* 호스트 이름으로 IP를 얻기 위한 구조체 */
    int sock_id, sock_fd;       /* 소켓 및 파일 디스크립터 */
    char message[BUFSIZ];       /* 메시지를 받을 버퍼 */
    int messlen;                /* 메시지 길이 */

    /*
     * 1단계: 소켓 생성
     */
    sock_id = socket(AF_INET, SOCK_STREAM, 0); /* 소켓 생성 */
    if (sock_id == -1)
        oops("socket"); /* 실패 시 오류 출력 */

    /*
     * 2단계: 서버 연결 설정
     * 먼저 서버 주소(host, port)를 설정해야 함
     */
    bzero(&servadd, sizeof(servadd)); /* 주소 구조체 초기화 */
    hp = gethostbyname(av[1]);        /* 호스트 이름의 IP 조회 */
    if (hp == NULL)
        oops(av[1]);                 /* 조회 실패 시 오류 출력 */
    bcopy(hp->h_addr, (struct sockaddr *)&servadd.sin_addr, hp->h_length); /* IP 복사 */

    servadd.sin_port = htons(atoi(av[2])); /* 포트 번호 설정 */
    servadd.sin_family = AF_INET;          /* 주소 체계 설정 */

    if (connect(sock_id, (struct sockaddr *)&servadd, sizeof(servadd)) != 0)
        oops("connect"); /* 연결 실패 시 오류 출력 */

    /*
     * 3단계: 서버에서 데이터 전송 받기, 이후 연결 해제
     */
    messlen = read(sock_id, message, BUFSIZ); /* 데이터 읽기 */
    if (messlen == -1)
        oops("read"); /* 읽기 실패 시 오류 출력 */

    if (write(1, message, messlen) != messlen) /* 데이터를 표준 출력으로 쓰기 */
        oops("write"); /* 쓰기 실패 시 오류 출력 */

    close(sock_id); /* 소켓 닫기 */

    return 0;
}
