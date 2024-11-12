#include <stdio.h>
#include <dirent.h> // 디렉토리 관련 함수들을 사용하기 위해 필요한 헤더

int main(int argc, char *argv[]) {
    struct dirent *entry;   // 디렉토리의 각 항목을 저장할 구조체
    DIR *dp;                // 디렉토리 포인터

    // 만약 인자가 주어지지 않으면 현재 디렉토리(".")를 사용
    const char *directory = (argc > 1) ? argv[1] : ".";

    // 디렉토리 열기
    dp = opendir(directory);
    if (dp == NULL) {
        perror("opendir");  // 디렉토리를 열지 못했을 때 에러 메시지 출력
        return 1;
    }

    // 디렉토리 내 항목들을 읽어서 출력
    while ((entry = readdir(dp)) != NULL) {
        printf("%s\n", entry->d_name);  // 항목 이름 출력
    }

    // 디렉토리 닫기
    closedir(dp);

    return 0;
}