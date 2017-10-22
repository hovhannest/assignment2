#include <unistd.h>
#include <sys/wait.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <memory.h>

char* concatenate(char*, char*);
int executeCommand(char** argv);
char* readLine();
char** tokenizeLine(char* line);

int main(int argc, char* argv[])
{

	printf("A COMPLETELY DIFFERENT SHELL\n");
	printf("(kidding it's an ordinary one)\n\n");
	char* shellLine;
	char** lineWords;
	int commandStatus;

	do {
		printf("SHELL_LINE = ");
		shellLine = readLine();
		lineWords = tokenizeLine(shellLine);
		printf("EXECUTING COMMAND...\n");
		commandStatus = executeCommand(lineWords);
		free(shellLine);
		free(lineWords);
	} while (commandStatus);

	return EXIT_SUCCESS;
}

char* concatenate(char* str1, char* str2) 
{
	char* str3 = (char*) malloc(1 + strlen(str1) + strlen(str2));
	strcpy(str3, str1);
	strcat(str3, str2);
	return str3;
}

int executeCommand(char** argv) 
{
	pid_t pid, wpid;
	int status;
	char* path = getenv("PATH");
	
	int c = 0;
	while(argv[c] != NULL) c++;
	
	char* and = "&";
	int isBG = *argv[c-1] == *and;
	if(isBG) {
		argv[c-1] = NULL;
	}
	int executed = 0;
	pid = fork();
	if(pid == 0) {
		char* directoryName = strtok(path, ":");
	
		while(directoryName != NULL) {
			char* commandName = 
				concatenate(concatenate(directoryName, "/"),
				argv[0]);		
			int result = execvp(commandName, argv);
			free(commandName);
			if(result != -1){
				executed = 1;
				break;
			}
			directoryName = strtok(NULL, ":");
		}
		if (executed == 0) {
			char* currentDirectory = getenv("PWD");
			char* commandName = concatenate(
				concatenate(currentDirectory,
				"/"), argv[0]);
			int result = execvp(commandName, argv);
			if (result != -1) {
				executed = 1;
			}
			free(commandName);
		}

		if (executed == 0) {
			perror("Command not found.");
		}
		exit(EXIT_FAILURE);
	} else 	if(pid < 0) {
		perror("Fork failed ");
	} else { 
		if(!isBG) {
			do {
				wpid = waitpid(pid, &status, WUNTRACED);
			} while(!WIFEXITED(status) && !WIFSIGNALED(status));
		} else {
			wait(NULL);
		}
	}
		
	return 1;
}

char* readLine()
{
	char* line = NULL;
	ssize_t buffsize = 0;
	getline(&line, &buffsize, stdin);
	return line;
}

char** tokenizeLine(char* line)
{
	const int BUFF_SIZE = 128;
	int buff_size = BUFF_SIZE;
	char** tokenizedLine = malloc(BUFF_SIZE * sizeof(char*));
	char* currentWord = strtok(line, " \t\r\n\a");
	int count = 0;
	while(currentWord != NULL) {
		tokenizedLine[count++] = currentWord;
		if(count >= BUFF_SIZE) {
			buff_size += BUFF_SIZE;
			tokenizedLine = realloc(tokenizedLine, 
				BUFF_SIZE * sizeof(char *)); 
		}
		currentWord = strtok(NULL, " \t\r\n\a");
	}
	tokenizedLine[count] = NULL;
	return tokenizedLine;
}
