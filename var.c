#include "domi.h"
/*
  author : Yixin Luo
  负责与变量相关的操作
*/
extern VarLink *head;
VarLink *findVar(char *name) {
  VarLink *p = head;
  while (p != NULL) {
    if (strcmp(p->value->varname, name) == 0) {
      //相等
      return p;
    }
    p++;
  }
  //未找到
  return NULL;
}
