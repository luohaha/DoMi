#include<stdio.h>
#include<stdlib.h>

 enum Boolean {
   TRUE = 1,
   FALSE = 0
 };

enum ValueType {
  STRING = 1,
  INTEGER = 2,
  DOUBLE = 3
}

/*
  存储数据的真实值
*/
typedef struct Node_t {
  char *str;
  int   integer;
  double doub;
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

Value* createVar(Node *node, char *name, Boolean isGive, ValueType type);
