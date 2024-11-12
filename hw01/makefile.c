#include <stdio.h>
#include <stdlib.h>

int main() {
    FILE *fp = fopen("input.txt", "w");
    if (fp == NULL) {
        perror("Error opening file");
        return 1;
    }

    for (int i = 0; i < 2048; i++) {
        fputc('A', fp); // 2KB 크기의 'A' 문자로 채움
    }

    fclose(fp);
    printf("2KB 파일 'input.txt'가 생성되었습니다.\n");
    return 0;
}
