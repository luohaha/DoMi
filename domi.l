%{
#include<stdio.h>
#include "y.tab.h"
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

"int"        return INTEGER_M;
"double"     return DOUBLE_M;
"string"     return STRING_M;

[1-9][0-9]*  yylval.number = atoi(yytext);return INTEGER;
(0\.[0-9]+)|([1-9][0-9]*\.[0-9]+)   yylval.d_number = strtod(yytext); return DOUBLE;
[_a-zA-Z][_a-zA-Z0-9]+        yylval.string = strdup(yytext);return VAL_NAME;
"\n"         ;
[ \t]        ;
%%
