#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>

int main ()
{
	while(1)
	{
	char *str1;
	printf("$" );
 	scanf("%[^\n]s", str1);
 	int isBackground=0;
 	// printf("1\n");
 	char *args[20];
 	// printf("2\n");
 	args[0] = strtok(str1, " ");
 	// printf("3\n");
	int i=1;
	int status;
 	while(1)
 	{
 		 // printf("4\n");

 		char* token = strtok(NULL, " ");
 		printf("%s\n", token);
 		if(token==NULL)
 			break;
 		int ret = strcmp(token, "&");
 		if(ret==0)
 		{
 			isBackground=1;
 		}
 		else{
 			args[i]=token;
 			i++;
 		}
 	}

 	pid_t pid;
	pid = fork();
	if(pid < 0)
	{
		printf("print error\n");
	}
	else if(pid != 0)
	{
           printf("this is not a child\n");
	   if(isBackground==1)
	   {
	   	printf("wait for the child process to end\n ");
		wait();
        }

	}
	else if(pid==0)
	{
		printf("execute the given program with arguments\n");
		int result= execvp(args[0], args);
		if(result==-1)
			printf("print error\n");
	}

 	}
 	return 0;

}