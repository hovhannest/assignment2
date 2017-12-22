#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <memory.h>

char* join(char* first,char* second)							// retursn result consisted of joined  strings (i.e char* s)
{
	char* result = (char*) malloc(strlen(first) + strlen(second) + 1);
	strcpy(result, first);
	strcat(result, second);
	return result;
}

int execute_command( char* args[], int count)
{
	pid_t pid, cpid;		// process id and child process id
	int exitStatus;			//the exit status
	char* path = getenv("PATH");	// we get string with the content in PATH that is libraries for functions

	int bBG =0 ;				// boolean BG is true when process is background


	if(strcmp(args[count-1],"&")==0)	// we check if program should be ran in the background
	{
		bBG= 1;
		args[count-1] =NULL;
	}

	int succeeded =0;					// if it isn't 1 then we try other path libraries			

	pid = fork();
	if(pid == 0)
	{
		char* dirName = strtok(path, ":");	//tokenizing using the symbol :

		while(dirName != NULL)				// basically we go over all libraries in Path
		{
			char* cmndName = join(join(dirName,"/"),args[0]); 
			int res = execvp(cmndName, args);
			free(cmndName);
			if(res != -1)					// if not -1 we suceeded to find the command
			{
				succeeded = 1;
				dirName == NULL;
			}
			if(dirName!=NULL)
				dirName = strtok(NULL,":");
		}
		if(succeeded == 0)					// no libraries in PATH fit we look in our current directory
		{
			char* dirName = getenv("PWD");	
			char* cmndName = join(join(dirName,"/"),args[0]);
			int res = execvp(cmndName, args);
			if(res != -1)
				succeeded =1;

			free(cmndName);
		}

		if(succeeded == 0)					// Command wasn't found in any library inside PATH or in the current directory
		{
			perror("Invalid command");
		}

		exit(EXIT_FAILURE);
	}
	else if(pid < 0)						// Fork failed
	{
		perror("Unfortunately shell wasn't able to fork :'(");
	}
	else
	{
		if(!bBG)					// if it wasn't a backgroudn process we have to wait until it finishes.
		{
			do
			{
				cpid = waitpid(pid, &exitStatus, WUNTRACED);
			}while(!WIFEXITED(exitStatus)&& !WIFSIGNALED(exitStatus));
		}
		else
		{
			wait(NULL);
		}
	}

	return 1;
}