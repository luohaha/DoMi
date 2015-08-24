%{

#include <stdio.h>

#include "domi.h"

#define YYDEBUG 1

%}

%union {

    char                *identifier;

    ParameterList       *parameter_list;

    ArgumentList        *argument_list;

    Expression          *expression;

    Statement           *statement;

    StatementList       *statement_list;

    Block               *block;

    Elsif               *elsif;

    IdentifierList      *identifier_list;

}

%token <expression>     INT_LITERAL

%token <expression>     DOUBLE_LITERAL

%token <expression>     STRING_LITERAL

%token <identifier>     IDENTIFIER

%token FUNCTION IF ELSE ELSIF WHILE FOR RETURN_T BREAK CONTINUE NULL_T

        LP RP LC RC SEMICOLON COMMA ASSIGN LOGICAL_AND LOGICAL_OR

        EQ NE GT GE LT LE ADD SUB MUL DIV MOD TRUE_T FALSE_T GLOBAL_T DOT INCREMENT DECREMENT

%type   <parameter_list> parameter_list

%type   <argument_list> argument_list

%type   <expression> expression expression_opt

        logical_and_expression logical_or_expression

        equality_expression relational_expression

        additive_expression multiplicative_expression

        unary_expression primary_expression

%type   <statement> statement global_statement

        if_statement while_statement for_statement

        return_statement break_statement continue_statement

%type   <statement_list> statement_list

%type   <block> block

%type   <elsif> elsif elsif_list

%type   <identifier_list> identifier_list

%%

translation_unit

        : definition_or_statement

        | translation_unit definition_or_statement

        ;

definition_or_statement

        : function_definition

        | statement

        {

            DM_Interpreter *inter = dm_get_current_interpreter();



            inter->statement_list

                = dm_chain_statement_list(inter->statement_list, $1);

        }

        ;

function_definition

        : FUNCTION IDENTIFIER LP parameter_list RP block

        {

            dm_function_define($2, $4, $6);

        }

        | FUNCTION IDENTIFIER LP RP block

        {

            dm_function_define($2, NULL, $5);

        }

        ;

parameter_list

        : IDENTIFIER

        {

            $$ = dm_create_parameter($1);

        }

        | parameter_list COMMA IDENTIFIER

        {

            $$ = dm_chain_parameter($1, $3);

        }

        ;

argument_list

        : expression

        {

            $$ = dm_create_argument_list($1);

        }

        | argument_list COMMA expression

        {

            $$ = dm_chain_argument_list($1, $3);

        }

        ;

statement_list

        : statement

        {

            $$ = dm_create_statement_list($1);

        }

        | statement_list statement

        {

            $$ = dm_chain_statement_list($1, $2);

        }

        ;

expression

        : logical_or_expression

        | IDENTIFIER ASSIGN expression

        {

            $$ = dm_create_assign_expression($1, $3);

        }

        ;

logical_or_expression

        : logical_and_expression

        | logical_or_expression LOGICAL_OR logical_and_expression

        {

            $$ = dm_create_binary_expression(LOGICAL_OR_EXPRESSION, $1, $3);

        }

        ;

logical_and_expression

        : equality_expression

        | logical_and_expression LOGICAL_AND equality_expression

        {

            $$ = dm_create_binary_expression(LOGICAL_AND_EXPRESSION, $1, $3);

        }

        ;

equality_expression

        : relational_expression

        | equality_expression EQ relational_expression

        {

            $$ = dm_create_binary_expression(EQ_EXPRESSION, $1, $3);

        }

        | equality_expression NE relational_expression

        {

            $$ = dm_create_binary_expression(NE_EXPRESSION, $1, $3);

        }

        ;

relational_expression

        : additive_expression

        | relational_expression GT additive_expression

        {

            $$ = dm_create_binary_expression(GT_EXPRESSION, $1, $3);

        }

        | relational_expression GE additive_expression

        {

            $$ = dm_create_binary_expression(GE_EXPRESSION, $1, $3);

        }

        | relational_expression LT additive_expression

        {

            $$ = dm_create_binary_expression(LT_EXPRESSION, $1, $3);

        }

        | relational_expression LE additive_expression

        {

            $$ = dm_create_binary_expression(LE_EXPRESSION, $1, $3);

        }

        ;

additive_expression

        : multiplicative_expression

        | additive_expression ADD multiplicative_expression

        {

            $$ = dm_create_binary_expression(ADD_EXPRESSION, $1, $3);

        }

        | additive_expression SUB multiplicative_expression

        {

            $$ = dm_create_binary_expression(SUB_EXPRESSION, $1, $3);

        }

        ;

multiplicative_expression

        : unary_expression

        | multiplicative_expression MUL unary_expression

        {

            $$ = dm_create_binary_expression(MUL_EXPRESSION, $1, $3);

        }

        | multiplicative_expression DIV unary_expression

        {

            $$ = dm_create_binary_expression(DIV_EXPRESSION, $1, $3);

        }

        | multiplicative_expression MOD unary_expression

        {

            $$ = dm_create_binary_expression(MOD_EXPRESSION, $1, $3);

        }

        ;

unary_expression

        : primary_expression

        | SUB unary_expression

        {

            $$ = dm_create_minus_expression($2);

        }

        ;

postfix_expression
        : primary_expression

        {

        }

        | postfix_expression LB expression RB

        {

        }

        | postfix_expression DOT IDENTIFIER LP argument_lisp RP

        {

        }

        | postfix_expression DOT IDENTIFIER LP RP

        {

        }

        | postfix_expression INCREMENT

        {


        }

        | postfix_expression DECREMENT

        {

        }
        ;

primary_expression

        : IDENTIFIER LP argument_list RP

        {

            $$ = dm_create_function_call_expression($1, $3);

        }

        | IDENTIFIER LP RP

        {

            $$ = dm_create_function_call_expression($1, NULL);

        }

        | LP expression RP

        {

            $$ = $2;

        }

        | IDENTIFIER

        {

            $$ = dm_create_identifier_expression($1);

        }

        | INT_LITERAL

        | DOUBLE_LITERAL

        | STRING_LITERAL

        | TRUE_T

        {

            $$ = dm_create_boolean_expression(DM_TRUE);

        }

        | FALSE_T

        {

            $$ = dm_create_boolean_expression(DM_FALSE);

        }

        | NULL_T

        {

            $$ = dm_create_null_expression();

        }

        ;

statement

        : expression SEMICOLON

        {

          $$ = dm_create_expression_statement($1);

        }

        | global_statement

        | if_statement

        | while_statement

        | for_statement

        | return_statement

        | break_statement

        | continue_statement

        ;

global_statement

        : GLOBAL_T identifier_list SEMICOLON

        {

            $$ = dm_create_global_statement($2);

        }

        ;

identifier_list

        : IDENTIFIER

        {

            $$ = dm_create_global_identifier($1);

        }

        | identifier_list COMMA IDENTIFIER

        {

            $$ = dm_chain_identifier($1, $3);

        }

        ;

if_statement

        : IF LP expression RP block

        {

            $$ = dm_create_if_statement($3, $5, NULL, NULL);

        }

        | IF LP expression RP block ELSE block

        {

            $$ = dm_create_if_statement($3, $5, NULL, $7);

        }

        | IF LP expression RP block elsif_list

        {

            $$ = dm_create_if_statement($3, $5, $6, NULL);

        }

        | IF LP expression RP block elsif_list ELSE block

        {

            $$ = dm_create_if_statement($3, $5, $6, $8);

        }

        ;

elsif_list

        : elsif

        | elsif_list elsif

        {

            $$ = dm_chain_elsif_list($1, $2);

        }

        ;

elsif

        : ELSIF LP expression RP block

        {

            $$ = dm_create_elsif($3, $5);

        }

        ;

while_statement

        : WHILE LP expression RP block

        {

            $$ = dm_create_while_statement($3, $5);

        }

        ;

for_statement

        : FOR LP expression_opt SEMICOLON expression_opt SEMICOLON

          expression_opt RP block

        {

            $$ = dm_create_for_statement($3, $5, $7, $9);

        }

        ;

expression_opt

        : /* empty */

        {

            $$ = NULL;

        }

        | expression

        ;

return_statement

        : RETURN_T expression_opt SEMICOLON

        {

            $$ = dm_create_return_statement($2);

        }

        ;

break_statement

        : BREAK SEMICOLON

        {

            $$ = dm_create_break_statement();

        }

        ;

continue_statement

        : CONTINUE SEMICOLON

        {

            $$ = dm_create_continue_statement();

        }

        ;

block

        : LC statement_list RC

        {

            $$ = dm_create_block($2);

        }

        | LC RC

        {

            $$ = dm_create_block(NULL);

        }

        ;

%%
