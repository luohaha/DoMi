/* A Bison parser, made by GNU Bison 3.0.2.  */

/* Bison interface for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2013 Free Software Foundation, Inc.

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.  */

/* As a special exception, you may create a larger work that contains
   part or all of the Bison parser skeleton and distribute that work
   under terms of your choice, so long as that work isn't itself a
   parser generator using the skeleton or a modified version thereof
   as a parser skeleton.  Alternatively, if you modify or redistribute
   the parser skeleton itself, you may (at your option) remove this
   special exception, which will cause the skeleton and the resulting
   Bison output files to be licensed under the GNU General Public
   License without this special exception.

   This special exception was added by the Free Software Foundation in
   version 2.2 of Bison.  */

#ifndef YY_YY_Y_TAB_H_INCLUDED
# define YY_YY_Y_TAB_H_INCLUDED
/* Debug traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif
#if YYDEBUG
extern int yydebug;
#endif

/* Token type.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
  enum yytokentype
  {
    INT_LITERAL = 258,
    DOUBLE_LITERAL = 259,
    STRING_LITERAL = 260,
    IDENTIFIER = 261,
    FUNCTION = 262,
    IF = 263,
    ELSE = 264,
    ELSIF = 265,
    WHILE = 266,
    FOR = 267,
    RETURN_T = 268,
    BREAK = 269,
    CONTINUE = 270,
    NULL_T = 271,
    LP = 272,
    RP = 273,
    LC = 274,
    RC = 275,
    SEMICOLON = 276,
    COMMA = 277,
    ASSIGN = 278,
    LOGICAL_AND = 279,
    LOGICAL_OR = 280,
    EQ = 281,
    NE = 282,
    GT = 283,
    GE = 284,
    LT = 285,
    LE = 286,
    ADD = 287,
    SUB = 288,
    MUL = 289,
    DIV = 290,
    MOD = 291,
    TRUE_T = 292,
    FALSE_T = 293,
    GLOBAL_T = 294
  };
#endif
/* Tokens.  */
#define INT_LITERAL 258
#define DOUBLE_LITERAL 259
#define STRING_LITERAL 260
#define IDENTIFIER 261
#define FUNCTION 262
#define IF 263
#define ELSE 264
#define ELSIF 265
#define WHILE 266
#define FOR 267
#define RETURN_T 268
#define BREAK 269
#define CONTINUE 270
#define NULL_T 271
#define LP 272
#define RP 273
#define LC 274
#define RC 275
#define SEMICOLON 276
#define COMMA 277
#define ASSIGN 278
#define LOGICAL_AND 279
#define LOGICAL_OR 280
#define EQ 281
#define NE 282
#define GT 283
#define GE 284
#define LT 285
#define LE 286
#define ADD 287
#define SUB 288
#define MUL 289
#define DIV 290
#define MOD 291
#define TRUE_T 292
#define FALSE_T 293
#define GLOBAL_T 294

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef union YYSTYPE YYSTYPE;
union YYSTYPE
{
#line 11 "domi.y" /* yacc.c:1909  */


    char                *identifier;

    ParameterList       *parameter_list;

    ArgumentList        *argument_list;

    Expression          *expression;

    Statement           *statement;

    StatementList       *statement_list;

    Block               *block;

    Elsif               *elsif;

    IdentifierList      *identifier_list;


#line 154 "y.tab.h" /* yacc.c:1909  */
};
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE yylval;

int yyparse (void);

#endif /* !YY_YY_Y_TAB_H_INCLUDED  */
