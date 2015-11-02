#include<stdio.h>
#include<stdlib.h>
#include "domi.h"

/*
  author : Yixin Luo
  语言自带的函数实现
*/

/*
  打印函数
*/
void out(Node *node) {
  if (node->type == INTEGER_TYPE) {
    printf("%d\n", node->integer);
  } else if (node->type == DOUBLE_TYPE) {
    printf("%f\n", node->doub);
  } else if (node->type == STRING_TYPE) {

  }
}

void local_func(char *varname, ArgumentList *head) {
  if (strcmp(varname, "out") == 0) {
    //exeTree(head->bag);
    out(head->bag->node);
  } else {
    fprintf(stderr, "函数不存在\n");
  }
}
