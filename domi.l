%option yylineno
%{
#include<stdio.h>
#include "y.tab.h"
#include "domi.h"
%}

%%

"+"          return ADD;
"-"          return SUB;
"*"          return MUL;
"/"          return DIV;
"=="         return EQUAL;
"="          return ASSIGN;
";"          return SEMICOLON;
"{"          return BL;
"}"          return BR;
"("          return SL;
")"          return SR;
"\""         return QUOT;
","          return COMMA;
">="         return BIG_EQL;
"<="         return SMALL_EQL;
">"          return BIG;
"<"          return SMALL;

"int"        return INTEGER_M;
"double"     return DOUBLE_M;
"string"     return STRING_M;
"func"       return FUNCTION_M;
"while"      return WHILE_M;

([1-9][0-9]*)|0  yylval.number = atoi(yytext);return INTEGER;
(0\.[0-9]+)|([1-9][0-9]*\.[0-9]+)   yylval.d_number = strtod(yytext, NULL); return DOUBLE;
[_a-zA-Z][_a-zA-Z0-9]*        yylval.string = strdup(yytext);return VAL_NAME;
[ \t\r\n]+      ;
%%
