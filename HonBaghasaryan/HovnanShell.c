#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdlib.h>



char *lsh_read_line(void)
{
  char *line = NULL;
  ssize_t bufsize = 0; // have getline allocate a buffer for us
  getline(&line, &bufsize, stdin);
  return line;
}

#define LSH_TOK_BUFSIZE 64
#define LSH_TOK_DELIM " \t\r\n\a"
char **lsh_split_line(char *line)
{
  int bufsize = LSH_TOK_BUFSIZE, position = 0;
  char **tokens = malloc(bufsize * sizeof(char*));
  char *token;

  if (!tokens) {
    fprintf(stderr, "lsh: allocation error\n");
    exit(EXIT_FAILURE);
  }

  token = strtok(line, LSH_TOK_DELIM);
  while (token != NULL) {
    tokens[position] = token;
    position++;

    if (position >= bufsize) {
      bufsize += LSH_TOK_BUFSIZE;
      tokens = realloc(tokens, bufsize * sizeof(char*));
      if (!tokens) {
        fprintf(stderr, "lsh: allocation error\n");
        exit(EXIT_FAILURE);
      }
    }

    token = strtok(NULL, LSH_TOK_DELIM);
  }
  tokens[position] = NULL;
  return tokens;
}

int runInBackground(char **args){
	for(int i=0; i<sizeof(args); i++){
		if(!args[i]){
			if(!strcmp(args[i-1],"&")){
				return i-1;
			}
			break;
		}
	}
	return -1;
}

int main()
{	
  char *line;
  char *token;

  char **args;


  while(1)
  {

	printf("\n$ ");
	line = lsh_read_line();
	args = lsh_split_line(line);

	int bg = 0;
	int rb = runInBackground(args);
	if(rb != -1){
		bg = 1;
		args[rb] = NULL;
	}


	fflush(NULL);
	pid_t pid;
	pid = fork();
	
	if(pid < 0)
	{
		printf("Forking process failed\n");
	}
	if(pid != 0)
	{
           printf("this is the parent process with id %d\n", (int)getpid());
	   if(bg)
	   {
	   	printf("Waiting");
		wait(NULL);
           }

	}
	else
	{
	   printf("this is the child process with id %d\n", (int)getpid());
	   if(execvp(args[0], args) == -1)
		printf("execvp FAILED");
	   
	   execvp(args[0], args);
	}
  }
  return 0;
}
