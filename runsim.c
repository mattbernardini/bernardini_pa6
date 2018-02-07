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

#ifndef CTYPES_H
#define CTYPES_H
#include <ctype.h>
#endif

static int FAN_COUNT = -1;
static int MAX_COMMAND_SIZE = 256;

void handleOpts(int argc, char ** argv);
int * fanProcesses(char ** argv);
void handleWaits (int * childPids, char ** argv);

int main (int argc, char *argv[]) 
{
    int * childPids;
    handleOpts(argc, argv);
    childPids = fanProcesses(argv);
    handleWaits(childPids, argv);
    free(childPids);
    return 0;
}

void handleOpts(int argc, char ** argv) 
{
    int ch;
    if (argc < 3) {        
        fprintf(stderr, "%s: Usage: -n [number of processes] -h recieve this message\n", argv[0]);
        exit(-1);
    }
    while ((ch = getopt(argc, argv, "n:h")) != -1) 
    {
        switch (ch)
        {
            case 'n':
                FAN_COUNT = atoi(optarg);
                break;
            case 'h':
                fprintf(stderr, "%s: Usage: -n [number of processes] -h recieve this message\n", argv[0]);
                exit(0);
                break;
            case '?':
                if (optopt == 'n')
                {
                    fprintf(stderr, "Option -%c requires an argument [number of processes].\n", optopt);
                    fprintf(stderr, "%s: Usage: -n [number of processes] -h recieve this message\n", argv[0]);
                }
                else if (isprint(optopt))
                {
                    fprintf(stderr, "Unknown option '-%c'.\n", optopt);
                    fprintf(stderr, "%s: Usage: -n [number of processes] -h recieve this message\n", argv[0]);
                }
                else
                {
                    fprintf(stderr, "Unknown option character '\\x%x'.\n", optopt);
                    fprintf(stderr, "%s: Usage: -n [number of processes] -h recieve this message\n", argv[0]);
                }
                exit(-1);
                break;
            default:
                perror("Unknown option selected");
                exit(-1);
                break;
        }
    }
    if (FAN_COUNT == -1) 
    {
        fprintf(stderr, "%s: Usage: -n [number of processes] -h recieve this message\n", argv[0]);
        exit(-1);
    }
}

int * fanProcesses(char ** argv) 
{
    
    char commandBuffer[MAX_COMMAND_SIZE];
    int * childPids;
    int counter = 0;
    childPids = (int *) malloc(sizeof(int) * FAN_COUNT);
    memset(childPids, 0, sizeof(childPids));
    while(fgets(commandBuffer, MAX_COMMAND_SIZE, stdin) != NULL && counter < FAN_COUNT) 
    {
        childPids[counter] = fork();
        if (childPids[counter] == -1) 
        {
            fprintf(stderr, "%s: Error: failed to fork a child process.\n", argv[0]);
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
void handleWaits (long int * childPids, char ** argv)
{
    pid_t cpid;
    int numberOfPids = sizeof(childPids)/sizeof(childPids[0]), status = 0;
    for (int i = 0; i < numberOfPids; i++) 
    {
        cpid = waitpid(childPids[i], &status, WUNTRACED | WCONTINUED);
        
        if (cpid == -1 && errno != EINTR) {
            perror("Failed to wait for child");
            exit(EXIT_FAILURE);
        }
        else if (WIFEXITED(status) && !WEXITSTATUS(status))
        {
            fprintf(stdout, "%s: Child Pid %ld: exited, status=%d\n", argv[0], childPids[i], status);
        } 
        else if (WIFSIGNALED(status)) 
        {
            fprintf(stderr, "%s: Error: Child Pid %ld: killed by signal %d\n", argv[0], childPids[i], WTERMSIG(status));
        } 
        else if (WIFSTOPPED(status)) 
        {
            fprintf(stderr, "%s: Error: Child Pid %ld: stopped by signal %d\n",argv[0], childPids[i], WSTOPSIG(status));
        } 
        else if (WIFCONTINUED(status)) 
        {
            fprintf(stdout, "%s: Child Pid %ld: continued\n", argv[0], childPids[i]);
        }
    }
}
