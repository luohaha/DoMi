#include "domi.h"

extern Manager *manager;

/*
  执行while语句块
*/
void exeWhileStatement(Bag *bag) {
  while (isTrue(bag->while_statement->state)) {
    exeBagLink(whileManager->baghead);
  }
  freeBagLink(whileManager->baghead);
}
/*
  执行函数调用
*/
Bag* exeFunc(Function_call *call) {
  if (call->isLocal == TRUE) {
    local_func(call->func_name, call->list);
  } else {
    //执行自定义函数
    //首先查看是否已经定义
    ManagerLink *p = managerLink->next;
    while (p != NULL) {
      if (strcmp(p->name, call->func_name) == 0) {
	break;
      } else
	p = p->next;
    }
    if (p == NULL) {
      //找不到
      fprintf(stderr, "函数未定义: %s\n", call->func_name);
      exit(-1);
    }
    //找到
    ArgumentList *args = call->list;
    while (args != NULL) {
      exeBag(args->bag);
      
    }
  }
  return NULL;
}
/*
  执行每一条语句
*/
void exeBag(Bag *bag) {
  if (strcmp(bag->type, "binary_op") == 0) {
    bag->node = simpleComputer(bag->binary_op->left->node, bag->binary_op->right->node, bag->binary_op->type);
  } else if (strcmp(bag->type, "assigment_op") == 0) {
    exeTree(bag->assigment_op->send);
    bag->assigment_op->recv->node = bag->assigment_op->send->node;
  } else if (strcmp(bag->type, "function_call") == 0) {
    exeFunc(bag->function_call);
  } else if (strcmp(bag->type, "node") == 0) {

  } else if (strcmp(bag->type, "value") == 0) {
    if (bag->value->node == NULL) {
      fprintf(stderr, "变量未赋值\n");
      exit(1);
    }
    bag->node = bag->value->node;
  } else if (strcmp(bag->type, "while_statement") == 0) {
    exeWhileStatement(bag);
    
  } else {
    fprintf(stderr, "语句类型错误\n");
    exit(1);
  }
}
/*
  执行语法树的解析
*/
void exeTree(Bag *bag) {
  /*后续遍历*/
  if (strcmp(bag->type, "binary_op") == 0) {
    exeTree(bag->binary_op->left);
    exeTree(bag->binary_op->right);
  }
  exeBag(bag);
}

/*
  执行语句链表
*/
void exeBagLink(BagLink *head) {
  BagLink *p = head->next;
  while (p != head) {
    exeBag(p->bag);
    p = p->next;
  }
  return;
}

/*
  回收语句链表
*/
BagLink *freeBagLink(BagLink *head) {
  BagLink *p = head->next;
  while (p != head) {
    head->next = p->next;
    head->next->prev = head;
    if (p->bag != NULL)
      free(p->bag);
    if (p != NULL)
      free(p);
    p = head->next;
  }
  return head;
}
