#include <stdio.h>
#include "sll_api.h"
#include <stdlib.h>

int main()
{
  list *l = create_list();
  printf("TEST: List created\n");

  add_node(l,899);
  add_node(l,80);
  add_node(l,81);

  printf("TEST: n = %d\n", l->n);

//  l->tail->callback_print(l->tail);
  print_list(l);

  delete_node(l,81);

  print_list(l);



  return 0;
}
