#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>

int main (int argc,char *argv[])
{
	printf("My Pid is : %d\n",(int) getpid());
	pid_t pid=fork();
	printf("fork returned: %d\n",(int) pid);
	if(pid<0){
	perror("fork failed");
	
	}

	if (pid==0){
		printf("child with this pid %d\n",(int) getpid());
		}else if(pid >0){
		printf("The parent\n");
		}
	return 0;
	}








