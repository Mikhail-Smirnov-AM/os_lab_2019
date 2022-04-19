#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <sys/time.h>
#include <getopt.h>
#include <pthread.h>

struct FactArgs 
{
  uint32_t begin;
  uint32_t end;
};

uint32_t k = -1;
uint32_t pnum = -1;
uint32_t mod = -1;
uint32_t x = 1;

void ThreadFactorial(struct FactArgs* args);

pthread_mutex_t mut = PTHREAD_MUTEX_INITIALIZER;

int main(int argc, char **argv) 
{
  
  while (true) 
  {
    int current_optind = optind ? optind : 1;

    static struct option options[] = {{"k", required_argument, 0, 0},
                                      {"pnum", required_argument, 0, 0},
                                      {"mod", required_argument, 0, 0},
                                      {0, 0, 0, 0}};

    int option_index = 0;
    int c = getopt_long(argc, argv, "k", options, &option_index);

    if (c == -1) break;

    switch (c) 
    {
      case 0:
        switch (option_index) 
        {
          case 0:
            k = atoi(optarg);
            if (k <= 0) k = -1;
          break;
          case 1:
            pnum = atoi(optarg);
            if (pnum <= 0) pnum = -1;
            break;
          case 2:
            mod = atoi(optarg);
            if (mod <= 0) mod = -1;
            break;
          defalut:
            printf("Index %d is out of options\n", option_index);
          }
      break;
    
      case '?':
        printf("Some error has been occurred. getopt_long returned ?\n");
        return 1;
        break;

      default:
        printf("getopt returned character code 0%o?\n", c);
    }
  }

  if (optind < argc) 
  {
    printf("Has at least one no option argument\n");
    return 1;
  }

  if (k == -1 || pnum == -1 || mod == -1) 
  {
    printf("Usage: %s -k \"num\" --pnum \"num\" --mod \"num\" \n",
           argv[0]);
    return 1;
  }

  if (mod <= k)
  {
    printf("mode <= k.\n");
    printf("Result: %d\n", 0);  
    return 0;
  }
  
  pthread_t threads[pnum];

  struct FactArgs args[pnum];

  for(uint32_t i = 0; i < pnum; i++)
  {
    args[i].begin = i*k/pnum;
    args[i].end = (i+1)*k/pnum;    
  }

  args[0].begin = 1;

  struct timeval start_time;
  gettimeofday(&start_time, NULL);

  for (uint32_t i = 0; i < pnum; i++) 
  {
    if (pthread_create(&threads[i], NULL, ThreadFactorial, (void *)&args[i]))
    {
      printf("Error: pthread_create failed!\n");
      return 1;
    }
  }

  for (uint32_t i = 0; i < pnum; i++) 
  {
    int res = 0;
    pthread_join(threads[i], NULL);
  }

  struct timeval finish_time;
  gettimeofday(&finish_time, NULL);
  
  double elapsed_time = (finish_time.tv_sec - start_time.tv_sec) * 1000.0;
  elapsed_time += (finish_time.tv_usec - start_time.tv_usec) / 1000.0;

  printf("Result: %d\n", x);
  
  printf("Elapsed time: %fms\n", elapsed_time);
  
  fflush(NULL);
  
  return 0;
}

void ThreadFactorial(struct FactArgs* args)
{
  pthread_mutex_lock(&mut);
  for(uint32_t i = args->end; i > args->begin; i--) 
  {
    x *= i;
    x %= mod;
  }
  pthread_mutex_unlock(&mut);
}