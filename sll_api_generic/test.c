#include <stdio.h>
#include "sll_api.h"
#include <stdlib.h>

int main()
{
  list *l = create_list(L_INT);
  printf("TEST: List created\n");

  float a  = 8.1;
  node *n,*m;
  n = create_node((int*)12, sizeof(int), L_INT);
  
  //printf("TEST: %d\n",*(int*)(n->val));
  n->callback_print(n);
  add_node(l,n);

  l->head->callback_print(l->head);
  print_list(l);



  return 0;
}
