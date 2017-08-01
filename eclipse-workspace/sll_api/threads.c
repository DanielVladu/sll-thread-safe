/*
 * threads.c
 *
 *  Created on: Aug 1, 2017
 *      Author: daniel
 */


#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
//#include <semaphores.h>
#include "sll_api.h"


void *thread1(void* l)
{

  add_node(l,2);
  add_node(l,4);
  add_node(l,10);
  delete_node(l,2);
  sort_list(l);
  delete_node(l,10);
  delete_node(l,5);

  return 0;
}

void *thread2(void *l)
{

  add_node(l,11);
  add_node(l,1);
  delete_node(l,11);
  add_node(l,8);
  print_list(l);

  return 0;
}

void *thread3(void *l)
{

  add_node(l,30);
  add_node(l,25);
  add_node(l,100);
  sort_list(l);
  print_list(l);
  delete_node(l,100);

  return 0;
}

int main()
{
  pthread_t threads[3];
  pthread_attr_t attr;
  int rc;
  void *status;

  list *l = create_list();
  set_verbose(l,1);

  pthread_attr_init(&attr);
  pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);

  rc = pthread_create(&threads[0], &attr, thread1, (void*)l);
  if (rc) {
    printf("ERROR; return code from pthread_create() is %d\n", rc);
    exit(-1);
  }
  rc = pthread_create(&threads[1], &attr, thread2, (void*)l);
  if (rc) {
    printf("ERROR; return code from pthread_create() is %d\n", rc);
    exit(-1);
  }
  rc = pthread_create(&threads[2], &attr, thread3, (void*)l);
  if (rc) {
    printf("ERROR; return code from pthread_create() is %d\n", rc);
    exit(-1);
  }

  pthread_attr_destroy(&attr);

  for(int t=0; t<3; t++)
  {
    rc = pthread_join(threads[t], &status);
    if (rc)
    {
      printf("ERROR; return code from pthread_join() is %d\n", rc);
      exit(-1);
    }
  }
  print_list(l);
  flush_list(l);

  pthread_exit(NULL);

  return 0;
}
