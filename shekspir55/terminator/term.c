#include <stdlib.h>
#include <string.h>
#include  <stdio.h>
#include  <sys/types.h>
#include <unistd.h>
#include <sys/syscall.h>
#include <sys/wait.h>

#define KRED  "\x1B[31m"
#define KWHT  "\x1B[37m"
const int maxLength = 100;

void err(char *text, int i) {
    printf("%s Error: %s%d\n", KRED, text, i);
    printf("%s", KWHT);
}

int main(int argc, char *argv[]) {
    printf("%s\n", "WELCOME TO TERMINATOR");
    printf("%s\n", "Ready to termiante");
    while (1) {
        char str[80];
        int i;
        printf("%s", "TERMINATOR $ ");

        while (fgets(str, sizeof(str), stdin) != NULL) {
            str[strcspn(str, "\n")] = '\0';  // strip the newline if present
            if (!strcmp(str, "exit")){
                printf("%s", "Goodbye my love, I will always remember you...");
                return;
            }
        }


        int status = 0;

        char *cmd;
        char *ineerArgv[5000];
        int innerArgc = 0;
        char *pch;
        pch = strtok(str, " ");

        while (pch != NULL) {
            if (innerArgc == 0) {
                cmd = pch;
            }
            ineerArgv[innerArgc++] = pch;
            pch = strtok(NULL, " ");
        }

        int background = 0;

        if (!strcmp(ineerArgv[innerArgc - 1], "&")) {
            ineerArgv[innerArgc - 1] = NULL;
            background = 1;
        }

        pid_t pid, wpid;
        if ((pid = fork()) < 0) {
            perror("Fork failed");
        } else if (pid == 0) { // <- Both processes continue executing here
            //Child process
            if (background) {
                printf("%s %d \n", "kill me at ", pid);
            }
            int errorio = execvp(cmd, ineerArgv);
            if (errorio) {
                err("", errorio);
            }
        } else {
            /**
             * Something is wrong with wait :( couldn't figure it out at last
             */
            if (!background) {
                //Parent process
                wpid = wait(&status);
            } else {
                wait(NULL);
            }
        }

    }
}
