#include "domi.h"

/*
  author : Yixin Luo
  主函数
*/

void main(int argc, char **argv) {
  //获取全局变量链表的头部
  VarLink *head = init_var();
  yyparse();
  return 0;
}
