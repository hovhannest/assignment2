#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>//exit()
#include <sys/wait.h>//wait()
#include <string.h>

char *fgetstr(char *string, int n, FILE *stream)
{
char *result;
result = fgets(string, n, stream);
if(!result)
 return(result);

if(string[strlen(string) - 1] == '\n')
 string[strlen(string) - 1] = 0;
  return(string);
}

char* concat(const char *s1, const char *s2)
{
char *result = malloc(strlen(s1) + strlen(s2) + 1);
strcpy(result, s1);
strcat(result, s2);
 return result; 
}

int main(int ac, char * av[])
{
char str[80] = "ls -a -l &";
const char s[2] = " ";
char *token;
char * prompt = " ";
char * user;
char *hostname;
char *pwd;
char* bgsym = "&";
user=getenv("HOSTNAME");
if (!hostname) hostname="";
concat (user, prompt);
concat ("@", prompt);
concat (hostname, prompt);
concat (":", prompt);
concat (pwd, prompt);
while (1)
{
char* args[5]= {"","","","", 0};
fflush(stdout);
printf ("\n$ ");
fflush(stdout);
fgetstr (str, 70, stdin);
int bg = 0;
int i = 0;
args[0]=strtok(str, s);
i=1;
do {
token = strtok(NULL, s);
args[i] = token;
 printf ("args[%d]=%s\n", i, args[i]);
 i++;
 }
while (token != NULL);
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
if (child < 0)
{
perror("fork() error");
exit(-1);
}
if(child != 0)
{
printf("parent %d, child%d\n", getpid(), child);
 if (bg==1)
 wait(NULL);
 }
else {
printf("child %d, parent%d\n", getpid(),getppid());
printf ("running: \n");
int j = 0;
do {
 if (args[j] != NULL)
printf ("args[%d]=%s\n", j, args[j]);
j++;
}
while (args[j] != NULL);
int res=execvp (args[0], args);
 if (res == -1)
printf("FAILED!\n");
}
return 0;
}
}



