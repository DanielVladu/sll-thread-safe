#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <pthread.h>
#include <unistd.h>
#include "sll_api.h"

list *create_list()
{
  printf("(PID:%ld,TID:%ld) List created\n",(long)getpid(),(long)pthread_self());

    list *root = (list*)malloc(sizeof(list));
    root->head = NULL;
    root->tail = NULL;
    root->verbose = 0;
    root->n = 0;
    return root;
}

int add_node(list *root, int data)
{
  int status = 0;

  node *aux = (node*)malloc(sizeof(node));
  aux->next = NULL;
  aux->val = data;
  aux->callback_print = printnode_int;

  pthread_rwlock_wrlock(&(root->m));
  if (root->head == NULL)
  {
    root->head = aux;
    root->tail = aux;
    root->n++;
    pthread_rwlock_unlock(&(root->m));
    status = 1;
  }
  else
  {
    pthread_rwlock_wrlock(&(root->tail->m));
    root->tail->next = aux;
    pthread_rwlock_unlock(&(root->tail->m));
    root->tail = aux;
    root->n++;
    pthread_rwlock_unlock(&(root->m));
    status = 1;
  }

  pthread_rwlock_rdlock(&(root->m));
  if (status && root->verbose)
  {
  pthread_rwlock_unlock(&(root->m));
  printf("(PID:%ld,TID:%ld) Added node %d (n=%d)\n",(long)getpid(),(long)pthread_self(),data,root->n);
  return 1;
  }
  if (!status && root->verbose)
  {
  pthread_rwlock_unlock(&(root->m));
  printf("(PID:%ld,TID:%ld) Problem adding node %d (n=%d)\n",(long)getpid(),(long)pthread_self(),data,root->n);
  return -1;
  }
  return 0;
}

int delete_node(list *root, int data)
{

  int status = 0;
  node *curr=NULL;
  node *prev=NULL;
  pthread_rwlock_wrlock(&(root->m));
  if (root->head->val == data)
  {
    curr = root->head;
    root->head = root->head->next;
    //pthread_rwlock_destroy(&(curr->m));
    free (curr);
    root->n--;
    pthread_rwlock_unlock(&(root->m));
    status = 1;
  }
  else
  {
    pthread_rwlock_wrlock(&(root->head->m));
    prev = root->head;
    curr = root->head->next;
    while (curr !=NULL)
    {
      if (curr->val == data && curr->next != NULL)
      {
        prev->next = curr->next;
        //pthread_rwlock_destroy(&(curr->m));
        free(curr);
        root->n--;
        status = 1;
      }
      else if(curr->val == data && curr->next == NULL)
      {
        prev->next = NULL;
        root->tail = prev;
        //pthread_rwlock_destroy(&(curr->m));
        free (curr);
        root->n--;
        status = 1;
      }
      else
      {
        curr = curr->next;
        prev = prev->next;
      }
    }
    pthread_rwlock_unlock(&(root->head->m));
    pthread_rwlock_unlock(&(root->m));
  }
  pthread_rwlock_rdlock(&(root->m));

  if (status && root->verbose)
  {
    pthread_rwlock_unlock(&(root->m));
    printf("(PID:%ld,TID:%ld) Deleted node %d (n=%d)\n",(long)getpid(),(long)pthread_self(),data,root->n);
    return 1;
  }
  else if (!status && root->verbose)
  {
    pthread_rwlock_unlock(&(root->m));
    printf("(PID:%ld,TID:%ld) Node with val %d not found to delete (n=%d)\n",(long)getpid(),(long)pthread_self(),data,root->n);
    return -1;
  }
  return 0;
}

int flush_list(list* root)
{
  pthread_rwlock_wrlock(&(root->m));
  if (root->verbose)
  {
  printf("(PID:%ld,TID:%ld) Flushing list\n",(long)getpid(),(long)pthread_self());
  }
  node *curr = root->head;
  node *next;
  while (curr != NULL)
  {
    next = curr->next;
    pthread_rwlock_destroy(&(curr->m));
    free(curr);
    curr = next;
  }
  root->head = NULL;
  root->tail = NULL;
  root->n = 0;
  pthread_rwlock_unlock(&(root->m));
  return 1;
}

void sort_list(list *root)
{
  pthread_rwlock_wrlock(&(root->m));
  if (root->verbose)
  {
    printf("(PID:%ld,TID:%ld) Sorting list\n",(long)getpid(),(long)pthread_self());
  }
  mergesort(&(root->head));

  node *aux = root->head;
  while(aux != NULL)
  {
    if (root->tail->val <= aux->val )
    {
      root->tail = aux;
    }
    aux = aux->next;
  }
  pthread_rwlock_unlock(&(root->m));
}

void mergesort(node** headRef)
{
    node* head = *headRef;
    node* a;
    node* b;
    if ((head == NULL) || (head->next == NULL))
    {
        return;
    }
    mergesplit(head, &a, &b);
    mergesort(&a);
    mergesort(&b);
    *headRef = sortedmerge(a, b);
}

node* sortedmerge(node* a, node* b)
{
    node* result = NULL;

    if (a == NULL)
        return(b);
    else if (b == NULL)
        return(a);

    if ( a->val <= b->val)
    {
        result = a;
        result->next = sortedmerge(a->next, b);
    }
    else
    {
        result = b;
        result->next = sortedmerge(a, b->next);
    }

    return(result);
}

void mergesplit(node* source, node** frontRef,node** backRef)
{
    node* fast;
    node* slow;
    if (source==NULL || source->next==NULL)
    {
        *frontRef = source;
        *backRef = NULL;
    }
    else
    {
        slow = source;
        fast = source->next;
        while (fast != NULL)
        {
            fast = fast->next;
            if (fast != NULL)
            {
                slow = slow->next;
                fast = fast->next;
            }
         }

    *frontRef = source;
    *backRef = slow->next;
    slow->next = NULL;
    }
}

int print_list(list *root)
{
  pthread_rwlock_rdlock(&(root->m));
  printf("(PID:%ld,TID:%ld) Printing list: \n",(long)getpid(),(long)pthread_self());

  node *aux = root->head;
  while (aux != NULL)
  {
    aux->callback_print(aux);
    aux = aux->next;
  }
  pthread_rwlock_unlock(&(root->m));
  return 1;
}

int printnode_int(node *data)
{
  printf("---Node value: %d\n",data->val);
  return 1;
}

void set_verbose(list* root, int x)
{
  pthread_rwlock_wrlock(&(root->m));
  if (x)
    root->verbose = 1;
  else root->verbose = 0;
  pthread_rwlock_unlock(&(root->m));
}
