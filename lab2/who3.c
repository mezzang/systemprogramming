#include <stdio.h>
#include <utmp.h>
#include <fcntl.h>
#include <unistd.h>
#include <time.h>

int utmp_open(char *);
void utmp_close();
void show_info(struct utmp *utbufp);
void show_time(long timeval);
struct utmp *utmp_next();

int main() {
    struct utmp *utbufp;; // utmp 구조체 레코드
                   
    // utmp 파일 열기
    if ((utmp_open( UTMP_FILE )) == -1) {
        perror(UTMP_FILE); // 오류 메시지 출력
        return 1;          // 비정상 종료
    }

    // utmp 파일에서 각 레코드 읽기
    while ((utbufp = utmp_next()) != NULL){
   
          show_info(utbufp);
    } // 읽은 레코드 출력
    

    utmp_close(); // 파일 닫기
    return 0;      // 정상 종료
}

// utmp 구조체의 정보 출력
void show_info(struct utmp *utbufp) {
    if (utbufp->ut_type != USER_PROCESS) {
        return; // 로그인된 사용자만 출력
    }

    // 사용자 이름, 터미널, 로그인 시간, 호스트 정보 출력
    printf("%-11.11s ", utbufp->ut_user);   // 사용자 이름
    printf("%-8.8s \t", utbufp->ut_line);   // 터미널 이름
    show_time(utbufp->ut_tv.tv_sec);      // 로그인 시간
    printf(" (%s)\n", utbufp->ut_host);   // 호스트 정보
}

// 시간을 "월 일 시간:분" 형식으로 출력하는 함수
void show_time(long timeval) {
    char buf[64];
    struct tm *timeinfo;

    timeinfo = localtime(&timeval);  // 초 단위의 시간을 구조체로 변환
    strftime(buf, sizeof(buf), "%Y-%m-%d %H:%M", timeinfo);  // 시간 형식을 지정하여 buf에 저장
    printf("%s", buf);  // 저장된 시간 형식을 출력
}
