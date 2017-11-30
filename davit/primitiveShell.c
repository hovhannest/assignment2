#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BUFFER_LEN 1024

int main(){
    char line[BUFFER_LEN];  
    char* argv[100];        
    char* path= "/bin/";    
    char progpath[20];      
    int argc;               

while(1){

    printf("$> ");                    

        if(!fgets(line, BUFFER_LEN, stdin)){  //get command and put it in line
        break;                                //if user hits CTRL+D break
    }
    if(strcmp(line, "exit")==0){            
        break;
    }

    char *token;                  
    token = strtok(line," ");
    int i=0;
    while(token!=NULL){
        argv[i]=token;      
        token = strtok(NULL," ");
        i++;
    }
    argv[i]=NULL;                     

    argc=i;                           
    for(i=0; i<argc; i++){
        printf("%s\n", argv[i]);      
    }
    strcpy(progpath, path);           
    strcat(progpath, argv[0]);            
    for(i=0; i<strlen(progpath); i++){    

        if(progpath[i]=='\n'){      
            progpath[i]='\0';
        }
    }
    int pid= fork();              //fork child

    if(pid==0){               //Child
        execvp(progpath,argv);
        fprintf(stderr, "Child process could not do execvp\n");

    }else{                    //Parent
        wait(NULL);
        printf("Child exited\n");
    }

}
} 