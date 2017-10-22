/*
 ============================================================================
 Name        : shell.c
 Author      : Hovhannes Hsakanyan
 Version     :
 Copyright   : OS, AUA 2017
 Description : shell in C, Ansi-style
 ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <string.h>

pid_t myFork();
int tokenLen(const char *token);
char **mytoc(char *str, char delim);
int numberOfTokens(char *tokens, char delim);
char *concat(char *str,char *cmd,int lenght, int cmdLenght);
int exitShell(char *input);


void my_handler(int s){
	//printf("Caught signal %d\n",s);
	//exit(0);

	// ignore ctrl+c for this shell. Close child, but keep shell running
}

int main(int argc, char **argv, char** envp) {
	struct sigaction sigIntHandler;

	sigIntHandler.sa_handler = my_handler;
	sigemptyset(&sigIntHandler.sa_mask);
	sigIntHandler.sa_flags = 0;

	sigaction(SIGINT, &sigIntHandler, NULL);

	int exitshell = 1;
	while(exitshell){
		char *inp = (char*)malloc(100);
		write(1,"$",1);
		int bytesread = read(0,inp,100);
		if(bytesread == 0) exit(0);
		int l = strlen(inp);
		if(l <= 0 || l >= 100)
			continue;
		exitshell = exitShell(inp);
		char **input = mytoc(inp,'\n');
		char **command;

		int commandExecuted = 0;

		if (input[0][0] == 'c' && input[0][1] == 'd'){
			int lastToken = numberOfTokens(input[0],' ');
			if(lastToken > 1){
				char** cmd = mytoc(input[0],' ');
				if(!strcmp(cmd[0], "cd")){
					char* to = cmd[1];
					chdir(to);
					commandExecuted = 1;
				}
			}
		}

		int child = myFork();

		if(child == 0){
			if(input[0][0] == '/'){ //check if a path has been given
				int lastToken = numberOfTokens(input[0],'/');//get position of command on given path
				command = mytoc(input[0],'/');//tokenize the given path to get the command to be executed
				char **cmd = mytoc(command[lastToken-1],' '); //create a vector of pointers containing the command

				struct stat sb;
				int found = stat(input[0],&sb);//check if command exists on path
				if(found == 0){//if command exists, try executing it.
					commandExecuted = 1;
					int retval = execve(input[0],cmd,envp);
					fprintf(stderr, "%s: exect returned %d\n",command[0],retval);
				}
			}
			command = mytoc(input[0],' ');

			char *pathenv;

			for(int i = 0; envp[i] != '\0'; i++){ //look for PATH environment
				char **environment = mytoc(envp[i],'=');

				if(strcmp(environment[0],"PATH") == 0){//check if PATH has been found
					pathenv = environment[1];
				}
			}

			char **listOfPaths = mytoc(pathenv,':');
			int found;

			int cmdLength = tokenLen(command[0]);

			for(int i = 0; listOfPaths[i] != '\0'; i++){
				int pathLength = tokenLen(listOfPaths[i]);
				char *completePath = concat(listOfPaths[i],command[0],pathLength,cmdLength);
				struct stat sb;

				found = stat(completePath,&sb);//check if command exists on path

				if(found == 0){//if command exists, try executing it.
					commandExecuted = 1;
					int retval = execve(completePath,command,envp);
					fprintf(stderr, "%s: exect returned %d\n",command[0],retval);
				}
			}
		}
		int status;

		waitpid(child,&status,0);

		if(child == 0){
			if(exitshell && commandExecuted == 0)
			{
				fprintf(stdout, "Command not found\n");
			}
			exit(1);
		}
	}

	return EXIT_SUCCESS;
}

//Concatenate the command entered to its corresponding path in case a path was not given
char *concat(char *str,char *cmd,int lenght, int cmdLenght){
	int totalLenght = lenght+cmdLenght;
	char *tmp = (char*)malloc(totalLenght+1);
	char *tempStr = tmp; //char destination d store in temp

	while (*str){ //while there is stuff in source copy it to the destination
		*tmp++ = *str++;
	}
	*tmp++ = '/';

	while (*cmd){ //while there is stuff in source copy it to the destination
		*tmp++ = *cmd++;
	}

	*tmp++ = '\0';//store null character
	return tempStr;
}

//exitShell function checks if "exit" has been entered by user in order to terminate shell
int exitShell(char *input){
	char * exit = "exit";
	int terminate = 1;
	for(int i = 0; input[i] != '\0' && exit[i] != '\0'; i++){
		if(input[i] == exit[i]){
			terminate = 0;
		}
		else{
			terminate = 1;
			break;
		}
	}
	return terminate;
}

pid_t myFork()
{
	//fprintf(stderr, "process <%d> calling fork() ...\n", getpid());
	return fork();
}
