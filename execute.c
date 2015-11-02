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
    VarLink *head = p->manager->varhead;
    while (args != NULL && head != NULL) {
      exeBag(args->bag);
      head->value->node = args->bag->node;
      head->value->isGivedValue = TRUE;
      args = args->next;
      head = head->next;
    }
    if (args != NULL || head != NULL) {
      fprintf(stderr, "实际参数不正确\n");
      exit(-1);
    }
    exeFuncBagLink(p->manager->baghead, p);
  }
  return NULL;
}


/*
  执行函数语句
*/
void exeFuncBag(Bag *bag, ManagerLink *p) {
  printf("%s\n", bag->type);
  if (strcmp(bag->type, "value") == 0) {
    
    if (bag->value->node == NULL) {
      VarLink *link = p->manager->varhead;
      while(link != NULL) {
	if (strcmp(link->value->varname, bag->value->varname) == 0) {
	  //find
	  bag->value->node = link->value->node;
	  break;
	} else {
	  link = link->next;
	}
      }//while
      if (link == NULL) {
	//not found
	fprintf(stderr, "变量未定义 : %s\n", bag->value->varname);
	exit(-1);
      }
      //found
      bag->node = bag->value->node;
      return;
    }
  } else if (strcmp(bag->type, "assigment_op") == 0) {
    exeFuncTree(bag->assigment_op->send, p);
    bag->assigment_op->recv->node = bag->assigment_op->send->node;
  } else if (strcmp(bag->type, "function_call") == 0) {
    exeFuncTree(bag->function_call->list->bag, p);
    exeFunc(bag->function_call);
  } else {
     exeBag(bag);
  }
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
    exeTree(bag->function_call->list->bag);
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
  执行函数内部的语法树的解析
*/
void exeFuncTree(Bag *bag, ManagerLink *p) {
  /*后续遍历*/
  if (strcmp(bag->type, "binary_op") == 0) {
    exeFuncTree(bag->binary_op->left, p);
    exeFuncTree(bag->binary_op->right, p);
  }
  exeFuncBag(bag, p);
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
执行函数的语句链表
*/
void exeFuncBagLink(BagLink *head, ManagerLink *link) {
  BagLink *p = head;
  exeFuncBag(p->bag, link);
  p = p->next;
  if (p == NULL)
    printf("yes\n");
  while (p != head) {
    //exeFuncBag(p->bag, link);
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
