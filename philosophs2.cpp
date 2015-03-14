// ConsoleApplication3.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <process.h>
#include <windows.h>
#include <iostream>
#include <time.h>

#define MAX_RAND_INT 100
#define MIN_RAND_INT 0
#define PHILOSOPHS_NUMBER 4

using namespace std;

void Philosoph(void *);
int getRandom();
void printTab(int);

HANDLE forks[PHILOSOPHS_NUMBER];

int _tmain(int argc, _TCHAR* argv[])
{
  for(int i = 0; i < PHILOSOPHS_NUMBER; ++i)
  {
    char integer_string[32];

    sprintf_s(integer_string, "Event%d", i);
    forks[i] = CreateEvent(NULL, FALSE, TRUE, integer_string);
    SetEvent(forks[i]);

    _beginthread(Philosoph, 1024, (void*)i);
  }

  system("pause");

  return 0;
}

void Philosoph (void * number)
{
  int fork1Num = (int)number;
  int fork2Num = fork1Num == PHILOSOPHS_NUMBER - 1 ? 0 : fork1Num + 1;

  while(true)
  {
    printTab(fork1Num);printf("Philosoph %d is sleeping\n", fork1Num);
    Sleep((getRandom() % 10) * 1000);
    printTab(fork1Num);printf("Philosoph %d is waking up\n", fork1Num);

    int fork1 = 1;
    int fork2 = 1;

    while (fork1 != 0)
    {
      printTab(fork1Num);printf("Philosoph %d is trying to take %d fork\n", fork1Num, fork1Num);
      fork1 = WaitForSingleObject(forks[fork1Num], INFINITE);
    }
    printTab(fork1Num);printf("Philosoph %d takes %d fork\n", fork1Num, fork1Num);
    ResetEvent(forks[fork1Num]);
    printTab(fork1Num);printf("Philosoph %d took %d fork\n", fork1Num, fork1Num);

    while (fork2 != 0)
    {
      printTab(fork1Num);printf("Philosoph %d is trying to take %d fork\n", fork1Num, fork2Num);
      fork2 = WaitForSingleObject(forks[fork2Num], INFINITE);
    }
    printTab(fork1Num);printf("Philosoph %d takes %d fork\n", fork1Num, fork2Num);
    ResetEvent(forks[fork2Num]);
    printTab(fork1Num);printf("Philosoph %d took %d fork\n", fork1Num, fork2Num);

    printTab(fork1Num);printf("Philosoph %d is eating now\n", fork1Num);
    Sleep((getRandom() % 10) * 1000);
    printTab(fork1Num);printf("Philosoph %d ate\n", fork1Num);

    SetEvent(forks[fork1Num]);
    SetEvent(forks[fork2Num]);
    printTab(fork1Num);printf("Philosoph %d returned all forks\n", fork1Num);
  }
}

int getRandom()
{
  return rand() % (MAX_RAND_INT - MIN_RAND_INT) + MIN_RAND_INT;
}

void printTab(int amount)
{
  for(int i = 0; i < amount; ++i)
  {
    printf("  ");
  }
}
