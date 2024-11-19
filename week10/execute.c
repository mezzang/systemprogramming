//execute.c
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <string.h>
#include <sys/wait.h>

//주어진 명령어(argv[])를 실행하고 실행 결과 상태를 반환
int execute(char *argv[])
{
    int pid;
    int child_info = -1;
    int is_background = 0;

    if(argv[0] == NULL) //명령어 확인
        return 0;
    
    int last_arg_index = 0;
    while(argv[last_arg_index] != NULL){
        last_arg_index++;
    }
    last_arg_index--;
    if(strcmp(argv[last_arg_index], "&") == 0){
        argv[last_arg_index] = NULL;
        is_background = 1;
    }

    if((pid = fork()) == -1)
        perror("fork");
    else if(pid == 0) { //자식
        signal(SIGINT,SIG_DFL);
        signal(SIGQUIT, SIG_DFL);
        execvp(argv[0], argv);
        perror("cannnot execute command");
        exit(1);
    }
    else { //부모(자식이 종료될 때 까지 기다린다.)
        if(!is_background){
            if(wait(&child_info) == -1)
            perror("wait");
        }
        else
        {
            printf("[Background] Process started with PID: %d\n", pid);
        }
        
    }
    return child_info;
}

