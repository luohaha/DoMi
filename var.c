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
