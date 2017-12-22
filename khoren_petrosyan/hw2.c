#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>

char *args[20];
char cmd[1000];

int main()
{
    while(1)
    {
        printf("\n$ "); // print prompt sign $
        fgets(cmd, 1000, stdin); // read input command
        int l = strlen(cmd);
        if(cmd[l-1] == '\n') cmd[--l] = 0;

            
        bool send_to_bckgr = (cmd[l-1] == '&');        
        pid_t pid;
                
        switch(pid=fork()) {
            case -1:
                printf("Error: unable to fork!\n");
                return -1; 
            case 0:
                 printf("This is a child process\n");
                args[0] = strtok (cmd, " ");
                int args_cnt;
                while(args[args_cnt] != NULL){
                    args_cnt++;
                    args[args_cnt] = strtok(NULL, " ");
                }
                if(send_to_bckgr) args[--args_cnt] = NULL;    
                
                if(execvp (args[0], args) == -1)
                    printf("An error accured (exec)\n");
                break;
                
            default:
                printf("It's a parrent process\n");
                printf("ppid - %d\n", getpid());
                printf("cpid -  %d\n",pid);
                
                if(send_to_bckgr) 
                    wait(NULL);
        }
    }
    return 0;
}








