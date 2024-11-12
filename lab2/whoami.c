#include <stdio.h>
#include <pwd.h>
#include <sys/types.h>
#include <unistd.h>

int main() {
    // 현재 사용자의 UID 가져오기
    uid_t uid = getuid();
    
    // UID에 해당하는 사용자 정보 가져오기
    struct passwd *pw = getpwuid(uid);

    // 사용자 이름 출력
    if (pw) {
        printf("%s\n", pw->pw_name);  // 사용자 이름 출력
    } else {
        perror("getpwuid");  // 사용자 정보를 가져오는 데 실패한 경우
        return 1;
    }

    return 0;
}
