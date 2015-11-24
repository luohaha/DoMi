#include "domi.h"
/*
  author : Yixin Luo
  负责与变量相关的操作
*/
extern Manager *manager;
VarLink *findVar(char *name) {
  VarLink *p = manager->varhead->next;
  while (p != NULL) {
    if (strcmp(p->value->varname, name) == 0) {
      //相等
      return p;
    }
    p = p->next;
  }
  //未找到
  return NULL;
}

/*
  判断变量是否为true false
*/
int isTrue(Bag *bag) {
  exeTree(bag);
    if (bag->node->type == INTEGER_TYPE) {
      if (bag->node->integer == 0)
	return 0;
      else
	return 1;
    } else if (bag->node->type == DOUBLE_TYPE) {
      if (bag->node->doub == 0.0)
	return 0;
      else
	return 1;
    } else {
      return 1;
    }
}
/*
  将语句bag加入对应的链表中
*/
void bagJoinLink(Bag *bag, BagLink *head) {
  if (bag != NULL) {
    BagLink *newLink = (BagLink*) malloc(sizeof(BagLink));
    newLink->bag = bag;
    newLink->prev = head->prev;
    newLink->next = head;
    head->prev->next = newLink;
    head->prev = newLink;
  }
}
