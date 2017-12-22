#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>


int new_cd(char **args);
int new_help(char **args);
int new_exit(char **args);


char *builtin_comm[] = {
  "cd",
  "help",
  "exit"
};

int (*builtin_func[]) (char **) = {
  &new_cd,
  &new_help,
  &new_exit
};

int num_builtins() {
  return sizeof(builtin_comm) / sizeof(char *);
}


int new_cd(char **args)
{
  if (args[1] == NULL) {
    fprintf(stderr, "<3: Too few arguments to  \"cd\"\n");
  } else {
    if (chdir(args[1]) != 0) {
      perror("<3");
    }
  }
  return 1;
}


int new_help(char **args)
{
  int i;

  printf("Here are the commands that you can use:\n");

  for (i = 0; i < num_builtins(); i++) {
    printf("  %s\n", builtin_comm[i]);
  }

  return 1;
}


int new_exit(char **args)
{
  return 0;
}


int launch(char **args)
{
  
    int status;

    pid_t child = fork();

    if(child == 0)
    {
      if (execvp(args[0], args) == -1) {
       perror("<3");
    }
    exit(EXIT_FAILURE);
    }
    else 
    {
      do {
          waitpid(child, &status, WUNTRACED);
        } while (!WIFEXITED(status) && !WIFSIGNALED(status));
    }

  return 1;
}


int execute(char **args)
{
  int i;

  if (args[0] == NULL) {

    return 1;
  }

  for (i = 0; i < num_builtins(); i++) {
  	if (strcmp(args[0], "cd") == 0)
  	{
  		return (new_cd(args));
  	}
  	else if (strcmp(args[0], "help") == 0)
  	{
  		return (new_help(args));
  	}
  	else if (strcmp(args[0], "exit") == 0)
  	{
  		return (new_exit(args));
  	}
    
  }

  return launch(args);
}



char *read_line(void)
{
  char *line = NULL;
  ssize_t buffersize = 0;
  getline(&line, &buffersize, stdin);
  return line;
}


char **split_line(char *line)
{
  int bufsize = 64, position = 0;
  char **tokens = malloc(bufsize * sizeof(char*));
  char *token;

 
  token = strtok(line, " \t\r\n\a");
  while (token != NULL) {
    tokens[position] = token;
    position++;

    token = strtok(NULL, " \t\r\n\a");
  }
  tokens[position] = NULL;
  return tokens;
}


void _main(void)
{
  char *line;
  char **args;
  int status;

  printf("\e[H\e[2J");
 
  printf("Welcome to myShell <3: To see your options please type 'help', if you wish to exit the shell please type 'exit'.\n");

  do {
    printf("<3  ");
    line = read_line();
    args = split_line(line);
    status = execute(args);

   
  } while (status);
}


int main(int argc, char **argv)
{

  _main();

  return EXIT_SUCCESS;
}