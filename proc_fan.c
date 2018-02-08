/**
 * Matt Bernardini
 * cs4760 HW1
 * 2/08/2017
 * 
 * runsim.c
*/

#define _XOPEN_SOURCE
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
#include <getopt.h>
#include <ctype.h>

static int FAN_COUNT = -1;
static int MAX_COMMAND_SIZE = 256;
static int PROCESSES_FANNED = 0;

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
    childPids = (int *) malloc(sizeof(int) * FAN_COUNT);
    memset(childPids, 0, sizeof(&childPids));
    while(fgets(commandBuffer, MAX_COMMAND_SIZE, stdin) != NULL && PROCESSES_FANNED < FAN_COUNT) 
    {
        childPids[PROCESSES_FANNED] = fork();
        if (childPids[PROCESSES_FANNED] == -1) 
        {
            fprintf(stderr, "%s: Error: failed to fork a child process.\n", argv[0]);
            perror((const char *)(long)errno);
        }
        else if (childPids[PROCESSES_FANNED] == 0)
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
            PROCESSES_FANNED += 1;
        }
    }
    return childPids;
}
void handleWaits (int * childPids, char ** argv)
{
    pid_t cpid;
    int *status;
    status = malloc(sizeof(int));
    for (int i = 0; i < PROCESSES_FANNED; i++) 
    {
        while(((cpid = waitpid(childPids[i], status, 0)) == -1) &&
                    (errno == EINTR));
        
        if (cpid == -1 && errno != EINTR) {
            perror("Failed to wait for child");
            exit(EXIT_FAILURE);
        }
        else if (WIFEXITED(*status))
        {
            fprintf(stdout, "%s: Child Pid %ld: exited, status=%d\n", argv[0], (long) childPids[i], *status);
        } 
        else if (WIFSIGNALED(*status)) 
        {
            fprintf(stderr, "%s: Error: Child Pid %ld: killed by signal %d\n", argv[0], (long) childPids[i], WTERMSIG(*status));
        } 
        else if (WIFSTOPPED(*status)) 
        {
            fprintf(stderr, "%s: Error: Child Pid %ld: stopped by signal %d\n",argv[0], (long) childPids[i], WSTOPSIG(*status));
        } 
        else if (WIFCONTINUED(status)) 
        {
            fprintf(stdout, "%s: Child Pid %ld: continued\n", argv[0], (long) childPids[i]);
        }
    }
    free(status);
}