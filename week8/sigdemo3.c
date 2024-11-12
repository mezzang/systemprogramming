#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <signal.h>

#define INPUTLEN 100

void inthandler(int);
void quithandler(int);

int main(int ac, char *av[])
{
    void inthandler(int);
    void quithandler(int);
    char input[INPUTLEN];
    int nchars;

    signal(SIGINT, inthandler); //Ctrl+c 가 SIGINT 미리 정의된 신호 상수이다.
    signal(SIGQUIT, quithandler); //Ctrl + \가 SIGQUIT

    do {
        printf("\nType a message\n");
        nchars = read(0, input, (INPUTLEN - 1)); //표준입력(파일디스크립터0)으로부터 읽어들여 input 배열에 저장한다.
        if(nchars == -1)
            perror("read returned an error");
        else {
            input[nchars] = '\0';
            printf("You typed: %s", input); //입력한 input 배열 출력
        }
    }
    while (strncmp(input, "quit" , 4) != 0); //quit 까지 반복
    
    return 0;
    
}

void inthandler(int s)
{
    printf("Received signal %d .. waiting\n", s);
    sleep(2);
    printf(" Leaving inthandler\n");
}

void quithandler(int s)
{
    printf("Received signal %d .. waiting\n", s);
    sleep(3);
    printf(" Leaving quithandler\n");
}