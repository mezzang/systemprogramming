#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>

#define BUFFER_SIZE 1024

int main(int argc, char *argv[]) {
    // 입력 인자 체크
    if (argc != 3) {
        fprintf(stderr, "Usage: %s source_file target_directory\n", argv[0]);
        return 1;
    }

    char *source_file = argv[1];
    char *target = argv[2];

    // 소스 파일 열기
    int source_fd = open(source_file, O_RDONLY);
    if (source_fd < 0) {
        perror("Error opening source file");
        return 1;
    }

    struct stat statbuf;
    
    // 두 번째 인자가 디렉토리인지 확인
    if (stat(target, &statbuf) == 0 && S_ISDIR(statbuf.st_mode)) {
        // 디렉토리면 파일 경로를 구성
        char target_file[BUFFER_SIZE];
        snprintf(target_file, sizeof(target_file), "%s/%s", target, source_file);

        target = target_file;
    }

    // 타겟 파일 열기 (디렉토리 이름을 이용해 동일한 이름으로 생성)
    int target_fd = open(target, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (target_fd < 0) {
        perror("Error opening target file");
        close(source_fd);
        return 1;
    }

    // 파일 복사
    char buffer[BUFFER_SIZE];
    ssize_t bytes;
    while ((bytes = read(source_fd, buffer, BUFFER_SIZE)) > 0) {
        if (write(target_fd, buffer, bytes) != bytes) {
            perror("Error writing to target file");
            close(source_fd);
            close(target_fd);
            return 1;
        }
    }

    if (bytes < 0) {
        perror("Error reading from source file");
    }

    // 파일 닫기
    close(source_fd);
    close(target_fd);

    return 0;
}
