#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdbool.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>

#define N 1000
#define M 50

char input[N];
size_t n = N;
char *args[M];

void fork_error();
void child_process(char * input, bool bg);
void default_(bool bg, pid_t pid);


int main() {

    while (1) {
        printf("\n$ ");
        fgets(input, n, stdin);
        int length = strlen(input);
        if (input[length-1] == '\n') {
            input[--length] = 0;
        }
        printf("input  = %s\n", input);
        
        bool bg = (input[length-1] == '&');
        
        if (bg) {
            printf("!!!!!!!!!bg\n");
        }
        
        pid_t pid;

        fflush(NULL);
        
        switch(pid=fork()) {
            case -1:
                fork_error();
            case 0:
                child_process(input, bg);
            default:
                default_(bg, pid);
        }
    }
    return 0;
}


void fork_error () {
    printf("Error accured in fork.\n");
    exit(1);
}


void child_process (char * input, bool bg) {
    printf("Child process: \n");
    args[0] = strtok(input, " ");
    
    int args_cnt = 0;
    
    while (args[args_cnt] != NULL) {
        args_cnt++;
        args[args_cnt] = strtok(NULL, " ");
    }

    if (bg) { 
        args[--args_cnt] = NULL;
    }

    if (execvp (args[0], args) == -1) {
        printf("Error accured in exec.\n");
    }
}


void default_(bool bg, pid_t pid) {
    printf("It is a parrent process.\n");
    printf("Parend pid - %d\n", getpid());
    printf("Child pid -  %d\n", pid);
    if(bg) {
        wait(NULL);
    }
}
