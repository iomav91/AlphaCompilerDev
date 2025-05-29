/* A Bison parser, made by GNU Bison 3.8.2.  */

/* Bison interface for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2015, 2018-2021 Free Software Foundation,
   Inc.

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <https://www.gnu.org/licenses/>.  */

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

/* DO NOT RELY ON FEATURES THAT ARE NOT DOCUMENTED in the manual,
   especially those whose name start with YY_ or yy_.  They are
   private implementation details that can be changed or removed.  */

#ifndef YY_YY_PARSER_H_INCLUDED
# define YY_YY_PARSER_H_INCLUDED
/* Debug traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif
#if YYDEBUG
extern int yydebug;
#endif
/* "%code requires" blocks.  */
#line 31 "parser.y"

    struct Call {
        expression* elist;
        int method;
        char* name;
    };

    struct Forprefix {
        unsigned test;
        unsigned enter;
    };

    struct Stmt {
        unsigned breaklist;
        unsigned contlist;
        unsigned returnlist;
    };

#line 68 "parser.h"

/* Token kinds.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
  enum yytokentype
  {
    YYEMPTY = -2,
    YYEOF = 0,                     /* "end of file"  */
    YYerror = 256,                 /* error  */
    YYUNDEF = 257,                 /* "invalid token"  */
    TOK_IF = 258,                  /* TOK_IF  */
    TOK_ELSE = 259,                /* TOK_ELSE  */
    TOK_WHILE = 260,               /* TOK_WHILE  */
    TOK_FOR = 261,                 /* TOK_FOR  */
    TOK_FUNCTION = 262,            /* TOK_FUNCTION  */
    TOK_RETURN = 263,              /* TOK_RETURN  */
    TOK_BREAK = 264,               /* TOK_BREAK  */
    TOK_CONTINUE = 265,            /* TOK_CONTINUE  */
    TOK_AND = 266,                 /* TOK_AND  */
    TOK_NOT = 267,                 /* TOK_NOT  */
    TOK_OR = 268,                  /* TOK_OR  */
    TOK_LOCAL = 269,               /* TOK_LOCAL  */
    TOK_TRUE = 270,                /* TOK_TRUE  */
    TOK_FALSE = 271,               /* TOK_FALSE  */
    TOK_NIL = 272,                 /* TOK_NIL  */
    TOK_ASSIGN = 273,              /* TOK_ASSIGN  */
    TOK_PLUS = 274,                /* TOK_PLUS  */
    TOK_MINUS = 275,               /* TOK_MINUS  */
    TOK_MULTIPLY = 276,            /* TOK_MULTIPLY  */
    TOK_DIVIDE = 277,              /* TOK_DIVIDE  */
    TOK_MODULO = 278,              /* TOK_MODULO  */
    TOK_EQUAL = 279,               /* TOK_EQUAL  */
    TOK_N_EQUAL = 280,             /* TOK_N_EQUAL  */
    TOK_DBL_PLUS = 281,            /* TOK_DBL_PLUS  */
    TOK_DBL_MINUS = 282,           /* TOK_DBL_MINUS  */
    TOK_GREATER = 283,             /* TOK_GREATER  */
    TOK_LESS = 284,                /* TOK_LESS  */
    TOK_GR_EQUAL = 285,            /* TOK_GR_EQUAL  */
    TOK_LS_EQUAL = 286,            /* TOK_LS_EQUAL  */
    TOK_L_CURLY_BR = 287,          /* TOK_L_CURLY_BR  */
    TOK_R_CURLY_BR = 288,          /* TOK_R_CURLY_BR  */
    TOK_L_BR = 289,                /* TOK_L_BR  */
    TOK_R_BR = 290,                /* TOK_R_BR  */
    TOK_L_PARENTH = 291,           /* TOK_L_PARENTH  */
    TOK_R_PARENTH = 292,           /* TOK_R_PARENTH  */
    TOK_SEMICOLON = 293,           /* TOK_SEMICOLON  */
    TOK_COMMA = 294,               /* TOK_COMMA  */
    TOK_COLON = 295,               /* TOK_COLON  */
    TOK_DBL_COLON = 296,           /* TOK_DBL_COLON  */
    TOK_DOT = 297,                 /* TOK_DOT  */
    TOK_DBL_DOT = 298,             /* TOK_DBL_DOT  */
    TOK_ID = 299,                  /* TOK_ID  */
    TOK_INT = 300,                 /* TOK_INT  */
    TOK_FLOAT = 301,               /* TOK_FLOAT  */
    TOK_STRING = 302,              /* TOK_STRING  */
    TOK_UMINUS = 303               /* TOK_UMINUS  */
  };
  typedef enum yytokentype yytoken_kind_t;
#endif

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
union YYSTYPE
{
#line 50 "parser.y"

    unsigned unsignedVal;
    int intVal;
    double floatVal;
    char* strVal;
    SymbolTableEntry* symbol;
    expression* expr;
    struct Call call_struct;
    struct Forprefix forprefix_struct;
    struct Stmt stmt_struct;
    indexed_elem* indexed;

#line 146 "parser.h"

};
typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE yylval;


int yyparse (void);


#endif /* !YY_YY_PARSER_H_INCLUDED  */
