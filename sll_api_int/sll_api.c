#include <stdio.h>
#include <stdlib.h>
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
    pthread_rwlock_init(&root->m, NULL);
    return root;
}

int add_node(list *root, int data)
{
  int status = 0;

  if(root == NULL)
  {
    printf("(PID:%ld,TID:%ld) ERR add_node:List is NULL\n",(long)getpid(),(long)pthread_self());
    return -1;
  }
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
    root->tail->next = aux;
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
  if (root == NULL || root->head == NULL || root->tail == NULL)
  {
    status = 0;
    printf("(PID:%ld,TID:%ld) ERR delete_node:List or list's head or tail is NULL\n",(long)getpid(),(long)pthread_self());
    return -1;
  }
  pthread_rwlock_wrlock(&(root->m));
  if (root->head->val == data)
  {
    curr = root->head;
    root->head = root->head->next;
    free (curr);
    root->n--;
    pthread_rwlock_unlock(&(root->m));
    status = 1;
  }
  else
  {
    prev = root->head;
    curr = root->head->next;
    while (curr !=NULL)
    {
      if (curr->val == data && curr->next != NULL)
      {
        prev->next = curr->next;
        free(curr);
        root->n--;
        status = 1;
      }
      else if(curr->val == data && curr->next == NULL)
      {
        prev->next = NULL;
        root->tail = prev;
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
  if (root == NULL)
  {
    printf("(PID:%ld,TID:%ld) ERR flush_list:List is NULL\n",(long)getpid(),(long)pthread_self());
    return -1;
  }
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
    free(curr);
    curr = next;
  }
  root->head = NULL;
  root->tail = NULL;
  root->n = 0;
  pthread_rwlock_unlock(&(root->m));
  return 1;
}

int sort_list(list *root)
{
  if (root == NULL)
  {
    printf("(PID:%ld,TID:%ld) ERR sort_list:List is NULL\n",(long)getpid(),(long)pthread_self());
    return -1;
  }
  if (root->head == NULL)
  {
    printf("(PID:%ld,TID:%ld) ERR sort_list:List is empty\n",(long)getpid(),(long)pthread_self());
    return -1;
  }
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
  return 1;
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
  if (root == NULL)
  {
    printf("(PID:%ld,TID:%ld) ERR print_list:List is NULL\n",(long)getpid(),(long)pthread_self());
    return -1;
  }
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

int set_verbose(list* root, int x)
{
  if (root == NULL)
  {
    printf("(PID:%ld,TID:%ld) ERR set_verbose:List is NULL\n",(long)getpid(),(long)pthread_self());
    return -1;
  }
  pthread_rwlock_wrlock(&(root->m));
  if (x)
    root->verbose = 1;
  else root->verbose = 0;
  pthread_rwlock_unlock(&(root->m));
  return 1;
}
