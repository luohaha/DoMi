#include "domi.h"
/*
  isGive : 是否已经被赋予初值
*/
Value* createVar(Node *node, char *name, Boolean isGive, ValueType type) {
  
  Value *newValue = (Value*) malloc(sizeof(Value));
  
  (*newValue).isGivedValue = isGive;
  (*newValue).node = node;
  (*newValue).type = type;
  (*newValue).varname = name;
  return newValue;
}
