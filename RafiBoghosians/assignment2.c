#include  <stdio.h>
#include  <string.h>
#include  <stdlib.h>
#include  <unistd.h>
#include  <sys/types.h>
#include  <sys/wait.h>


//funtion reads characters from stream and stores them as a C string
char *fgetstr(char *string, int n, FILE *stream)
{
	char *result;

	result = fgets(string, n, stream);

	if(!result)
	{
		return(result);
	}
				
	if(string[strlen(string) - 1] == '\n')
	{
		string[strlen(string) - 1] = 0;
	}
	return(string);
}

//function that concatinate two strings
char* concat(const char *s1, const char *s2)
{
    char *result = malloc(strlen(s1)+strlen(s2)+1);
    strcpy(result, s1);
    strcat(result, s2);
    return result;
}

//tha main function that tokenize the string and it forks, wait and execute it
int main(int ac, char * av[]){

//100 char string
char str[100] = "ls -a -l &";
	//during tokenization it separates "str" by spasce
	const char s[2] = " ";
	//it takes the tokens  
	char *token;
  	//The dollar sign of the beginning
	char * prompt = " ";
	//The username  
	char * user;
	//The hostname
	char *hostname;
  	//the path extention
	char *pwd;
  	//& sign
	char* bgsym = "&"; 
  	
	//getenv function which makes the user to be "USER"
	 user=getenv("USER");
  	
	//if there is no user just leave it empty
	 if (!user)	
	 user="";
	//getenv function which makes the pwd to be "PWD"
  	 pwd=getenv("PWD");
 	//if there is no path makes it empty 
	 if (!pwd)
	 pwd="";
	//getenv function which makes the hostname "HOSTNAME"
  	 hostname=getenv("HOSTNAME");
  
	 if (!hostname) hostname="";
  		 concat (user, prompt);
  		 concat ("@", prompt);
  		 concat (hostname, prompt);
                 concat (":", prompt);
  		 concat (pwd, prompt);
 
  while (1)
   {
	 //{"user","hostname", "pwd", "prompt"}
	 char* args[5]= {"", "", "", "", 0};
  	 printf ("\n$ ");
  	 fgetstr (str, 70, stdin);
         //the beginning
	 int bg = 0 ;
  	 int i = 0; 
	 //It tokenizes the str(ls) to be the first index of the args
  	 args[0]=strtok(str, s);
  	 i=1;
  
	 //Prints the argument index and its content
	 do
  	 {
  		 token = strtok(NULL, s);
  		 args[i] = token;
      			 printf ("args[%d]=%s\n", i, args[i]);
  		 i++;
  	 } while (token != NULL);
  		 printf ("i=%d\n", i);
  		 printf ("%s\n", args[i-2]);
  		 printf ("%s\n", bgsym);
  		 int sint = strcmp(args[i-2], bgsym);
  	 if (!sint)
  	 {
    		 args[i-2]=NULL;
    		 bg = 1;
  	 }

	
    	 pid_t child = fork();
         //if child is less than zero then gives us an error
	 if (child < 0){
  	 perror("fork() error");
  	 exit(-1);
 	 }
	
	 //if child is not zero print parent id and child id
 	 if(child !=0){
   	 printf("parent %d,child%d\n",getpid(),child);
   	 //wait until get argument from stdin  
	 if (bg==1)
    	 wait(NULL);
 	 }else{
	 //print the child and the parent
  	 printf("child %d,parent%d\n",getpid(),getppid());
  	 
	 int j = 0;
         
	//The execution of commands 
         do
  	 {
      		 if (args[j]!= NULL) printf ("args[%d]=%s\n", j, args[j]);
  		 j++;
  	 } while (args[j] != NULL);
		 int res=execvp (args[0], args);
  	
	//If there is no result print Error!
	 if (res==-1)
		 printf ("Error!\n");  
  	 }

	 }


return 0;
}
