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
