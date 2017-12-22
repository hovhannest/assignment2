#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <sys/types.h>
int main(int argc, char* args[] ){
 printf("Welcome to custom shell written by C language\n");

int r = 0;
    char buff[100];
char* token;
char* arg[10];
int runBG = 0;
printf("$\n");
    fgets(buff, 100, stdin);
    if (sizeof(buff) == 0) {
        printf ("\nNo input\n");
        return 1;
    }
	else{
		printf("%s", buff);
const char s[2] = " ";
   // observe first token
		token = strtok(buff, s);
   
   // walk through other tokens
int count = 0;
arg[0] = token;
//printf("arg0 token %s", token);
   while( token != NULL ) {
      token = strtok(NULL, s);
//printf("token : %s", token);
arg[++count] = token;
   }

//printf("%s", arg[count-2]);

//printf("%s", arg[count-2]);

	if(strcmp(arg[count-2], "&")==0)
	{
runBG = 1;
		printf("is background");
	
}

pid_t processID = fork();
printf("%d", processID);
if(processID > 0){
	printf(" this process is a child process, its parent id is %d", (int)getpid());
if((int)getpid()>0){
	if(execvp(arg[0], arg) == -1){
printf("%s", arg[0]);
int i = count-2;
    //while ( i < count-2 ) printf( "%s\n", arg[i++] );
//printf("you can see %s", arg[0]);
		printf("an invalid command received");
	} else{
		printf("valid Command");
		execvp(arg[0], arg);

	}
	}
else{
printf("this process doesn't have a parent ID");

if(runBG == 0)
	  {	
	  	printf(" its a foreground process\n");
		wait(NULL);
        }
else{
printf("command line is over");}

}
}

	}
 return 0; 
}

