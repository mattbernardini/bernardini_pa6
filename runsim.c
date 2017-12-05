#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
int pr_count;
int main (int argc, char *argv[]) {
  int pr_limit;

  if (argc != 2){ /* check for valid number of command-line arguments */
    fprintf(stderr, "Usage: %s processes\n", argv[0]);
    return 1;
  }

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
                        
  return 0;
}