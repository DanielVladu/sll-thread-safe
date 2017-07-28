
#define L_INT 0
#define L_FLOAT 1
#define L_CHAR 2
#define L_STRING 3
#define L_MAXTYPE 4

typedef struct llnode node;
typedef struct llist list;

//nodes of the list
struct llnode {
  void *val;
  int (*callback_print)(node *);
  node *next;
};

//list "object"
struct llist {
  node *head;
  node *tail;
  int n; //size of the list
  int list_type;
};

list* create_list(int);
node* create_node(void *,int,int);
int add_node(list *, node *);
int delete_node(list *, node *);
int sort_list (list *);
int flush_list(list *);
int print_list(list *l);

int printnode_int(node *);
int printnode_char(node *);
int printnode_float(node *);
int printnode_string(node *);
