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
