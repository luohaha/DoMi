#include<stdio.h>
#include<stdlib.h>
#include<string.h>

typedef struct Bag_t Bag;

typedef enum Boolean_e {
   TRUE = 1,
   FALSE = 0
 } Boolean;

typedef enum ValueType_e {
  STRING_TYPE = 1,
  INTEGER_TYPE = 2,
  DOUBLE_TYPE = 3
}ValueType;

/*
  存储数据的真实值
*/
typedef struct Node_t {
  char *str;
  int   integer;
  double doub;
  ValueType type;
} Node;
/*
  存储变量
*/
typedef struct Value_t{
  char *varname; //变量名称
  Node *node;    //指向存储的值
  Boolean  isGivedValue; //是否被初始化
  ValueType type;
} Value;

/*
  变量的链表，把变量当作单链表来存储
*/
typedef struct VarLink_t {
  Value *value;
  struct VarLink_t *next;
} VarLink;

/*
  实际参数的链表
*/
typedef struct ArgumentList_t{
  Bag *bag;
  struct ArgumentList_t *next;
} ArgumentList;

/*
  二元运算的数据结构，比如＋－＊／ == >= <= > <
*/
typedef struct Binary_op_t {
  Bag *left;
  Bag *right;
  char type;//运算类型,'-' '+' '*' '/' 'e' '1' '2' 'b' 's'
} Binary_op;

typedef struct Assigment_op_t {
  Value *recv; // 被赋值
  Bag *send; // 赋值者
} Assigment_op;

typedef struct Function_call_t {
  char *func_name; // 函数名
  ArgumentList *list; // 指向实际参数的链表
  Boolean isLocal;
} Function_call;

typedef struct While_statement_t {
  Bag *state;  //判断语句
  Bag *block; //语句链表
} While_statement;
/*
  所有可以被连入二元树的节点类型
*/
struct Bag_t {
  char *type;
  Node *node;
  Value *value;
  Binary_op *binary_op;
  Assigment_op *assigment_op;
  Function_call *function_call;
  While_statement *while_statement;
};
/*
  语句的链表
*/
typedef struct BagLink_t {
  Bag *bag;
  struct BagLink_t *next;
  struct BagLink_t *prev;
} BagLink;
/*
  管理员，负责管理语句，全局变量和函数定义
*/
typedef struct Manager_t{
  BagLink *baghead;
  VarLink *varhead;
}Manager;
/*
  全局变量的链表
*/
extern Manager *manager;
extern Manager *whileManager;

Value* createVar(char *name, Boolean isGive, ValueType type);
Node *simpleComputer(Node *node1, Node *node2, char type);
VarLink* init_var();
ArgumentList *createArgumentList(Bag *head);
void insertIntoArgumentList(ArgumentList *head, Bag *bag);
void local_func(char *varname, ArgumentList *head);
VarLink *findVar(char *name);

Bag *createBinaryOp(Bag *bag1, Bag *bag2, char type);
Bag *createAssigmentOp(Value *recv, Bag *send);
Bag *createFunctionCall(char *name, ArgumentList *list, Boolean isLocal);
BagLink *init_bag();
void exeBag(Bag *bag);
void exeTree(Bag *bag);
void exeBagLink(BagLink *head);
void bagJoinLink(Bag *bag, BagLink *head);
int isTrue(Bag *bag);
BagLink *freeBagLink(BagLink *head);
