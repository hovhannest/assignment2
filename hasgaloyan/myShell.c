#include <unistd.h>
#include <sys/wait.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


int main()
{
	printf("%s","Shell starts..");
	char str[30];
	fgets(str, sizeof(str), stdin);
	
	char *command;
	char *cmdArgs[1000];
	int cmdCount = 0;

	char *tokens = strtok(str, " ");
	command = tokens; //first token is command

	while(tokens != NULL)
	{
		cmdArgs[cmdCount++] = tokens;
		tokens = strtok(NULL, " ");	
	}
	char *amp = "&";
	int isBckg = *cmdArgs[cmdCount-1] == *amp;  //is background proc?
	if(isBckg)
		cmdArgs[cmdCount-1] = NULL;	

	int status = 1;
	pid_t pid, wpid;
	pid = fork();
	if(pid == 0) //child process
	{
		int res = execvp(command, cmdArgs);
		if(res == -1)
			printf("%s", "This is invalid command");	
	}	
	else if(pid > 0 ) //parent
	{	
		if(!isBckg)
		{	
			while(!WIFEXITED(status) && !WIFSIGNALED(status))	
				wpid = waitpid(pid, &status, WUNTRACED);		
		}
		else{
			wait(NULL);
		}
	}
	else
		printf("Fork failed:(");
		
	

	return 0;
}
