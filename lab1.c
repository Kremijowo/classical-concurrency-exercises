#define BUF_SIZE     10
#define NC           8
#define NP           3
#define MAX_RAND_INT 100
#define MIN_RAND_INT 0

#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct 
{
  int buf[BUF_SIZE];
  int pointer;
  sem_t mutex;
  sem_t full;
  sem_t empty;
} ctrl;

ctrl c;

int getRandom()
{
  return rand() % (MAX_RAND_INT - MIN_RAND_INT) + MIN_RAND_INT;
}

void *producer(void * arg) 
{
  int prod_id = (int)arg;
  int item;

  printf("\n\nCreated producer with id %d \n\n", prod_id);

  while(1)
  {
    item = getRandom();
    printf("Produced item %d by %d\n", item, prod_id);

    sem_wait(&c.full);
    sem_wait(&c.mutex);
    c.buf[c.pointer++] = item;
    printf("Added produced item %d by %d\n to buf in index %d\n", item, prod_id, c.pointer);
    sem_post(&c.mutex);
    sem_post(&c.empty);

    sleep(getRandom() % 3);
  }
}

void *consumer(void *arg)
{
  int cons_id = (int)arg;

  printf("\n\nCreated consumer with id %d \n\n", cons_id);

  while(1)
  {
    sem_wait(&c.empty);
    sem_wait(&c.mutex);
    printf("Took item %d by consumer%d from buf with index %d\n", c.buf[c.pointer], cons_id, c.pointer);
    c.pointer--;
    sem_post(&c.mutex);
    sem_post(&c.full);

    sleep(getRandom() % 3);
  }

}

void main() 
{
  pthread_t ptreads[NP];
  pthread_t ctreads[NC];

  sem_init(&c.mutex, 0, 1);
  sem_init(&c.empty, 0, 0);
  sem_init(&c.full, 0, BUF_SIZE);
  c.pointer = 0;
  int i;

  for(i = 0; i < NP; ++i)
  {
    pthread_create(&(ptreads[i]), NULL, producer, (void*)i);
  }

  for(i = 0; i < NC; ++i) {
    pthread_create(&(ctreads[i]), NULL, consumer, (void*)i);
  }

  pthread_join((ptreads[0]), NULL);
  
  // for(i = 0; i < NP; ++i) 
  // {
  //   if(pthread_join((ptreads[i]), NULL)) 
  //   {
  //     printf("\n ERROR joining thread with i = %d\n", i);
  //     exit(1);
  //   }
  // }

  // for(i = 0; i < NC; ++i) {
  //   if(pthread_join((ctreads[i]), NULL)) {
  //     printf("\n ERROR joining thread");
  //     exit(1);
  //   }
  // }
}