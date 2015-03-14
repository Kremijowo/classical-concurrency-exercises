#include <pthread.h>

class Node 
{
  pthread_t* value;
  Node* next;

public:
  Node(pthread_t* thread)
  {
    value = thread;
  }

  pthread_t* getValue()
  {
    return value;
  }

  Node* getNext()
  {
    return next;
  }

  void setNext(Node* node)
  {
    next = node;
  }
};

class Queue 
{
  Node* root;

public:
  Queue()
  {
    root = NULL;
  }

  Node* last() {
    Node* last = root;
    Node* prev = NULL;

    do
    {
      prev = last;
      last = last->getNext();
    }
    while(last != NULL);

    return prev;
  }

  void push(pthread_t* thread)
  {
    if (root == NULL)
    {
      root = new Node(thread);
    }
    else
    {
      last()->setNext(new Node(thread));
    }
  }

  pthread_t* pop()
  {
    if(!root)
    {
      return NULL;
    }

    Node* currentRoot = root;
    Node* newRoot = root->getNext();

    root = newRoot;

    return currentRoot->getValue();
  }

  int size()
  {
    return sizeSubRoutine(root);
  }

  int sizeSubRoutine(Node* node)
  {
    return node ? sizeSubRoutine(node->getNext()) + 1 : 0;
  }
};