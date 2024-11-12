#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>

#define BUFFER_SIZE 1024

void copy_file(const char *src, const char *dest);

int main(int argc, char *argv[]) {
    // 인자의 개수가 올바른지 확인 (복사할 파일과 대상 파일이 필요)
    if (argc != 3) {
        fprintf(stderr, "Usage: %s <source file> <destination file>\n", argv[0]);
        exit(1);
    }

    // 파일 복사 함수 호출
    copy_file(argv[1], argv[2]);

    return 0;
}

void copy_file(const char *src, const char *dest) {
    int src_fd, dest_fd;
    char buffer[BUFFER_SIZE];
    ssize_t bytes_read, bytes_written;

    // 소스 파일 열기 (읽기 전용)
    src_fd = open(src, O_RDONLY);
    if (src_fd == -1) {
        perror("Error opening source file");
        exit(1);
    }

    // 대상 파일 열기 (쓰기 전용, 없으면 새로 생성, 있으면 덮어쓰기)
    dest_fd = open(dest, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (dest_fd == -1) {
        perror("Error opening destination file");
        close(src_fd);
        exit(1);
    }

    // 소스 파일에서 데이터를 읽어 대상 파일에 쓰기
    while ((bytes_read = read(src_fd, buffer, BUFFER_SIZE)) > 0) {
        bytes_written = write(dest_fd, buffer, bytes_read);
        if (bytes_written != bytes_read) {
            perror("Error writing to destination file");
            close(src_fd);
            close(dest_fd);
            exit(1);
        }
    }

    if (bytes_read == -1) {
        perror("Error reading source file");
    }

    // 파일 닫기
    close(src_fd);
    close(dest_fd);
}
