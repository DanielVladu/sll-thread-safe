#include "sll_api.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

list *create_list(int type)
{
  if (type < L_MAXTYPE)
  {
    list *root = (list*)malloc(sizeof(list));
    root->list_type = type;
    root->head = NULL;
    root->tail = NULL;
    root->n = 0;
    return root;
  }
  else return NULL;
}

int add_node(list *root, int data)
{
  node *aux = (node*)malloc(sizeof(node));
  aux->next = NULL;
  aux->val = data;
  aux->callback_print = printnode_int; //print default as int
  if (root->head == NULL) //means this is first element added
  {
    root->head = aux;
    root->tail = aux;
    root->n++;
    return 1;
  }
  else
  {
    root->tail->next = aux;
    root->tail = aux;
    root->n++;
    return 1;
  }
  return 0;
}

int delete_node(list *root, int data)
{
  node *curr=NULL;
  node *prev=NULL;
  if (root->head->val == data)
  {
    curr = root->head;
    root->head = root->head->next;
    free (curr);
    root->n--;
    return 1;
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
        return 1;
      }
      else if(curr->val == data && curr->next == NULL)
      {
        prev->next = NULL;
        root->tail = prev;
        free (curr);
        root->n--;
        return 1;
      }
      else
      {
        curr = curr->next;
        prev = prev->next;
      }
    }
  }
  return 0;
}

int flush_list(list* root)
{
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
  return 1;
}


int print_list(list *root)
{
  node *aux = root->head;
  while (aux != NULL)
  {
    aux->callback_print(aux);
    aux = aux->next;
  }
  return 1;
}

int printnode_int(node *data)
{
  printf("Node value: %d\n",data->val);
  return 1;
}
