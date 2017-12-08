#define _XOPEN_SOURCE
#define MAX_COMMAND_SIZE 256

#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/wait.h>
#include <string.h>
#include <stddef.h>
int main (int argc, char *argv[]) {

  // Check for valid number of command-line arguments
  if (argc != 2){ 
    fprintf(stderr, "Usage: %s processes\n", argv[0]);
    return 1;
  }

  // Declare variables to be used in the rest of the program
  pid_t cpid;
  int shmId,i,pr_limit,*pr_current, childpid, status;
  key_t key;
  char command[MAX_COMMAND_SIZE];

  // Allocate shared memory to store the number of running processes
  shmId = shmget(key, sizeof(int), IPC_CREAT | 0666);
  pr_current = (int *) shmat( shmId, NULL, 0);
  *pr_current = 0;
  char *commandList = strsep(command, " ");
  printf("%s\n", commandList);
  // Set the number of concurrent processes to let run
  pr_limit = atoi(argv[1]);

  while (fgets(command, MAX_COMMAND_SIZE, stdin) != NULL) {
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