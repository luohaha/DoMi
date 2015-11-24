#include "domi.h"
/*
  isGive : 是否已经被赋予初值
*/
Value* createVar(char *name, Boolean isGive, ValueType type) {
  
  Value *newValue = (Value*) malloc(sizeof(Value));
  
  (*newValue).isGivedValue = isGive;
  (*newValue).varname = name;
  newValue->type = type;
  return newValue;
}
/*
  创建新的语句链表
*/
BagLink *createBlockExpList(Bag *bag) {
  BagLink *newLink = (BagLink*)malloc(sizeof(BagLink));
  newLink->bag =bag;
  newLink->next = newLink;
  newLink->prev = newLink;
  return newLink;
}
/*
  插入语句链表
*/
BagLink *insertIntoBlockExpList(BagLink *link, Bag *bag) {
  BagLink *newLink = (BagLink*)malloc(sizeof(BagLink));
  newLink->bag = bag;
  newLink->prev = link->prev;
  newLink->next = link;
  
  newLink->next->prev = newLink;
  newLink->prev->next = newLink;

  return link;
}
/*
  创建新的形式参数链表
*/
VarLink *createStatementList(Value *value) {
  VarLink *newLink = (VarLink*)malloc(sizeof(VarLink));
  newLink->value = value;
  newLink->next = NULL;
  return newLink;
}

/*
  在形式参数链表中插入
*/
VarLink* insertIntoStatementList(VarLink *link, Value *value) {
  VarLink *newLink = (VarLink*)malloc(sizeof(VarLink));
  newLink->value = value;
  newLink->next = link->next;
  link->next = newLink;
  return link;
}

/*
  创建新的实际参数链表
*/
ArgumentList *createArgumentList(Bag *head) {
  ArgumentList *newList = (ArgumentList*)malloc(sizeof(ArgumentList));
  newList->bag = head;
  newList->next = NULL;
  return newList;
}
/*
  在实际参数链表中插入新的
*/
void insertIntoArgumentList(ArgumentList *head, Bag *bag) {
  if (head == NULL) {
    fprintf(stderr, "实际参数链表不存在\n");
    return;
  }
  ArgumentList *newList = (ArgumentList*)malloc(sizeof(ArgumentList));
  newList->bag = bag;
  newList->next = head->next;
  head->next = newList;
  return;
}

/*
  新建binary_op
*/

Bag *createBinaryOp(Bag *bag1, Bag *bag2, char type) {
  Bag *newBag = (Bag*) malloc(sizeof(Bag));
  Binary_op *binary = (Binary_op*) malloc(sizeof(Binary_op));
  binary->type = type;
  binary->left = bag1;
  binary->right = bag2;
  newBag->type = "binary_op";
  newBag->binary_op = binary;
  return newBag;
}

/*
  新建assigment_op
*/
Bag *createAssigmentOp(Value *recv, Bag *send) {
  Bag *newBag = (Bag*) malloc(sizeof(Bag));
  Assigment_op *assigment = (Assigment_op*)malloc(sizeof(Assigment_op));
  assigment->recv = recv;
  assigment->send = send;
  newBag->type = "assigment_op";
  newBag->assigment_op = assigment;
  return newBag;
}
/*
  新建函数调用op
*/
Bag *createFunctionCall(char *name, ArgumentList *list, Boolean isLocal) {
  Bag *newBag = (Bag*) malloc(sizeof(Bag));
  Function_call *call = (Function_call*) malloc(sizeof(Function_call));
  call->func_name = name;
  call->list = list;
  call->isLocal = isLocal;
  newBag->type = "function_call";
  newBag->function_call = call;
  return newBag;
}
/*
  新建while语句
*/
Bag *creatWhileStatement(Bag *state, Bag *block) {
  Bag *newBag = (Bag*) malloc(sizeof(Bag));
  While_statement *w = (While_statement*) malloc(sizeof(While_statement));
  w->state = state;
  w->block = block;
  newBag->type = "while_statement";
  newBag->while_statement = w;
  return newBag;
}
