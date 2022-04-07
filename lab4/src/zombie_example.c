#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>

int main()
{
  pid_t child_id = fork();
  if(child_id == 0)
  {
      printf("Child process.\n");
      exit(0);
  }

  sleep(10);

  wait(0);
  
  printf("Parent process.\n");
  return 0;
}