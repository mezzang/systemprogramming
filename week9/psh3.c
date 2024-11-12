#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

#define MAXARGS 20
#define ARGLEN 100

void execute(char **);
char * makestring(char *);

int main()
{
    char *arglist[MAXARGS + 1];
    int numargs;
    char argbuf[ARGLEN];
    char *makestring();

    numargs = 0;
    while( numargs < MAXARGS)
    {
        printf("Args[%d]? ", numargs);
        if(fgets(argbuf,ARGLEN, stdin)){ //stdin에서 ARGLEN -1만큼을 읽고 끝에 널문자를 추가하여 argbuf에 저장한다.
            if(strcmp(argbuf, "exit\n") == 0){
                break;
            }
            if(*argbuf != '\n'){ //버퍼의 첫번째 문자가 엔터인지 *argbuf = argbuf[0]과 같다.
                arglist[numargs++] = makestring(argbuf);
            }
            else
            { //엔터를 입력받으면
                if (numargs > 0) {
                    arglist[numargs] = NULL;
                    execute(arglist);
                    numargs = 0;
                }
            }
        } else{
            break;
        }

    }
    return 0;
}

void execute(char *arglist[])
{
    int pid, exitstatus;
    pid = fork();
    switch (pid)
    {
    case -1:
        perror("fork failed");
        exit(1);
        
    case 0:
        execvp(arglist[0], arglist);
        perror("execvp failed");
        exit(1);
    default:
        while(wait(&exitstatus) != pid )
        ; // 반복문 자체가 조건을 만족할 때까지 기다리는 역할을 할 때 붙인다. 
        //자식 프로세스가 종료될 때까지 기다리는 역할
        printf("child exited with status %d %d\n", exitstatus>>8, exitstatus&0377);
        
    }
}

char *makestring(char * buf)
{
    char *cp;
    buf[strlen(buf)-1] = '\0';
    cp = malloc(strlen(buf) + 1);
    if(cp == NULL){ // 메모리 할당 에러
        fprintf(stderr, "no memory\n");
        exit(1);
    }
    strcpy(cp, buf); //메모리 할당 후 문자열 복사
    return cp;
}