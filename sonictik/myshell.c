#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>

#define N 1000
#define M 20

char command[N];
size_t n = N;
char *args[M];

int main()
{
    while(true)
    {
        printf("\n$ ");
        fgets(command, n, stdin);
        int comm_len = strlen(command);
        if(command[comm_len-1] == '\n') command[--comm_len] = 0;
        
        //printf("command  = %s\n", command);
        
        bool bg = (command[comm_len-1] == '&');
        //if(bg) printf("!!!!!!!!!bg\n");
        
        pid_t pid;
        //int rv;
        
        fflush(NULL);
        
        switch(pid=fork()) {
            case -1:
                printf("An error accured (fork)\n");
                exit(1); /*выход из родительского процесса*/
            case 0:
                printf("child\n");
                args[0] = strtok (command, " ");
                int args_cnt = 0;
                while(args[args_cnt] != NULL)
                {
                    args_cnt++;
                    args[args_cnt] = strtok(NULL, " ");
                }
                //printf("argscnt = %d\n", args_cnt);
                if(bg) args[--args_cnt] = NULL;
                //printf("argscnt = %d\n", args_cnt);
                
                if(execvp (args[0], args) == -1)
                    printf("An error accured (exec)\n");
                
            default:
                printf("It's a parrent process\n");
                printf("PARENT PID - %d\n", getpid());
                printf("CHILD PID -  %d\n",pid);
                if(bg) 
                {
                    wait(NULL);
                }
        }
    }
    return 0;
}








