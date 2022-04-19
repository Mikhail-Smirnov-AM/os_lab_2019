/********************************************************
 * An example source module to accompany...
 *
 * "Using POSIX Threads: Programming with Pthreads"
 *     by Brad nichols, Dick Buttlar, Jackie Farrell
 *     O'Reilly & Associates, Inc.
 *  Modified by A.Kostin
 ********************************************************
 * mutex.c
 *
 * Simple multi-threaded example with a mutex lock.
 */
#include <errno.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

void do_one_thing(int *);
void do_another_thing(int *);
int common = 0; /* A shared variable for two threads */
pthread_mutex_t mut1 = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t mut2 = PTHREAD_MUTEX_INITIALIZER;

int main() 
{
  pthread_t thread1, thread2;

  if (pthread_create(&thread1, NULL, (void *)do_one_thing,
			  (void *)&common) != 0) {
    perror("pthread_create");
    exit(1);
  }

  if (pthread_create(&thread2, NULL, (void *)do_another_thing,
                     (void *)&common) != 0) {
    perror("pthread_create");
    exit(1);
  }

  if (pthread_join(thread1, NULL) != 0) {
    perror("pthread_join");
    exit(1);
  }

  if (pthread_join(thread2, NULL) != 0) {
    perror("pthread_join");
    exit(1);
  }

  return 0;
}

void do_one_thing(int *pnum_times) 
{
  pthread_mutex_lock(&mut1);
  printf("doing one thing\n");
  sleep(5);        /* long cycle */
  printf("Поток 1 пытается обратиться к ресурсу mut2...\n");
  pthread_mutex_lock(&mut2);
  printf("Поток 1 не может попасть в эту секцию.\n");
  pthread_mutex_unlock(&mut2);
  pthread_mutex_unlock(&mut1);
}

void do_another_thing(int *pnum_times) 
{
  pthread_mutex_lock(&mut2);
  printf("doing another thing\n");
  sleep(5);        /* long cycle */
  printf("Поток 2 пытается обратиться к ресурсу mut1...\n");
  pthread_mutex_lock(&mut1);
  printf("Поток 2 не может попасть в эту секцию.\n");
  pthread_mutex_unlock(&mut1);
  pthread_mutex_unlock(&mut2);
}

