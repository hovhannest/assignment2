#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>


char *fgetstr(char *string, int n, FILE *stream)
{
	char *result;
	result = fgets(string, n, stream);
	if(!result)
		return result;
	if(string[strlen(string) -1] == '\n')
		string[strlen(string) - 1] = 0;
	
	return(string);

}

int main()
{	
	char line[1000];
	char *token;

	char *args[10];


  while(1)
  {

	printf("\n$ ");
	fgetstr(line, 1000, stdin);
	char *s = "&";
	int isBackground = 0;
	args[0] = strtok(line, " ");  #We take the part of the command beforethe first " "
	int i = 1;

	do
	{
		token = strtok(NULL, " ");  #To signal strtok() that you want to keep searching the same string, you pass a NULL pointer as its first argument
		args[i] = token;
		i++;
	} while(token !=NULL);


	if(!strcmp(args[i-2], s)) #strcmp will return 0 then args[i-2] is &, so we do !strcmp to get 1
	{
		args[i-2] = NULL;
		bg = 1;
	}


	fflush(NULL); #flushes all open output streams
	pid_t pid;
	pid = fork(); 
	
	if(pid < 0) 
	{
		printf("Forking process failed\n");
	}
	if(pid != 0) #Fork returns a positive value(the process ID of the child process to the parent process)
	{
           printf("this is the parent process with id %d\n", (int)getpid());
	   if(bg)
	   {
	   	printf("Waiting");
		wait(NULL);
           }

	}
	else #fork return 0 to the child process,so this is a child process
	{
	   printf("this is the child process with id %d\n", (int)getpid());
	   if(execvp(args[0], args) == -1) #execvp returns value only if an error occured, andthat value is -1
		printf("execvp FAILED");
	   
	   execvp(args[0], args);
	}
  }
  return 0;
}