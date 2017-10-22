#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <memory.h>

int execute_command( char* args[], int count);

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


char** to_arrays( char* str )						// Tokenizing function. 
{
	const int CHUNK = 256;
	int buff_size = CHUNK;						// We define chunks and if the chanks are not enough we add more tu buff_size

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
	return strings;								// returns an array of strings with cmnd Name as first argument.
}
/*
void to_arrays( char* strings[], char* string , size_t size )	// my initial tokenizer. After spending ours on lookinf for a bug I found out that it 
																//was the main source of an error. SOmething in the strings is broken
																// I suspect its strndup even though in internet it was said it takes care of memory allocation
																// and null termination it is probably doesn't. If there is time I will fix and go back to it.
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

char* input_to_string()		// makes a string from the input. since fgets is not the best option it uses it. Maybe I will rewrite it using read.
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

int input_handler()		// handles input. Also runs the execute commands. 
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
	if(strcmp(strings[0],"breakFree")==0)
		return -5;
	int exitStatus = execute_command(strings,count);
	
	//input_handler();
	return;
}

int main()		
{
	printf(" \n Simple Shell successfully initiated. Enter breakFree to exit.\n$");
	
	//char* cmndName;
	//char* args[];
	while( input_handler() != -5);	
	
	//free(test);
	/*char* str = input_to_string();
	int n;
	char subbf[n];
	memcpy(subbf,str,4);
	subbf[4] = '\0';
	printf("\n %s",subbf);*/
	return 0;
}

