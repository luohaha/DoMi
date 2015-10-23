%{
  #include<stdio.h>
  #include<string.h>
  #include "domi.h"

  //extern VarLink *head;
  extern Manager *manager;
  extern char* yytext;
  extern int yylineno;
  int yydebug = 1;
  void yyerror(const char* err) {
    fprintf(stderr, "line: %d   error: %s   before:%s\n", yylineno, err, yytext);
  }
  
  int yywrap() {
    return 1;
  }
%}
%union {
  int number;
  double d_number;
  char* string;
  struct Bag_t *bag;
  struct ArgumentList_t *argument_list;
}
%token ADD SUB MUL DIV EQUAL ASSIGN SEMICOLON BL BR SL SR QUOT INTEGER_M DOUBLE_M STRING_M COMMA FUNCTION_M WHILE_M BIG_EQL SMALL_EQL BIG SMALL
%token <number> INTEGER;
%token <d_number> DOUBLE;
%token <string>   VAL_NAME;
%type <bag> primary_exp high_expression expression argument assign_expression function_expression eval compa_expression while_expression block while_sentence;
%type <argument_list> argument_list;
%%

all:
    sentence
    |
    all sentence
    ;
block:
    while_sentence
    |
    block while_sentence
    ;
while_sentence:
    eval SEMICOLON
    {
      /*加入控制语句的语句块block*/
      bagJoinLink($1, whileManager->baghead);
      $$ = whileManager->baghead->next->bag;
    }
    |
    while_sentence
    ;
sentence:
    eval SEMICOLON
    {
      /*加入语句链表*/
      bagJoinLink($1, manager->baghead);
    }
    |
    sentence
    ;
eval:
    assign_expression
    |
    function_expression
    |
    while_expression
    ;
while_expression:
    WHILE_M SL expression SR BL block BR
    {
      //while语句的实现
      While_statement *w = (While_statement*)malloc(sizeof(While_statement));
      Bag *bag = (Bag*)malloc(sizeof(Bag));
      w->state = $3;
      w->block = $6;
      bag->type = "while_statement";
      bag->while_statement = w;
      $$ = bag;
    }
    ;
function_expression:
    VAL_NAME SL argument_list SR
    {
      //函数执行操作
      //local_func($1, $3);
      $$ = createFunctionCall($1, $3, TRUE);
    };
assign_expression:
    VAL_NAME ASSIGN expression
    {
      //变量赋值操作
      VarLink *get = (VarLink*)findVar($1);
      if (get != NULL) {
	get->value->isGivedValue = TRUE;
	$$ = createAssigmentOp(get->value, $3);
      } else {
	fprintf(stderr, "变量未定\n");
	exit(1);
      }
    }
    |
    INTEGER_M VAL_NAME ASSIGN expression
    {
      //int变量初始化,并赋值
      Value *newValue = createVar($2, TRUE, INTEGER_TYPE);
      VarLink *newLink = (VarLink*)malloc(sizeof(VarLink));
      newLink->value = newValue;
      newLink->next = manager->varhead->next;
      manager->varhead->next = newLink;

      $$ = createAssigmentOp(newValue, $4);
    }
    |
    DOUBLE_M VAL_NAME ASSIGN expression
    {
      //double变量初始化，并赋值
      
      Value *newValue = createVar($2, TRUE, DOUBLE_TYPE);
      VarLink *newLink = (VarLink*)malloc(sizeof(VarLink));
      newLink->value = newValue;
      newLink->next = manager->varhead->next;
      manager->varhead->next = newLink;

      $$ = createAssigmentOp(newValue, $4);
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
      Value *newValue = createVar($2, FALSE, INTEGER_TYPE);
      VarLink *newLink = (VarLink*)malloc(sizeof(VarLink));
      newLink->value = newValue;
      newLink->next = manager->varhead->next;
      manager->varhead->next = newLink;
    }
    |
    DOUBLE_M VAL_NAME
    {
      //double初始化
      Value *newValue = createVar($2, FALSE, DOUBLE_TYPE);
      VarLink *newLink = (VarLink*)malloc(sizeof(VarLink));
      newLink->value = newValue;
      newLink->next = manager->varhead->next;
      manager->varhead->next = newLink;
    }
    |
    STRING_M VAL_NAME
    {
      //string初始化
    };
argument:
    VAL_NAME
    {
      Value *value;
      VarLink *link = (VarLink*)findVar($1);
      if (link == NULL) {
	fprintf(stderr, "找不到变量\n");
        exit(1);
      } else {
	value = link->value;
      }
      Bag *bag = (Bag*) malloc(sizeof(Bag));
      bag->type = "value";
      bag->value = value;
      $$ = bag;
    }
    ;
argument_list:
    expression
    {
      $$ = createArgumentList($1);
    }
    |
    argument_list COMMA expression
    {
      insertIntoArgumentList($1, $3);
    }
    ;
expression:
    high_expression
    |
    expression ADD high_expression
    {
      $$ = createBinaryOp($1, $3, '+');
    }
    |
    expression SUB high_expression
    {
      $$ = createBinaryOp($1, $3, '-');
    }
    ;
high_expression:
    compa_expression
    |
    high_expression MUL compa_expression
    {
      $$ = createBinaryOp($1, $3, '*');
    }
    |
    high_expression DIV compa_expression
    {
      $$ = createBinaryOp($1, $3, '/');
    }
    ;
compa_expression:
    primary_exp
    |
    compa_expression EQUAL primary_exp
    {
      $$ = createBinaryOp($1, $3, 'e');
    }
    |
    compa_expression BIG_EQL primary_exp
    {
      $$ = createBinaryOp($1, $3, '1');
    }
    |
    compa_expression SMALL_EQL primary_exp
    {
      $$ = createBinaryOp($1, $3, '2');
    }
    |
    compa_expression BIG primary_exp
    {
      $$ = createBinaryOp($1, $3, 'b');
    }
    |
    compa_expression SMALL primary_exp
    {
      $$ = createBinaryOp($1, $3, 's');
    };
primary_exp:
    DOUBLE
    {
      Node *node = (Node*)malloc(sizeof(Node));
      (*node).doub = $1;
      (*node).type = DOUBLE_TYPE;
      Bag *bag = (Bag*) malloc(sizeof(Bag));
      bag->type = "node";
      bag->node = node;
      $$ = bag;
    }
    |
    INTEGER
    {
      Node *node = (Node*)malloc(sizeof(Node));
      (*node).integer = $1;
      (*node).type = INTEGER_TYPE;
      Bag *bag = (Bag*) malloc(sizeof(Bag));
      bag->type = "node";
      bag->node = node;
      $$ = bag;
    }
    |
    argument
    ;
