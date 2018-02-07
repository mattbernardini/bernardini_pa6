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

static int FAN_COUNT = 0;
static int MAX_COMMAND_SIZE = 256;

void handleOpts(int argc, char ** argv);
int * fanProcesses();
void handleWaits (int * childPids, char ** argv);

int main (int argc, char *argv[]) 
{
    int * childPids;
    handleOpts(argc, argv);
    childPids = fanProcesses();
    handleWaits(childPids, argv);
    return 0;
}

void handleOpts(int argc, char ** argv) 
{
    int ch;
    if (argc != 2){ 
        fprintf(stderr, "%s: Usage: -n [number of processes]\n", argv[0]);
        exit(-1);
    } 
    else 
    {
        while ((ch = getopt(argc, argv, "n::h")) != -1) 
        {
            switch (ch)
            {
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

int * fanProcesses() 
{
    
    char commandBuffer[MAX_COMMAND_SIZE];
    int * childPids, counter = 0;
    childPids = (int *) malloc(sizeof(int) * FAN_COUNT);
    memset(childPids, 0, (size_t) FAN_COUNT);
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
      	    char formattedCommand[MAX_COMMAND_SIZE];
            strcpy(formattedCommand, "./");
            char *commandList = strtok(commandBuffer, " ");
            strcat(formattedCommand, commandList);
            // Get lengths of time
            char *length1 = strtok(NULL, " ");
            char *length2 = strtok(NULL, " ");
            // Execute
            execl(formattedCommand, formattedCommand, length1, length2, (char*) NULL);
            exit(42);
        }
        else 
        {
            counter += 1;
        }
    }
    return childPids;
}
void handleWaits (int * childPids, char ** argv)
{
    pid_t cpid;
    int numberOfPids = sizeof(childPids)/sizeof(childPids[0]);
    for (int i = 0; i < numberOfPids; i++) 
    {
        cpid = waitpid(childPids[i], &status, WUNTRACED | WCONTINUED);
        
        if (cpid == -1) {
            perror("waitpid");
            exit(EXIT_FAILURE);
        }
        if (WIFEXITED(status)) {
            fprintf(stdout, "%s: Child Pid %d: exited, status=%d\n", argv[0], childPids[i], status);
        } else if (WIFSIGNALED(status)) {
            fprintf(stderr, "%s: Error: Child Pid %d: killed by signal %d\n", argv[0], childPids[i], WTERMSIG(status));
        } else if (WIFSTOPPED(status)) {
            fprintf(stderr, "%s: Error: Child Pid %d: stopped by signal %d\n",argv[0], childPids[i], WSTOPSIG(status));
        } else if (WIFCONTINUED(status)) {
            fprintf(stdout, "%s: Child Pid %d: continued\n", argv[0], childPids[i]);
        }
    }
}
