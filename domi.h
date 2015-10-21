#include<stdio.h>
#include<stdlib.h>

typedef enum Boolean_e {
   TRUE = 1,
   FALSE = 0
 } Boolean;

typedef enum ValueType_e {
  STRING_TYPE = 1,
  INTEGER_TYPE = 2,
  DOUBLE_TYPE = 3
}ValueType;

/*
  存储数据的真实值
*/
typedef struct Node_t {
  char *str;
  int   integer;
  double doub;
  ValueType type;
} Node;
/*
  存储变量
*/
typedef struct Value_t{
  char *varname;
  Node *node;
  Boolean  isGivedValue;
  ValueType type;
} Value;

/*
  变量的链表，把变量当作单链表来存储
*/
typedef struct VarLink_t {
  Value *value;
  struct VarLink_t *next;
} VarLink;

extern VarLink *head;

Value* createVar(Node *node, char *name, Boolean isGive, ValueType type);
Node *simpleComputer(Node *node1, Node *node2, char type);
VarLink* init_var();
