#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define QUEUE_SIZE 4
#define CLIENTS_AMOUNT 10

#define MAX_RAND_INT 100
#define MIN_RAND_INT 0

int queue[QUEUE_SIZE];
int pointer;

sem_t online_clients;
int online_clients_value;
pthread_mutex_t queue_locker;
pthread_mutex_t sem_value_locker;
pthread_t clients[CLIENTS_AMOUNT];
pthread_t barber;

int done_clients = 0;

int getRandom()
{
  return rand() % (MAX_RAND_INT - MIN_RAND_INT) + MIN_RAND_INT;
}

void* barber_worker(void* args)
{
  printf("Barber is starting work...\n");
  while(1)
  {
    printf("Barber is waiting for clients, queue size: %d...\n", pointer + 1);

    sem_wait(&online_clients);
    
    printf("Barber got new client...\n");

    pthread_mutex_lock(&queue_locker);
    pointer--;
    pthread_mutex_unlock(&queue_locker);

    printf("Barber removed client, done clients: %d\n", ++done_clients);

    sleep(getRandom() % 3);
  }
}

void* client(void* args)
{
  int num = (int)args;
  sleep(getRandom() % 5);
  int online = 0;

  printf("Client%d created\n", num);

  pthread_mutex_lock(&sem_value_locker);
    printf("Client%d getting value of online clients\n", num);
    sem_getvalue(&online_clients, &online_clients_value);
    online = online_clients_value;

    if(online < QUEUE_SIZE) {
      pthread_mutex_lock(&queue_locker);
      printf("Client%d will be on chair now...\n", num);
      sem_post(&online_clients);

      queue[pointer++] = num;
      printf("Client%d in queue...\n", num);
      printf("Queue size: %d\n", pointer + 1);
      pthread_mutex_unlock(&queue_locker);
    }

  pthread_mutex_unlock(&sem_value_locker);
}

int main() {
  int i;
  pointer = -1;

  sem_init(&online_clients, 0, 0);
  pthread_mutex_init(&queue_locker, NULL);

  pthread_create(&barber, NULL, barber_worker, NULL);

  for (i = 0; i < 20; ++i)
  {
    pthread_create(&(clients[i]), NULL, client, (void*)i);
  }

  pthread_join(barber, NULL);

  return 0;
}