#include "domi.h"
/*
  author : Yixin Luo
  各种初始化的操作
*/
VarLink* init_var() {
  VarLink* head = (VarLink*) malloc(sizeof(VarLink));
  head->next = NULL;
  head->value = NULL;
  return head;
}

/*
  初始化语句链表
*/
BagLink *init_bag() {
  BagLink *head = (BagLink*) malloc(sizeof(BagLink));
  head->bag = NULL;
  head->next = head;
  head->prev = head;
  return head;
}
