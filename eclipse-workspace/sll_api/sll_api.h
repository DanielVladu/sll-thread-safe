/*
 * sll_api.h
 *
 *  Created on: Aug 1, 2017
 *      Author: daniel
 */

#ifndef SLL_API_H_
#define SLL_API_H_


#include <pthread.h>

typedef struct llnode node;
typedef struct llist list;

//nodes of the list
struct llnode {
  int val;
  int (*callback_print)(node *);
  pthread_rwlock_t m;
  node *next;
};

//list "object"
struct llist {
  node *head;
  node *tail;
  int n; //size of the list
  int verbose;
  pthread_rwlock_t m;
};

list* create_list();

int add_node(list *, int);
int delete_node(list *, int);
int flush_list(list *);

void sort_list(list *);
void mergesort(node**);
node* sortedmerge(node* , node*);
void mergesplit(node*, node**,node** );

int print_list(list *);
int printnode_int(node *);

void set_verbose(list *,int);


#endif /* SLL_API_H_ */
