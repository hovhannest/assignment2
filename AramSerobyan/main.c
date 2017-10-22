#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <memory.h>

//void print_stream();
/*
	To Be Done
	Tokenizing args
	Sending to Background with & 
	Commands in Path or in current directory
	Command Realization (?)
		fork
		wait if not background function
		exec() family for executing
	

*/
char* join(char* first,char* second)
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


	if(strcmp(args[count-1],"&")==0)	// we check if program should be ran
	{
		bBG= 1;
		args[count-1] =NULL;
	}

	int succeeded =0;				

	pid = fork();
	if(pid == 0)
	{
		char* dirName = strtok(path, ":");

		while(dirName != NULL)
		{
			char* cmndName = join(join(dirName,"/"),args[0]);
			int res = execvp(cmndName, args);
			free(cmndName);
			if(res != -1)
			{
				succeeded = 1;
				dirName == NULL;
			}
			if(dirName!=NULL)
				dirName = strtok(NULL,":");
		}
		if(succeeded == 0)
		{
			char* dirName = getenv("PWD");
			char* cmndName = join(join(dirName,"/"),args[0]);
			int res = execvp(cmndName, args);
			if(res != -1)
				succeeded =1;

			free(cmndName);
		}

		if(succeeded == 0)
		{
			perror("Invalid command");
		}

		exit(EXIT_FAILURE);
	}
	else if(pid < 0)
	{
		perror("Unfortunately shell wasn't able to fork :'(");
	}
	else
	{
		if(!bBG)
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


char** to_arrays( char* str )
{
	const int CHUNK = 256;
	int buff_size = CHUNK;

	char* arg = strtok(str, " \t\r\n\a");
	char** strings = malloc(CHUNK * sizeof(char*));
	
	int n =0;
	while(arg != NULL)
	{
		strings[n++] = arg;
		if(n>=CHUNK)
		{
			buff_size += CHUNK;
			strings = realloc(strings,CHUNK*sizeof(char *));
		}
		arg = strtok(NULL, " \t\r\n\a");
	}
	strings[n] = NULL;
	return strings;
}
/*
void to_arrays( char* strings[], char* string , size_t size )
{
	
	int i = 0; 
	int arrCount =0;
	int start =0;

	for(char * iter = string; *iter ;iter++ ) 
	{
		i++;
		if(*iter==32 || *iter == '\0')
		{
			strings[arrCount] = strndup(string + start, i-1);
			//string[arrCount] = (char*) malloc(i+1)
			//strncpy(string[arrCount],str+start,i);			
			start = start + i;
			i = 0;
			arrCount++;

		}
		

	}

	strings[arrCount] = strndup (string + start, strlen(string) - start);

	return;
  
}*/

char* input_to_string()		// rewrite using read!!
{
   char* str = NULL;
   char tbuf[200];
   size_t stringlen = 0, tlen = 0;
   do {
       fgets(tbuf, 200, stdin);
       tlen = strlen(tbuf);
       stringlen += tlen;
       str = realloc(str, stringlen+1);
       strcat(str, tbuf);
    } while (tlen==200-1 && tbuf[200-2]!='\n');
    return str;
}

void input_handler()
{	
	fflush(stdout);
		printf("\n");
	//printf("%s",str);
	printf("$");
	char* str = input_to_string();
	int count=1;

	for(char * iter = str; *iter ;iter++ ) 
	{
		if(*iter==32)
			count +=1;
	}
	char** strings;
	strings = to_arrays(str);


		//to_arrays(strings, str, count);
		//printf("%d \n",count);

//	for(int i = 0; i < count; i++) 
		printf("%s \n",strings[0]);
	//print_string();
 	int exitStatus = execute_command(strings,count);
	/*free(str);


	for(int i = 0; i < count; i++) // make sure it is done in the end not necessarily here
			free(strings[i]);
	*/
	//input_handler();
	return;
}

int main()
{
	printf(" \n Simple Shell successfully initiated\n$");
	
	//char* cmndName;
	//char* args[];
	while(1)
	input_handler();	
	
	//free(test);
	/*char* str = input_to_string();
	int n;
	char subbf[n];
	memcpy(subbf,str,4);
	subbf[4] = '\0';
	printf("\n %s",subbf);*/
	return 0;
}

