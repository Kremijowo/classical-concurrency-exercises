// readerwriter.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <process.h>
#include <windows.h>
#include <iostream>
#include <time.h>

#define MAX_RAND_INT 100
#define MIN_RAND_INT 0
#define WRITERS_NUMBER 2
#define READERS_NUMBER 2

using namespace std;

unsigned long __stdcall Reader(void*);
unsigned long __stdcall Writer(void*);
int getRandom();
void sleep(int);

CRITICAL_SECTION writersBlocker;
CRITICAL_SECTION readersBlocker;
CRITICAL_SECTION allReadersBlocker;
CRITICAL_SECTION readersCounterBlocker;
CRITICAL_SECTION writersCounterBlocker;

int readersAmount;
int writersAmount;
HANDLE writers[WRITERS_NUMBER];
HANDLE readers[READERS_NUMBER];
DWORD threadID;

int _tmain(int argc, _TCHAR* argv[])
{
  readersAmount = 0;
  writersAmount = 0;
  InitializeCriticalSection(&writersBlocker);
  InitializeCriticalSection(&readersBlocker);
  InitializeCriticalSection(&allReadersBlocker);
  InitializeCriticalSection(&readersCounterBlocker);
  InitializeCriticalSection(&writersCounterBlocker);

  for(int i = 0; i < WRITERS_NUMBER; ++i)
  {
    writers[i] = CreateThread(NULL, 0, Writer, (void*)i, 0, &threadID);
  }

  for(int i = 0; i < READERS_NUMBER; ++i)
  {
    readers[i] = CreateThread(NULL, 0, Reader, (void*)i, 0, &threadID);
  }

  system("pause");

  return 0;
}

unsigned long __stdcall Reader(void* num)
{
  int number = (int)num;
  printf("Started Reader %d\n", number);

  while(true)
  {
    EnterCriticalSection(&allReadersBlocker);
    
      EnterCriticalSection(&readersBlocker);
        EnterCriticalSection(&readersCounterBlocker);
        readersAmount++;
        if(readersAmount == 1)
        {
          EnterCriticalSection(&writersBlocker);
        }
        LeaveCriticalSection(&readersCounterBlocker);
      LeaveCriticalSection(&readersBlocker);

    LeaveCriticalSection(&allReadersBlocker);
    
    printf("Reader %d started to read\n", number);
    sleep(3);
    printf("Reader %d read\n", number);

    EnterCriticalSection(&readersCounterBlocker);
      readersAmount--;
      if(readersAmount == 0)
      {
        LeaveCriticalSection(&writersBlocker);
      }
    LeaveCriticalSection(&readersCounterBlocker);

    sleep(3);
  }

  return 0;
}

unsigned long __stdcall Writer(void* num)
{
  int number = (int)num;
  printf("  Started Writer %d\n", number);
  
  while(true)
  {
    EnterCriticalSection(&writersCounterBlocker);
      writersAmount++;
      if(writersAmount == 1)
      {
        EnterCriticalSection(&readersBlocker);
      }
    LeaveCriticalSection(&writersCounterBlocker);

    EnterCriticalSection(&writersBlocker);
      printf("  Writer %d started to write\n", number);
      sleep(5);
      printf("  Writer %d wrote\n", number);
    LeaveCriticalSection(&writersBlocker);

    EnterCriticalSection(&writersCounterBlocker);
      writersAmount--;
      if(writersAmount == 0)
      {
        LeaveCriticalSection(&readersBlocker);
      }
    LeaveCriticalSection(&writersCounterBlocker);

    sleep(10);
  }
  return 0;
}


int getRandom()
{
  return rand() % (MAX_RAND_INT - MIN_RAND_INT) + MIN_RAND_INT;
}

void sleep(int maxSleepSeconds)
{
  Sleep((getRandom() % maxSleepSeconds) * 1000);
}
