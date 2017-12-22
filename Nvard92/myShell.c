#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>

char *fgetstr(char *string, int n, FILE *file)
{
	char *result;
	result = fgets(string, n, file);
	if(!result)
		return result;
	if(string[strlen(string) -1] == '\n')
			string[strlen(string) - 1] = 0;

		return string;
}

int main()
{
	char line[1000];
	char *token;
	char *args[20];
 while(1)
 
{
	printf("\n$ ");
	fgetstr(line, 1000, stdin);
	char *amp="&";
	int bg=0;
	args[0] = strtok(line, " ");
	int i = 1;

	do
	{
		token = strtok(NULL, " ");
		args[i] = token;
		i++;
	} while(token !=NULL);

	if(!strcmp(args[i-2], amp))
	{
		args[i-2] = NULL;
		bg = 1;
	}

	fflush(NULL);
	pid_t pid;
	pid = fork();

	if(pid < 0)
	{	
		printf(" forking failed\n");
	}

	if(pid != 0)
	{ 
		printf(" parent process with id %d\n", (int)getpid());
	  if(bg)
	  {	
	  	printf("waiting");
		wait(NULL);
        }

	}
	else
	{ 
	    printf(" child process with id %d\n ", (int)getpid());
	    if(execvp(args[0], args) == -1)
	    	printf("execvp failed");

	    execvp(args[0], args);
	 }
        }
	return 0;
}




