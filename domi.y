%{
  #include<stdio.h>
  #include<string.h>
  #include "domi.h"

  extern VarLink *head;
  int yydebug = 1;
  void yyerror(const char* err) {
    fprintf(stderr, "error: %s\n", err);
  }
  
  int yywrap() {
    return 1;
  }
%}
%union {
  int number;
  double d_number;
  char* string;
  struct Value_t* value;
  struct Node_t* node;
}
%token ADD SUB MUL DIV EQUAL ASSIGN SEMICOLON BL BR SL SR QUOT INTEGER_M DOUBLE_M STRING_M
%token <number> INTEGER;
%token <d_number> DOUBLE;
%token <string>   VAL_NAME;
%type <node>  primary_exp high_expression expression;
%%

all:
    sentence
    |
    all
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
      Node *node = $3;
      if (node->type == INTEGER_TYPE) {
	printf("%d", node->integer);
      } else if (node->type == DOUBLE_TYPE) {
	printf("%f", node->doub);
      }
    };
assign_expression:
    VAL_NAME ASSIGN expression
    {
      //变量赋值操作
      Node *node = $3;
      VarLink *get = (VarLink*)findVar($1);
      if (get != NULL) {
	if (node->type != get->value->type) {
	  fprintf(stderr, "赋值时，类型不一致");
	} else {
	  if (node->type == INTEGER_TYPE) {
	    get->value->node->integer = node->integer;
	  } else if (node->type == DOUBLE_TYPE) {
	    get->value->node->doub = node->doub;
	  }
	}
      } else {
	fprintf(stderr, "变量未定\n");
      }
    }
    |
    INTEGER_M VAL_NAME ASSIGN expression
    {
      //int变量初始化,并赋值
      Value *newValue = createVar($4, $2, TRUE, INTEGER_TYPE);
      VarLink *newLink = (VarLink*)malloc(sizeof(VarLink));
      newLink->value = newValue;
      newLink->next = head->next;
      head->next = newLink;
    }
    |
    DOUBLE_M VAL_NAME ASSIGN expression
    {
      //double变量初始化，并赋值
      
      Value *newValue = createVar($4, $2, TRUE, DOUBLE_TYPE);
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
      Value *newValue = createVar(NULL, $2, FALSE, INTEGER_TYPE);
      VarLink *newLink = (VarLink*)malloc(sizeof(VarLink));
      newLink->value = newValue;
      newLink->next = head->next;
      head->next = newLink;
    }
    |
    DOUBLE_M VAL_NAME
    {
      //double初始化
      Value *newValue = createVar(NULL, $2, FALSE, DOUBLE_TYPE);
      VarLink *newLink = (VarLink*)malloc(sizeof(VarLink));
      newLink->value = newValue;
      newLink->next = head->next;
      head->next = newLink;
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
      Node *node2 = $3;
      Node *newNode = (Node*)simpleComputer(node1, node2, '+');
      $$ = newNode;
    }
    |
    expression SUB primary_exp
    {
      Node *node1 = $1;
      Node *node2 = $3;
      Node *newNode = (Node*)simpleComputer(node1, node2, '-');
      $$ = newNode;
    }
    ;
high_expression:
    primary_exp
    |
    high_expression MUL primary_exp
    {
      Node *node1 = $1;
      Node *node2 = $3;
      Node *newNode = (Node*)simpleComputer(node1, node2, '*');
      $$ = newNode;
    }
    |
    high_expression DIV primary_exp
    {
      Node *node1 = $1;
      Node *node2 = $3;
      Node *newNode = (Node*)simpleComputer(node1, node2, '/');
      $$ = newNode;
    }
    ;
primary_exp:
    DOUBLE
    {
      Node *node = (Node*)malloc(sizeof(Node));
      (*node).doub = $1;
      (*node).type = DOUBLE_TYPE;
      $$ = node;
    }
    |
    INTEGER
    {
      Node *node = (Node*)malloc(sizeof(Node));
      (*node).integer = $1;
      (*node).type = INTEGER_TYPE;
      $$ = node;
    }
    ;
