#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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
		{
			printf("So long, and thanks for all the fish\n");
			return -5;
		}
	int exitStatus = execute_command(strings,count);
	
	//input_handler();
	return;
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