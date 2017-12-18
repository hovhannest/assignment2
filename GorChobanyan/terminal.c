#include <stdlib.h>
#include <string.h>
#include  <stdio.h>
#include  <sys/types.h>
#include <unistd.h>
#include <sys/syscall.h>
#include <sys/wait.h>

int main(int argc, char *argv[]) {

while (1) {
  char str[80];
  int i;
  printf("%s", "$ ");

  while (fgets(str, sizeof(str), stdin) != NULL) {
  str[strcspn(str, "\n")] = '\0';  // strip the newline if present
    if (!strcmp(str, "exit")){
        printf("%s", "Exiting");
        return 0;
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
  perror("Failed");
  } else if (pid == 0) {
  if (background) {
      printf("%s %d \n", "Killed ", pid);
  }
  int err = execvp(cmd, ineerArgv);

  if (err) {
      printf("%d ", err);
  }
  } else {
    if (background) {
        wpid = wait(&status);
        printf("Waiting ");
    } else {
        wait(NULL);
    }
  }
}
}
