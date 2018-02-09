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

static int MAX_COMMAND_SIZE = 256;
static int PROCESSES_ALIVE = 0;
static int PROCESS_LIMIT = 0;

void handleOpts(int argc, char ** argv);
int * fanProcesses(char ** argv);
void handleWait (char ** argv);

int main (int argc, char *argv[]) 
{
    int * childPids;
    handleOpts(argc, argv);
    childPids = fanProcesses(argv);
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
                PROCESS_LIMIT = atoi(optarg);
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
    if (PROCESS_LIMIT == -1) 
    {
        fprintf(stderr, "%s: Usage: -n [number of processes] -h recieve this message\n", argv[0]);
        exit(-1);
    }
}

int * fanProcesses(char ** argv) 
{
    char commandBuffer[MAX_COMMAND_SIZE];
    int * childPids, counter = 0, currentChildPidSize = PROCESS_LIMIT;
    childPids = (int *) malloc(sizeof(int) * PROCESS_LIMIT);
    memset(childPids, 0, sizeof(&childPids));
    while(fgets(commandBuffer, MAX_COMMAND_SIZE, stdin) != NULL) 
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
        }
        else 
        {
            PROCESSES_ALIVE += 1;
            counter += 1;
            if (counter >= currentChildPidSize) 
            {
                // If we are in here our childPids array needs more memory
               childPids = realloc(childPids, sizeof(int) * (counter * 2)); 
               currentChildPidSize = 2 * counter;
            }
            if (PROCESSES_ALIVE >= PROCESS_LIMIT)
            {
                handleWait(argv);
                PROCESSES_ALIVE -= 1;
            }
        }
    }
    while (PROCESSES_ALIVE > 0)
    {
        handleWait(argv);
        PROCESSES_ALIVE -= 1; 
    }
    return childPids;
}
void handleWait (char ** argv)
{
    pid_t cpid;
    int *status;
    status = malloc(sizeof(int));
    while(((cpid = waitpid(-1, status, 0)) == -1) &&
            (errno == EINTR));
        
    if (cpid == -1 && errno != EINTR) {
        perror("Failed to wait for child");
        exit(EXIT_FAILURE);
    }
    else if (WIFEXITED(*status))
    {
        fprintf(stdout, "%s: Child Pid %ld: exited, status=%d\n", argv[0], (long) cpid, *status);
    } 
    else if (WIFSIGNALED(*status)) 
    {
        fprintf(stderr, "%s: Error: Child Pid %ld: killed by signal %d\n", argv[0], (long) cpid, WTERMSIG(*status));
    } 
    else if (WIFSTOPPED(*status)) 
    {
        fprintf(stderr, "%s: Error: Child Pid %ld: stopped by signal %d\n",argv[0], (long) cpid, WSTOPSIG(*status));
    } 
    else if (WIFCONTINUED(status)) 
    {
        fprintf(stdout, "%s: Child Pid %ld: continued\n", argv[0], (long) cpid);
    }
    free(status);
}
