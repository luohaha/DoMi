%{
  #include<stdio.h>
  #include<string.h>
  #include "domi.h"

  extern VarLink *head;
  void yyerror(const char* err) {
    fprintf(stderr, "error: %s\n", err);
  }

  int yywrap() {
    return 1;
  }

%}

%token ADD SUB MUL DIV EQUAL ASSIGN SEMICOLON BL BR SL SR QUOT INTEGER_M DOUBLE_M STRING_M
%union {
  int number;
  double d_number;
  char* string;
  Value* value;
  Node* node;
}
%token <number> INTEGER;
%token <d_number> DOUBLE;
%token <string>   VAL_NAME;
%token <node>  primary_exp high_expression expression;

%%

all:
    all
    |
    sentence
    ;
sentence:
    eval SEMICOLON
    ;
eval:
    assign_expression
    |
    function_expression
    ;
function_expression:
    VAL_NAME SL expression SR
    {
      //函数执行操作
      printf("%d\n", (int)$3);
    };
assign_expression:
    VAL_NAME ASSIGN expression
    {
      //变量赋值操作
    }
    |
    INTEGER_M VAL_NAME ASSIGN expression
    {
      //int变量初始化,并赋值
      Value *newValue = createVar($4, $2, TRUE, INTEGER);
      VarLink *newLink = (VarLink*)malloc(sizeof(VarLink));
      newLink->value = newValue;
      newLink->next = head->next;
      head->next = newLink;
    }
    |
    DOUBLE_M VAL_NAME ASSIGN expression
    {
      //double变量初始化，并赋值
      //int变量初始化,并赋值
      Value *newValue = createVar($4, $2, TRUE, DOUBLE);
      VarLink *newLink = (VarLink*)malloc(sizeof(VarLink));
      newLink->value = newValue;
      newLink->next = head->next;
      head->next = newLink;
    }
    |
    STRING_M VAL_NAME ASSIGN expression
    {
      //string变量初始化，并赋值
    }
    |
    INTEGER_M VAL_NAME
    {
      //int初始化
    }
    |
    DOUBLE_M VAL_NAME
    {
      //double初始化
    }
    |
    STRING_M VAL_NAME
    {
      //string初始化
    };
expression:
    high_expression
    |
    expression ADD primary_exp
    {
      Node *node1 = $1;
      Node *node2 = $2;
      Node *newNode = simpleComupter(node1, node2, '+');
      $$ = newNode;
    }
    |
    expression SUB primary_exp
    {
      Node *node1 = $1;
      Node *node2 = $2;
      Node *newNode = simpleComupter(node1, node2, '-');
      $$ = newNode;
    }
    ;
high_expression:
    primary_exp
    |
    high_expression MUL primary_exp
    {
      Node *node1 = $1;
      Node *node2 = $2;
      Node *newNode = simpleComupter(node1, node2, '*');
      $$ = newNode;
    }
    |
    high_expression DIV primary_exp
    {
      Node *node1 = $1;
      Node *node2 = $2;
      Node *newNode = simpleComupter(node1, node2, '/');
      $$ = newNode;
    }
    ;
primary_exp:
    DOUBLE
    {
      Node *node = (Node*)malloc(sizeof(Node));
      (*node).doub = $1;
      (*node).type = DOUBLE;
      $$ = node;
    }
    |
    INTEGER
    {
      Node *node = (Node*)malloc(sizeof(Node));
      (*node).integer = $1;
      (*node).type = INTEGER;
      $$ = node;
    }
    ;
