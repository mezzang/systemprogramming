//smsh.c
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include "smsh.h"

#define DFL_PROMPT "> "

//간단한 쉘 프로그램 구현
int main()
{
    //사용자로부터 입력받은 명령어 문자열, 쉘 프롬프트 문자열, 명령어와 인자 목록, 명령 실행 결과
    char *cmdline, *prompt, **arglist;
    int result;
    void setup(); //SIGINT와 SIGQUIT을 무시하도록 설정

    prompt = DFL_PROMPT;
    setup();

    while((cmdline = next_cmd(prompt, stdin)) != NULL){
        if((arglist = splitline(cmdline)) != NULL){
            result = execute(arglist); //명령어 실행, 결과 반환
            freelist(arglist); //arglist 메모리를 해제
        }
        free(cmdline); //메모리 해제
    }
    return 0;
}
void setup() //쉘의 초기 설정
{
    signal(SIGINT, SIG_IGN);
    signal(SIGQUIT, SIG_IGN);

}

void fatal(char *s1, char *s2, int n) //에러 메시지 출력
{
    fprintf(stderr, "Error: %s, %s\n", s1, s2);
    exit(n);
}