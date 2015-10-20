#include "domi.h"

/*
  返回值为0，说明现在两个值都为int
  返回值为1，说明现在两个值都为double
  返回值为－1，出错！！！
*/
int computerType(Node *node1, Node *node2) {
  if ((*node1).type == INTEGER && (*node2).type == DOUBLE) {
    node1->type = DOUBLE;
    node1->doub = (double)node1->integer;
    return 1;
  } else if ((*node1).type == DOUBLE && (*node2).type == INTEGER) {
    node2->type = DOUBLE;
    node2->doub = (double)node2->integer;
    return 1;
  } else if ((*node1).type == INTEGER && (*node2).type == INTEGER) {
    return 0;
  } else if ((*node1).type == DOUBLE && (*node2).type == DOUBLE) {
    return 1;
  } else {
    fprintf(stderr, "数据类型不适合此运算!\n");
    return -1;
  }
}

Node *simpleComputer(Node *node1, Node *node2, char type) {
  Node *newNode = (Node*) malloc(sizeof(Node));
  if (type == '+') {
    if (computerType(node1, node2) == 0) {
      newNode->type = INTEGER;
      newNode->integer = node1->integer + node2->integer;
    } else if (computerType(node1, node2) == 1) {
      newNode->type = DOUBLE;
      newNode->doub = node1->doub + node2->doub;
    }
  } else if (type == '-') {
    if (computerType(node1, node2) == 0) {
      newNode->type = INTEGER;
      newNode->integer = node1->integer － node2->integer;
    } else if (computerType(node1, node2) == 1) {
      newNode->type = DOUBLE;
      newNode->doub = node1->doub － node2->doub;
    }
  } else if (type == '*') {
    if (computerType(node1, node2) == 0) {
      newNode->type = INTEGER;
      newNode->integer = node1->integer * node2->integer;
    } else if (computerType(node1, node2) == 1) {
      newNode->type = DOUBLE;
      newNode->doub = node1->doub * node2->doub;
    }
  } else if (type == '/') {
    if (computerType(node1, node2) == 0) {
      newNode->type = INTEGER;
      newNode->integer = node1->integer / node2->integer;
    } else if (computerType(node1, node2) == 1) {
      newNode->type = DOUBLE;
      newNode->doub = node1->doub / node2->doub;
    }
  } else {
    fprintf(stderr, "type err\n");
  }
  free(node1);
  free(node2);
  return newNode;
}

