#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <memory.h>

char* join(char* first, char*second);		// retursn result consisted of joined  strings (i.e char* s)
int execute_command( char* args[], int count);		 // takes array of strings with the input and tries to execute as command. also handles with forking/bg&fg
char** to_arrays( char* str );				// takes string and devides into tokens
char* input_to_string();					// makes input to string
int input_handler();						// handles input and runs execute_command

int main()		 // main with sophisticated functionality. Maybe in the future it will take more on himself.
{
	printf(" \n Simple Shell successfully initiated. Enter breakFree to exit.\n$");
	
	while( input_handler() != -5);	
	
	return 0;
}

