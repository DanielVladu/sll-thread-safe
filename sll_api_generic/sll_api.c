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

node* create_node(void *data, int size, int data_type)
{
  node *aux = (node*)malloc(sizeof(node));
  aux->next = NULL;
  aux->val = malloc(size+1);
  memcpy(aux->val, &data, size);

  if (data_type == L_INT) {
    aux->callback_print = printnode_int;
  }
  if (data_type == L_FLOAT) {
    aux->callback_print = printnode_float;
  }
  if (data_type == L_CHAR) {
    aux->callback_print = printnode_char;
  }
  if (data_type == L_STRING) {
    aux->callback_print = printnode_string;
  }
  return aux;
}

int add_node(list *root, node* data)
{
  if (root->head == NULL) //means this is first element added
  {
    root->head = data;
    root->tail = data;
    root->n++;
    return 1;
  }
  else
  {
    root->tail->next = data;
    root->tail = data;
    root->n++;
    return 1;
  }
  return 0;
}

int print_list(list *root)
{
  node *aux = (node*)malloc(sizeof(node));
  aux = root->head;
  if (aux != NULL)
  {
    aux->callback_print(aux);
    aux = aux->next;
  }
  return 1;
}
/*ADD NODE BY VALUE
int add_node(list *root, void *data)
{
node *aux = (node*)malloc(sizeof(node));
if (root->head == NULL) //means this is first element added
{
aux->val = data;
aux->next = NULL;
root->head = aux;
root->tail = aux;
root->n++;
return 1;
}
else
{
aux->val = data;
aux->next = NULL;
root->tail->next = aux;
root->tail = aux;
root->n++;
return 1;
}
else return 0;
}*/

int printnode_int(node *data)
{
  printf("Node value: %d\n",*(int*)(data->val));
  return 1;
}
int printnode_char(node *data)
{
  printf("Node value: %c\n",*(char*)(data->val));
  return 1;
}
int printnode_float(node *data)
{
  printf("Node value: %f\n",*(float*)(data->val));
  return 1;
}
int printnode_string(node *data)
{
//  TODO
return 1;
}
