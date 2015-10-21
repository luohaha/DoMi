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

/*
  创建新的实际参数链表
*/
ArgumentList *createArgumentList(Node *head) {
  ArgumentList *newList = (ArgumentList*)malloc(sizeof(ArgumentList));
  newList->node = head;
  newList->next = NULL;
  return newList;
}
/*
  在实际参数链表中插入新的
*/
void insertIntoArgumentList(ArgumentList *head, Node *node) {
  if (head == NULL) {
    fprintf(stderr, "实际参数链表不存在\n");
    return;
  }
  ArgumentList *newList = (ArgumentList*)malloc(sizeof(ArgumentList));
  newList->node = node;
  newList->next = head->next;
  head->next = newList;
  return;
}
