#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>

void execute(char **, int, char **);
int parse(char *, char **, char **, int *);

#define INPUT_STRING_SIZE	80
#define NORMAL          	0
#define BACKGROUND      	1

int main(int argc, char *argv[])
{
    int i, mode = NORMAL, cmdArgc;
    size_t len = INPUT_STRING_SIZE;
    char *cpt, *inputString, *cmdArgv[INPUT_STRING_SIZE], *supplement = NULL;
    inputString = (char*)malloc(sizeof(char)*INPUT_STRING_SIZE);
    signal(SIGINT, SIG_IGN);
    char Directory[100];
    
    printf("Type 'quit' to exit\n");

    while(1)
    {
        mode = NORMAL;
        getcwd(Directory, 100);
        printf(">", getlogin(),Directory);
        getline( &inputString, &len, stdin);

        if(strcmp(inputString, "quit\n") == 0)
            exit(0);
        cmdArgc = parse(inputString, cmdArgv, &supplement, &mode);

        if(strcmp(*cmdArgv, "cd") == 0)
        {
            chdir(cmdArgv[1]);
        }
        else 
            execute(cmdArgv, mode, &supplement);
    }
    return 0;
}

int parse(char *inputString, char *cmdArgv[], char **supplementPtr, int *modePtr)
{
    int cmdArgc = 0, terminate = 0;
    char *srcPtr = inputString;
    while(*srcPtr != '\0' && terminate == 0)
    {
        *cmdArgv = srcPtr;
        cmdArgc++;
        while(*srcPtr != ' ' && *srcPtr != '\t' && *srcPtr != '\0' && *srcPtr != '\n' && terminate == 0)
        {
            if(*srcPtr == '&')
            {   
                *modePtr = BACKGROUND;
                break;
            }
            srcPtr++;
        }
        while((*srcPtr == '\t' || *srcPtr == '\n' || *srcPtr == ' ') && terminate == 0)
        {
            *srcPtr = '\0';
            srcPtr++;
        }
        cmdArgv++;
    }
    *cmdArgv = '\0';
    return cmdArgc;
}
void execute(char **cmdArgv, int mode, char **supplementPtr)
{
    pid_t pid;
    FILE *fp;
    int mode2 = NORMAL, cmdArgc, status1, status2;
    char *cmdArgv2[INPUT_STRING_SIZE], *supplement2 = NULL;
    pid = fork();
    
    if(pid == 0)
    {
        execvp(*cmdArgv, cmdArgv);
    }
    else if( pid < 0)
    {
        printf("Error");
        exit(-1);
    }
    else
    {
        if(mode == BACKGROUND)
                ;
        if(mode == NORMAL)
            waitpid(pid, &status1, 0);
    }
}