#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>


//this function reads string, the number of strings, change it into C File
char *fgetstr(char *string, int n, FILE *stream)
{
	char *result;

	result = fgets(string, n, stream);

	if(!result)
	{
		return (result);
	}

	if(string[strlen(string) -1] == '\n')
	{
		string[strlen(string) -1] = 0;
	}
	return(string);
}

//this function concatinates 2 strings
char *concat(const char *s1, const char *s2)
{
	char *result = malloc(strlen(s1)+ strlen(s2)+1);
	strcpy(result, s1);
	strcat(result, s2);
	return result;
}
//the is the main function, that tokenizes string and forks it
int main(int ac, char *av[]){

	//char array with 100 length
	char str[100] = "ls -a -l &";
	//it divide "str" by space while tokenize
	const char s[2] = " ";
	//char token
	char *token;
	// $ sign of the beginning
	char *prompt = " ";
	// username
	char *user;
	// hostname
	char *hostname;
	// path extension
	char *pwd;
	//& sign
	char *bgsym = "&";

//getenv function, it makes the user to be "USER"
	user = getenv("USER");
	if(!user) user = "";

//if there is no user just leave it empty
	pwd = getenv("PWD");
	if (!pwd) pwd = "";

//getenv function which makes the hostname "HOSTNAME"
	hostname = getenv("HOSTNAME");
	if (!hostname) hostname ="";
	concat (user, prompt);
	concat ("@", prompt);
	concat (hostname, prompt);
	concat (":", prompt);
	concat (pwd, prompt);

while (1)
{
	// char {"user", "hostname", "pwd", "prompt"}
	char* args[5] = {"", "", "", "", 0};
	printf ("\n$ ");
	fgetstr (str, 70, stdin);
	// beginning
	int bg = 0;
	int i = 0;
	//it tokenizes the str(ls) to be the first of the args
	args[0] = strtok(str, s);
	i = 1;

	//print function of argument index and its content
	do 
	{
		token = strtok(NULL, s);
		args[i] = token;
		printf ("args[%d]=%s\n", i, args[i]);
		i++;
	}
	while (token !=NULL);
	printf ("i=%d\n", i);
	printf ("s\n", args[i-2]);
	printf ("%s\n", bgsym);
	int sint = strcmp(args[i-2], bgsym);
	
	if(!sint)
	{
		args[i-2] =NULL;
		bg =1;
	}

	pid_t child = fork();

//if child is less than zero it generates error
	if(child <0){
	perror("fork() error");
	exit(-1);
}

//if child is not zero print parent id and child id
	if(child !=0){
	printf("parent %d, child%d\n", getpid(), child);

//wait until get argument fromstdin
	if(bg==1)
	wait(NULL);
	}
	
	else{
//print the child id and the parent id
	printf("child %d, parent%d\n", getpid(), getppid());
	int j = 0;
	
//the execution of commands
		do{
			if (args[j]!=NULL) printf ("args[%d]=%s\n", j, args[j]);
			j++;
		}
		
		while (args[j] !=NULL);
			int res=execvp (args[0], args);

//if there is no result print error
		if(res ==-1)
		printf("Error!\n");
		}
	}
return 0;

