#include "domi.h"

/*
  isGive : 是否已经被赋予初值
*/
Value* create(Node *node, Boolean isGive, ValueType type) {
  
  Value *newValue = (Value*) malloc(sizeof(Value));
  
  (*newValue).isGivedValue = isGive;
  (*newValue).node = newNode;
  (*newValue).type = type;
  return newValue;
}



