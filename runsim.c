#define _XOPEN_SOURCE
#define MAX_COMMAND_SIZE 256

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/wait.h>
#include <string.h>
int main (int argc, char *argv[]) {

  // Check for valid number of command-line arguments
  if (argc != 2){ 
    fprintf(stderr, "Usage: %s processes\n", argv[0]);
    return 1;
  }

  // Declare variables to be used in the rest of the program
  pid_t cpid;
  int shmId,i,pr_limit,*pr_current, childpid;
  key_t key;
  char command[MAX_COMMAND_SIZE];

  // Allocate shared memory to store the number of running processes
  shmId = shmget(key, sizeof(int), IPC_CREAT | 0666);
  pr_current = (int *) shmat( shmId, NULL, 0);
  *pr_current = 0;

  // Set the number of concurrent processes to let run
  pr_limit = atoi(argv[1]);

  while (fgets(command, MAX_COMMAND_SIZE, stdin) != NULL) {
    char **command;
    int i = 0;
    split=strtok(command, " ");
    while (split != NULL){
      command[i++] = split;
      split=strtok(NULL, " ");
    }
    // Handle error
    
    childpid = fork();
    if (childpid != 0) {
      *pr_current += 1;

      // Parent


    } else {
      // Child process
      printf("%d\n", *pr_current);
      execl("./testsim", "./testsim", "3", "1", (char*) NULL);

    }
    if (*pr_current >= pr_limit & childpid != 0) {
        cpid = wait(NULL);
      }
  }

  fprintf(stderr, "i:%d process ID:%ld parent ID:%ld child ID:%ld\n",
                        i, getpid(), getppid(), childpid);                      
  return 0;
}
/*
#define _XOPEN_SOURCE
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include<sys/wait.h>

int main() {
  pid_t cpid;
  int shmId,i;
  key_t key;
  int *shmPtr;
  const int SIZE = 10;
  (shmId = shmget(key, sizeof(int), IPC_CREAT | 0666));
  shmPtr = (int *) shmat( shmId, NULL, 0);
  *shmPtr = 10;
  int pid = fork();
  if (pid == 0) {
    *shmPtr = 1;
    exit(0);
  }
  cpid= wait(NULL);
  printf("%d\n", *shmPtr);
  shmdt(shmPtr);
  shmctl(shmId,IPC_RMID,NULL);
  return 0;
}
*/