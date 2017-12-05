#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int pr_count;
int main (int argc, char *argv[]) {
  char s[256];
  // Check for valid number of command-line arguments
  if (argc != 2){ 
    fprintf(stderr, "Usage: %s processes\n", argv[0]);
    return 1;
  }
  printf("argc: %d", argc);
  fgets(s, 256, stdin);
  printf("%s", s);
  /*
  // Declare variables to be used in the rest of the program
  pid_t cpid;
  int shmId,i,pr_limit,*shmPtr;
  key_t key;
  FILE *fp;

  // Allocate shared memory to store the number of running processes
  (shmId = shmget(key, sizeof(int), IPC_CREAT | 0666));
  shmPtr = (int *) shmat( shmId, NULL, 0);

  // Set the number of concurrent processes to let run
  pr_limit = atoi(argv[1]);


  for (int i = 1; i < n; i++)
    int childpid = fork()
    switch (childpid)
    {
      case -1:
      {
          printf("Error\n");
          exit(255);
      }
      case 0:
      {
        exec()

        children = children - 1;
        if (children < 0) children = 0;

        exit(1);
      }
      default:
      {   
        if (children > pr_limit)
            wait();
        children++;

        pid = fork();
      }

  fprintf(stderr, "i:%d process ID:%ld parent ID:%ld child ID:%ld\n",
                        i, getpid(), getppid(), childpid);
  */                      
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