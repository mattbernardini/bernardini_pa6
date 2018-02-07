/**
 * Matt Bernardini
 * cs4760 HW1
 * 2/08/2017
 * 
 * runsim.c
*/

#define _XOPEN_SOURCE
#ifndef STDIO_H
#define STDIO_H
#include <stdio.h>
#endif

#ifndef UNISTD_H
#define UNISTD_H
#include <unistd.h>
#endif

#ifndef ERRNO_H
#define ERRNO_H
#include <errno.h>
#endif

#ifndef STDLIB_H
#define STDLIB_H
#include <stdlib.h>
#endif

#ifndef TYPES_H
#define TYPES_H
#include <sys/types.h>
#endif

#ifndef IPC_H
#define IPC_H
#include <sys/ipc.h>
#endif

#ifndef SHM_H
#define SHM_H
#include <sys/shm.h>
#endif

#ifndef WAIT_H
#define WAIT_H
#include <sys/wait.h>
#endif

#ifndef STRING_H
#define STRING_H
#include <string.h>
#endif

#ifndef STDDEF_H
#define STDDEF_H
#include <stddef.h>
#endif

#ifndef GETOPT_H
#define GETOPT_H
#include <getopt.h>
#endif

#ifndef MAKEARGV_H
#define MAKEARGV_H
#include "makeargv.h"
#endif

static int FAN_COUNT = 0;
static int MAX_COMMAND_SIZE = 256;

void handleOpts(int argc, char ** argv);
void fanProcesses();

int main (int argc, char *argv[]) {
  // Declare variables to be used in the rest of the program
  pid_t cpid;
  int shmId,*pr_current, childpid, status;
  key_t key = 0;
  char command[MAX_COMMAND_SIZE];
  // Allocate shared memory to store the number of running processes
  shmId = shmget(key, sizeof(int), IPC_CREAT | 0666);
  pr_current = (int *) shmat( shmId, NULL, 0);
  *pr_current = 0;
  // Check for valid number of command-line arguments
  handleOpts(argc, argv);
  fanProcesses();
  exit(0);
  // Main loop
  while (fgets(command, MAX_COMMAND_SIZE, stdin) != NULL) {
    // Handle error
    printf("%s", command);
    childpid = fork();
    if (childpid != 0) {
      // In the parent process
      *pr_current += 1;
    } else {
      // Child process
      // Format command
      char formattedCommand[MAX_COMMAND_SIZE];
      memset(formattedCommand, MAX_COMMAND_SIZE, '\0');
      strcpy(formattedCommand, "./");
      char *commandList = strtok(command, " ");
      strcat(formattedCommand, commandList);
      // Get lengths of time
      char *length1 = strtok(NULL, " ");
      char *length2 = strtok(NULL, " ");
      // Execute
      execl(formattedCommand, formattedCommand, length1, length2, (char*) NULL);
      exit(42);
    }
    // Check to make sure we do not have more than enough running processes
    if (*pr_current >= FAN_COUNT & childpid != 0) {
        cpid = waitpid(-1, &status, WUNTRACED | WCONTINUED);
        *pr_current -= 1;
        if (cpid == -1) {
          perror("waitpid");
          exit(EXIT_FAILURE);
        }
        if (WIFEXITED(status)) {
          printf("exited, status=%d\n", WEXITSTATUS(status));
        } else if (WIFSIGNALED(status)) {
          printf("killed by signal %d\n", WTERMSIG(status));
        } else if (WIFSTOPPED(status)) {
          printf("stopped by signal %d\n", WSTOPSIG(status));
        } else if (WIFCONTINUED(status)) {
          printf("continued\n");
        }
      }
  }
  cpid = waitpid(-1, &status, WUNTRACED | WCONTINUED);
  *pr_current -= 1;
  if (cpid == -1) {
    perror("waitpid");
    exit(EXIT_FAILURE);
  }
  if (WIFEXITED(status)) {
    printf("exited, status=%d\n", status);
  } else if (WIFSIGNALED(status)) {
    printf("killed by signal %d\n", WTERMSIG(status));
  } else if (WIFSTOPPED(status)) {
    printf("stopped by signal %d\n", WSTOPSIG(status));
  } else if (WIFCONTINUED(status)) {
    printf("continued\n");
  }         
  return 0;
}

void handleOpts(int argc, char ** argv) {
    int ch;
    if (argc != 2){ 
    fprintf(stderr, "%s: Usage: -n [number of processes]\n", argv[0]);
    exit(-1);
  } else {
    while ((ch = getopt(argc, argv, "n::h")) != -1) {
      switch (ch){
        case 'n':
          FAN_COUNT = atoi(optarg);
          break;
        case 'h':
          fprintf(stdout, "%s: Usage: -n [number of processes]\n", argv[0]);
          exit(0);
          break;
        case '?':
        default:
          perror((const char *)(long)errno);
          break;
      }
    }
  }
}
void fanProcesses() 
{
    
    char commandBuffer[MAX_COMMAND_SIZE];
    memset(commandBuffer, MAX_COMMAND_SIZE, '\0');
    
    int childPids[FAN_COUNT], counter = 0;
    memset(childPids, FAN_COUNT, 0);
    
    while(fgets(commandBuffer, MAX_COMMAND_SIZE, stdin) != NULL) 
    {
        childPids[counter] = fork();
        if (childPids[counter] == -1) 
        {
            perror((const char *)(long)errno);
        }
        else if (childPids[counter] == 0)
        {
            // Child process
            char ** commands;
            int numtokens;
            if ((numtokens = makeargv(commandBuffer, " ", &commands)) == -1) 
            {
                fprintf(stderr,
                        "Failed to make command line arguments array for %s\n",
                        commandBuffer);
            }
            else 
            {
                // Valid arguments
                for (int i = 0; i < numtokens; i++)
                {
                    fprintf(stdout, "%s\n", commands[i]);
                }
            }
        }
    }
}
