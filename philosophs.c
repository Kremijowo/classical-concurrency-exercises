//#include <stdafx.h>
#include <windows.h>
#include <iostream>
#include <process.h> 

using namespace std;

HANDLE event;

void Test1(void*);
void Test2(void*);
void Test3(void*);

int main()
{
  event = CreateEvent(NULL, TRUE, FALSE, "FIRST STEP");

  if (_beginthread(Test1, 1024, NULL) == -1)
  {
    cout << "Error begin thread " << endl;
  }
  if (_beginthread(Test2, 1024, NULL) == -1)
  {
    cout << "Error begin thread " << endl;
  }
  if (_beginthread(Test3, 1024, NULL) == -1)
  {
    cout << "Error begin thread " << endl;
  }

  if(event != NULL)
  {
    Sleep(1000);
    SetEvent(event);
    Sleep(1000);

    ResetEvent(event);
    CloseHandle(event);
  }
  else
  {
    cout << "error create event" << endl;
  }
  
  return 0;
}

void Test1(void* args)
{
  DWORD dwWaitResult;
  while(dwWaitResult != WAIT_OBJECT_0)
  {
    dwWaitResult = WaitForSingleObject(event, 1);
    cout << "Test 1 timeout" << endl;
  }

  cout << "Event Test 1" << endl;
  _endthread;
}

void Test2(void* args)
{
  DWORD dwWaitResult;
  while(dwWaitResult != WAIT_OBJECT_0)
  {
    dwWaitResult = WaitForSingleObject(event, 1);
    cout << "Test 2 timeout" << endl;
  }

  cout << "Event Test 2" << endl;
  _endthread;
}

void Test3(void* args)
{
  DWORD dwWaitResult;
  while(dwWaitResult != WAIT_OBJECT_0)
  {
    dwWaitResult = WaitForSingleObject(event, 1);
    cout << "Test 3 timeout" << endl;
  }

  cout << "Event Test 3" << endl;
  _endthread;
}
