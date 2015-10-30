/* A Bison parser, made by GNU Bison 2.3.  */

/* Skeleton interface for Bison's Yacc-like parsers in C

   Copyright (C) 1984, 1989, 1990, 2000, 2001, 2002, 2003, 2004, 2005, 2006
   Free Software Foundation, Inc.

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2, or (at your option)
   any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 51 Franklin Street, Fifth Floor,
   Boston, MA 02110-1301, USA.  */

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

/* Tokens.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
   /* Put the tokens into the symbol table, so that GDB and other debuggers
      know about them.  */
   enum yytokentype {
     ADD = 258,
     SUB = 259,
     MUL = 260,
     DIV = 261,
     EQUAL = 262,
     ASSIGN = 263,
     SEMICOLON = 264,
     BL = 265,
     BR = 266,
     SL = 267,
     SR = 268,
     QUOT = 269,
     INTEGER_M = 270,
     DOUBLE_M = 271,
     STRING_M = 272,
     COMMA = 273,
     FUNCTION_M = 274,
     WHILE_M = 275,
     BIG_EQL = 276,
     SMALL_EQL = 277,
     BIG = 278,
     SMALL = 279,
     INTEGER = 280,
     DOUBLE = 281,
     VAL_NAME = 282
   };
#endif
/* Tokens.  */
#define ADD 258
#define SUB 259
#define MUL 260
#define DIV 261
#define EQUAL 262
#define ASSIGN 263
#define SEMICOLON 264
#define BL 265
#define BR 266
#define SL 267
#define SR 268
#define QUOT 269
#define INTEGER_M 270
#define DOUBLE_M 271
#define STRING_M 272
#define COMMA 273
#define FUNCTION_M 274
#define WHILE_M 275
#define BIG_EQL 276
#define SMALL_EQL 277
#define BIG 278
#define SMALL 279
#define INTEGER 280
#define DOUBLE 281
#define VAL_NAME 282




#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef union YYSTYPE
#line 19 "domi.y"
{
  int number;
  double d_number;
  char* string;
  struct Bag_t *bag;
  struct ArgumentList_t *argument_list;
}
/* Line 1529 of yacc.c.  */
#line 111 "y.tab.h"
	YYSTYPE;
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
# define YYSTYPE_IS_TRIVIAL 1
#endif

extern YYSTYPE yylval;

