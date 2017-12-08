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

  // Set the number of concurrent processes to let run
  pr_limit = atoi(argv[1]);

  // Main loop
  while (fgets(command, MAX_COMMAND_SIZE, stdin) != NULL) {
    // Handle error

    childpid = fork();
    if (childpid != 0) {
      // In the parent process
      *pr_current += 1;
    } else {
      // Child process
      // Format command
      char formattedCommand[MAX_COMMAND_SIZE];
      strcpy(formattedCommand, "./");
      char *commandList = strtok(command, " ");
      strcat(formattedCommand, commandList);
      // Get lengths of time
      char *length1 = strtok(NULL, " ");
      char *length2 = strtok(NULL, " ");
      // Execute
      printf("%d\n", pr_current);
      execl(formattedCommand, formattedCommand, length1, length2, (char*) NULL);
      exit(42);
      *pr_current -= 1;
    }
    // Check to make sure we do not have more than enough running processes
    if (*pr_current >= pr_limit & childpid != 0) {
        cpid = wait(NULL);
      }
  }               
  return 0;
}