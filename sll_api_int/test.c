#include <stdio.h>
#include "sll_api.h"
#include <stdlib.h>

int main()
{
  list *l = create_list();
  set_verbose(l,1);

  add_node(l,899);
  add_node(l,80);
  add_node(l,81);
  add_node(l,-90);
  add_node(l,0);
  add_node(l,3);


//  l->tail->callback_print(l->tail);
  print_list(l);

//  delete_node(l,81);
  printf("TAIL VEFORE SORT %d\n",l->tail->val);
  sort_list(l);
printf("TAIL AFTER SORT %d\n",l->tail->val);
  print_list(l);

  delete_node(l,900);
  print_list(l);
  delete_node(l,80);
  print_list(l);
  add_node(l,2);
  print_list(l);
  delete_node(l,81);

  print_list(l);


  return 0;
}
