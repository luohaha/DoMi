%{
  #include<stdio.h>
  #include<string.h>

  void yyerror(const char* err) {
    fprintf(stderr, "error: %s\n", err);
  }

  int yywrap() {
    return 1;
  }

  void main() {
    yyparse();
  }
%}

%token ADD SUB MUL DIV EQUAL ASSIGN SEMICOLON BL BR SL SR QUOT INTEGER_M DOUBLE_M STRING_M
%union {
  int number;
  double d_number;
  char* string;
}
%token <number> INTEGER;
%token <d_number> DOUBLE;
%token <string>   VAL_NAME;

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
    };
assign_expression:
    VAL_NAME ASSIGN expression
    {
      //变量赋值操作
    }
    ｜
    INTEGER_M VAL_NAME ASSIGN expression
    {
      //int变量初始化,并赋值
    }
    |
    DOUBLE_M VAL_NAME ASSIGN expression
    {
      //double变量初始化，并赋值
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
    ｜
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
      $$ = $1 + $3;
    }
    |
    expression SUB primary_exp
    {
      $$ = $1 - $3;
    }
    ;
high_expression:
    primary_exp
    |
    high_expression MUL primary_exp
    {
      $$ = $1 * $3;
    }
    |
    high_expression DIV primary_exp
    {
      $$ = $1 / $3;
    }
    ;
primary_exp:
    double_exp
    |
    integer_exp
    |
    string_exp
    ;
double_exp:
    DOUBLE
    {
      $$ = $1;
    }
;
integer_exp:
    INTEGER
    {
      $$ = $1;
    }
;
string_exp:
    STRING
    {
      $$ = $1;
    }
;
