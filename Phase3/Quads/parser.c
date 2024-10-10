/* A Bison parser, made by GNU Bison 3.8.2.  */

/* Bison implementation for Yacc-like parsers in C

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

/* C LALR(1) parser skeleton written by Richard Stallman, by
   simplifying the original so-called "semantic" parser.  */

/* DO NOT RELY ON FEATURES THAT ARE NOT DOCUMENTED in the manual,
   especially those whose name start with YY_ or yy_.  They are
   private implementation details that can be changed or removed.  */

/* All symbols defined below should begin with yy or YY, to avoid
   infringing on user name space.  This should be done even for local
   variables, as they might otherwise be expanded by user macros.
   There are some unavoidable exceptions within include files to
   define necessary library symbols; they are noted "INFRINGES ON
   USER NAME SPACE" below.  */

/* Identify Bison output, and Bison version.  */
#define YYBISON 30802

/* Bison version string.  */
#define YYBISON_VERSION "3.8.2"

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 0

/* Push parsers.  */
#define YYPUSH 0

/* Pull parsers.  */
#define YYPULL 1




/* First part of user prologue.  */
#line 1 "parser.y"

    #include "symbol_table.h"
    #include "handlers.h"
    #include "icode_managers.h"
    extern int yylex();
    extern void yyerror(const char *);
    extern int yyparse();
    extern int yylineno;
    extern FILE* yyin;
    //extern std::list<std::string> argsList;
    int scope_counter;
    //int scope_space_counter = 1;
    int is_in_function_mode = 0;
    int is_in_block_mode = 0;
    int is_a_member_access = 0;
    std::string name = "";

#line 89 "parser.c"

# ifndef YY_CAST
#  ifdef __cplusplus
#   define YY_CAST(Type, Val) static_cast<Type> (Val)
#   define YY_REINTERPRET_CAST(Type, Val) reinterpret_cast<Type> (Val)
#  else
#   define YY_CAST(Type, Val) ((Type) (Val))
#   define YY_REINTERPRET_CAST(Type, Val) ((Type) (Val))
#  endif
# endif
# ifndef YY_NULLPTR
#  if defined __cplusplus
#   if 201103L <= __cplusplus
#    define YY_NULLPTR nullptr
#   else
#    define YY_NULLPTR 0
#   endif
#  else
#   define YY_NULLPTR ((void*)0)
#  endif
# endif

#include "parser.h"
/* Symbol kind.  */
enum yysymbol_kind_t
{
  YYSYMBOL_YYEMPTY = -2,
  YYSYMBOL_YYEOF = 0,                      /* "end of file"  */
  YYSYMBOL_YYerror = 1,                    /* error  */
  YYSYMBOL_YYUNDEF = 2,                    /* "invalid token"  */
  YYSYMBOL_TOK_IF = 3,                     /* TOK_IF  */
  YYSYMBOL_TOK_ELSE = 4,                   /* TOK_ELSE  */
  YYSYMBOL_TOK_WHILE = 5,                  /* TOK_WHILE  */
  YYSYMBOL_TOK_FOR = 6,                    /* TOK_FOR  */
  YYSYMBOL_TOK_FUNCTION = 7,               /* TOK_FUNCTION  */
  YYSYMBOL_TOK_RETURN = 8,                 /* TOK_RETURN  */
  YYSYMBOL_TOK_BREAK = 9,                  /* TOK_BREAK  */
  YYSYMBOL_TOK_CONTINUE = 10,              /* TOK_CONTINUE  */
  YYSYMBOL_TOK_AND = 11,                   /* TOK_AND  */
  YYSYMBOL_TOK_NOT = 12,                   /* TOK_NOT  */
  YYSYMBOL_TOK_OR = 13,                    /* TOK_OR  */
  YYSYMBOL_TOK_LOCAL = 14,                 /* TOK_LOCAL  */
  YYSYMBOL_TOK_TRUE = 15,                  /* TOK_TRUE  */
  YYSYMBOL_TOK_FALSE = 16,                 /* TOK_FALSE  */
  YYSYMBOL_TOK_NIL = 17,                   /* TOK_NIL  */
  YYSYMBOL_TOK_ASSIGN = 18,                /* TOK_ASSIGN  */
  YYSYMBOL_TOK_PLUS = 19,                  /* TOK_PLUS  */
  YYSYMBOL_TOK_MINUS = 20,                 /* TOK_MINUS  */
  YYSYMBOL_TOK_MULTIPLY = 21,              /* TOK_MULTIPLY  */
  YYSYMBOL_TOK_DIVIDE = 22,                /* TOK_DIVIDE  */
  YYSYMBOL_TOK_MODULO = 23,                /* TOK_MODULO  */
  YYSYMBOL_TOK_EQUAL = 24,                 /* TOK_EQUAL  */
  YYSYMBOL_TOK_N_EQUAL = 25,               /* TOK_N_EQUAL  */
  YYSYMBOL_TOK_DBL_PLUS = 26,              /* TOK_DBL_PLUS  */
  YYSYMBOL_TOK_DBL_MINUS = 27,             /* TOK_DBL_MINUS  */
  YYSYMBOL_TOK_GREATER = 28,               /* TOK_GREATER  */
  YYSYMBOL_TOK_LESS = 29,                  /* TOK_LESS  */
  YYSYMBOL_TOK_GR_EQUAL = 30,              /* TOK_GR_EQUAL  */
  YYSYMBOL_TOK_LS_EQUAL = 31,              /* TOK_LS_EQUAL  */
  YYSYMBOL_TOK_L_CURLY_BR = 32,            /* TOK_L_CURLY_BR  */
  YYSYMBOL_TOK_R_CURLY_BR = 33,            /* TOK_R_CURLY_BR  */
  YYSYMBOL_TOK_L_BR = 34,                  /* TOK_L_BR  */
  YYSYMBOL_TOK_R_BR = 35,                  /* TOK_R_BR  */
  YYSYMBOL_TOK_L_PARENTH = 36,             /* TOK_L_PARENTH  */
  YYSYMBOL_TOK_R_PARENTH = 37,             /* TOK_R_PARENTH  */
  YYSYMBOL_TOK_SEMICOLON = 38,             /* TOK_SEMICOLON  */
  YYSYMBOL_TOK_COMMA = 39,                 /* TOK_COMMA  */
  YYSYMBOL_TOK_COLON = 40,                 /* TOK_COLON  */
  YYSYMBOL_TOK_DBL_COLON = 41,             /* TOK_DBL_COLON  */
  YYSYMBOL_TOK_DOT = 42,                   /* TOK_DOT  */
  YYSYMBOL_TOK_DBL_DOT = 43,               /* TOK_DBL_DOT  */
  YYSYMBOL_TOK_ID = 44,                    /* TOK_ID  */
  YYSYMBOL_TOK_INT = 45,                   /* TOK_INT  */
  YYSYMBOL_TOK_FLOAT = 46,                 /* TOK_FLOAT  */
  YYSYMBOL_TOK_STRING = 47,                /* TOK_STRING  */
  YYSYMBOL_TOK_UMINUS = 48,                /* TOK_UMINUS  */
  YYSYMBOL_YYACCEPT = 49,                  /* $accept  */
  YYSYMBOL_program = 50,                   /* program  */
  YYSYMBOL_statement = 51,                 /* statement  */
  YYSYMBOL_statements = 52,                /* statements  */
  YYSYMBOL_expression = 53,                /* expression  */
  YYSYMBOL_term = 54,                      /* term  */
  YYSYMBOL_assignexpr = 55,                /* assignexpr  */
  YYSYMBOL_primary = 56,                   /* primary  */
  YYSYMBOL_lvalue = 57,                    /* lvalue  */
  YYSYMBOL_member = 58,                    /* member  */
  YYSYMBOL_call = 59,                      /* call  */
  YYSYMBOL_callsuffix = 60,                /* callsuffix  */
  YYSYMBOL_normcall = 61,                  /* normcall  */
  YYSYMBOL_methodcall = 62,                /* methodcall  */
  YYSYMBOL_elist = 63,                     /* elist  */
  YYSYMBOL_indexed = 64,                   /* indexed  */
  YYSYMBOL_objectdef = 65,                 /* objectdef  */
  YYSYMBOL_indexedelem = 66,               /* indexedelem  */
  YYSYMBOL_block = 67,                     /* block  */
  YYSYMBOL_68_1 = 68,                      /* $@1  */
  YYSYMBOL_funcdef_block = 69,             /* funcdef_block  */
  YYSYMBOL_70_2 = 70,                      /* $@2  */
  YYSYMBOL_funcdef = 71,                   /* funcdef  */
  YYSYMBOL_72_3 = 72,                      /* $@3  */
  YYSYMBOL_73_4 = 73,                      /* $@4  */
  YYSYMBOL_const = 74,                     /* const  */
  YYSYMBOL_idlist = 75,                    /* idlist  */
  YYSYMBOL_if_statement = 76,              /* if_statement  */
  YYSYMBOL_while_statement = 77,           /* while_statement  */
  YYSYMBOL_for_statement = 78,             /* for_statement  */
  YYSYMBOL_return_statement = 79           /* return_statement  */
};
typedef enum yysymbol_kind_t yysymbol_kind_t;




#ifdef short
# undef short
#endif

/* On compilers that do not define __PTRDIFF_MAX__ etc., make sure
   <limits.h> and (if available) <stdint.h> are included
   so that the code can choose integer types of a good width.  */

#ifndef __PTRDIFF_MAX__
# include <limits.h> /* INFRINGES ON USER NAME SPACE */
# if defined __STDC_VERSION__ && 199901 <= __STDC_VERSION__
#  include <stdint.h> /* INFRINGES ON USER NAME SPACE */
#  define YY_STDINT_H
# endif
#endif

/* Narrow types that promote to a signed type and that can represent a
   signed or unsigned integer of at least N bits.  In tables they can
   save space and decrease cache pressure.  Promoting to a signed type
   helps avoid bugs in integer arithmetic.  */

#ifdef __INT_LEAST8_MAX__
typedef __INT_LEAST8_TYPE__ yytype_int8;
#elif defined YY_STDINT_H
typedef int_least8_t yytype_int8;
#else
typedef signed char yytype_int8;
#endif

#ifdef __INT_LEAST16_MAX__
typedef __INT_LEAST16_TYPE__ yytype_int16;
#elif defined YY_STDINT_H
typedef int_least16_t yytype_int16;
#else
typedef short yytype_int16;
#endif

/* Work around bug in HP-UX 11.23, which defines these macros
   incorrectly for preprocessor constants.  This workaround can likely
   be removed in 2023, as HPE has promised support for HP-UX 11.23
   (aka HP-UX 11i v2) only through the end of 2022; see Table 2 of
   <https://h20195.www2.hpe.com/V2/getpdf.aspx/4AA4-7673ENW.pdf>.  */
#ifdef __hpux
# undef UINT_LEAST8_MAX
# undef UINT_LEAST16_MAX
# define UINT_LEAST8_MAX 255
# define UINT_LEAST16_MAX 65535
#endif

#if defined __UINT_LEAST8_MAX__ && __UINT_LEAST8_MAX__ <= __INT_MAX__
typedef __UINT_LEAST8_TYPE__ yytype_uint8;
#elif (!defined __UINT_LEAST8_MAX__ && defined YY_STDINT_H \
       && UINT_LEAST8_MAX <= INT_MAX)
typedef uint_least8_t yytype_uint8;
#elif !defined __UINT_LEAST8_MAX__ && UCHAR_MAX <= INT_MAX
typedef unsigned char yytype_uint8;
#else
typedef short yytype_uint8;
#endif

#if defined __UINT_LEAST16_MAX__ && __UINT_LEAST16_MAX__ <= __INT_MAX__
typedef __UINT_LEAST16_TYPE__ yytype_uint16;
#elif (!defined __UINT_LEAST16_MAX__ && defined YY_STDINT_H \
       && UINT_LEAST16_MAX <= INT_MAX)
typedef uint_least16_t yytype_uint16;
#elif !defined __UINT_LEAST16_MAX__ && USHRT_MAX <= INT_MAX
typedef unsigned short yytype_uint16;
#else
typedef int yytype_uint16;
#endif

#ifndef YYPTRDIFF_T
# if defined __PTRDIFF_TYPE__ && defined __PTRDIFF_MAX__
#  define YYPTRDIFF_T __PTRDIFF_TYPE__
#  define YYPTRDIFF_MAXIMUM __PTRDIFF_MAX__
# elif defined PTRDIFF_MAX
#  ifndef ptrdiff_t
#   include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  endif
#  define YYPTRDIFF_T ptrdiff_t
#  define YYPTRDIFF_MAXIMUM PTRDIFF_MAX
# else
#  define YYPTRDIFF_T long
#  define YYPTRDIFF_MAXIMUM LONG_MAX
# endif
#endif

#ifndef YYSIZE_T
# ifdef __SIZE_TYPE__
#  define YYSIZE_T __SIZE_TYPE__
# elif defined size_t
#  define YYSIZE_T size_t
# elif defined __STDC_VERSION__ && 199901 <= __STDC_VERSION__
#  include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  define YYSIZE_T size_t
# else
#  define YYSIZE_T unsigned
# endif
#endif

#define YYSIZE_MAXIMUM                                  \
  YY_CAST (YYPTRDIFF_T,                                 \
           (YYPTRDIFF_MAXIMUM < YY_CAST (YYSIZE_T, -1)  \
            ? YYPTRDIFF_MAXIMUM                         \
            : YY_CAST (YYSIZE_T, -1)))

#define YYSIZEOF(X) YY_CAST (YYPTRDIFF_T, sizeof (X))


/* Stored state numbers (used for stacks). */
typedef yytype_uint8 yy_state_t;

/* State numbers in computations.  */
typedef int yy_state_fast_t;

#ifndef YY_
# if defined YYENABLE_NLS && YYENABLE_NLS
#  if ENABLE_NLS
#   include <libintl.h> /* INFRINGES ON USER NAME SPACE */
#   define YY_(Msgid) dgettext ("bison-runtime", Msgid)
#  endif
# endif
# ifndef YY_
#  define YY_(Msgid) Msgid
# endif
#endif


#ifndef YY_ATTRIBUTE_PURE
# if defined __GNUC__ && 2 < __GNUC__ + (96 <= __GNUC_MINOR__)
#  define YY_ATTRIBUTE_PURE __attribute__ ((__pure__))
# else
#  define YY_ATTRIBUTE_PURE
# endif
#endif

#ifndef YY_ATTRIBUTE_UNUSED
# if defined __GNUC__ && 2 < __GNUC__ + (7 <= __GNUC_MINOR__)
#  define YY_ATTRIBUTE_UNUSED __attribute__ ((__unused__))
# else
#  define YY_ATTRIBUTE_UNUSED
# endif
#endif

/* Suppress unused-variable warnings by "using" E.  */
#if ! defined lint || defined __GNUC__
# define YY_USE(E) ((void) (E))
#else
# define YY_USE(E) /* empty */
#endif

/* Suppress an incorrect diagnostic about yylval being uninitialized.  */
#if defined __GNUC__ && ! defined __ICC && 406 <= __GNUC__ * 100 + __GNUC_MINOR__
# if __GNUC__ * 100 + __GNUC_MINOR__ < 407
#  define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN                           \
    _Pragma ("GCC diagnostic push")                                     \
    _Pragma ("GCC diagnostic ignored \"-Wuninitialized\"")
# else
#  define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN                           \
    _Pragma ("GCC diagnostic push")                                     \
    _Pragma ("GCC diagnostic ignored \"-Wuninitialized\"")              \
    _Pragma ("GCC diagnostic ignored \"-Wmaybe-uninitialized\"")
# endif
# define YY_IGNORE_MAYBE_UNINITIALIZED_END      \
    _Pragma ("GCC diagnostic pop")
#else
# define YY_INITIAL_VALUE(Value) Value
#endif
#ifndef YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_END
#endif
#ifndef YY_INITIAL_VALUE
# define YY_INITIAL_VALUE(Value) /* Nothing. */
#endif

#if defined __cplusplus && defined __GNUC__ && ! defined __ICC && 6 <= __GNUC__
# define YY_IGNORE_USELESS_CAST_BEGIN                          \
    _Pragma ("GCC diagnostic push")                            \
    _Pragma ("GCC diagnostic ignored \"-Wuseless-cast\"")
# define YY_IGNORE_USELESS_CAST_END            \
    _Pragma ("GCC diagnostic pop")
#endif
#ifndef YY_IGNORE_USELESS_CAST_BEGIN
# define YY_IGNORE_USELESS_CAST_BEGIN
# define YY_IGNORE_USELESS_CAST_END
#endif


#define YY_ASSERT(E) ((void) (0 && (E)))

#if 1

/* The parser invokes alloca or malloc; define the necessary symbols.  */

# ifdef YYSTACK_ALLOC
   /* Pacify GCC's 'empty if-body' warning.  */
#  define YYSTACK_FREE(Ptr) do { /* empty */; } while (0)
#  ifndef YYSTACK_ALLOC_MAXIMUM
    /* The OS might guarantee only one guard page at the bottom of the stack,
       and a page size can be as small as 4096 bytes.  So we cannot safely
       invoke alloca (N) if N exceeds 4096.  Use a slightly smaller number
       to allow for a few compiler-allocated temporary stack slots.  */
#   define YYSTACK_ALLOC_MAXIMUM 4032 /* reasonable circa 2006 */
#  endif
# else
#  define YYSTACK_ALLOC YYMALLOC
#  define YYSTACK_FREE YYFREE
#  ifndef YYSTACK_ALLOC_MAXIMUM
#   define YYSTACK_ALLOC_MAXIMUM YYSIZE_MAXIMUM
#  endif
#  if (defined __cplusplus && ! defined EXIT_SUCCESS \
       && ! ((defined YYMALLOC || defined malloc) \
             && (defined YYFREE || defined free)))
#   include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#   ifndef EXIT_SUCCESS
#    define EXIT_SUCCESS 0
#   endif
#  endif
#  ifndef YYMALLOC
#   define YYMALLOC malloc
#   if ! defined malloc && ! defined EXIT_SUCCESS
void *malloc (YYSIZE_T); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
#  ifndef YYFREE
#   define YYFREE free
#   if ! defined free && ! defined EXIT_SUCCESS
void free (void *); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
# endif
# define YYCOPY_NEEDED 1
#endif /* 1 */

#if (! defined yyoverflow \
     && (! defined __cplusplus \
         || (defined YYSTYPE_IS_TRIVIAL && YYSTYPE_IS_TRIVIAL)))

/* A type that is properly aligned for any stack member.  */
union yyalloc
{
  yy_state_t yyss_alloc;
  YYSTYPE yyvs_alloc;
};

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_MAXIMUM (YYSIZEOF (union yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# define YYSTACK_BYTES(N) \
     ((N) * (YYSIZEOF (yy_state_t) + YYSIZEOF (YYSTYPE)) \
      + YYSTACK_GAP_MAXIMUM)

# define YYCOPY_NEEDED 1

/* Relocate STACK from its old location to the new one.  The
   local variables YYSIZE and YYSTACKSIZE give the old and new number of
   elements in the stack, and YYPTR gives the new location of the
   stack.  Advance YYPTR to a properly aligned location for the next
   stack.  */
# define YYSTACK_RELOCATE(Stack_alloc, Stack)                           \
    do                                                                  \
      {                                                                 \
        YYPTRDIFF_T yynewbytes;                                         \
        YYCOPY (&yyptr->Stack_alloc, Stack, yysize);                    \
        Stack = &yyptr->Stack_alloc;                                    \
        yynewbytes = yystacksize * YYSIZEOF (*Stack) + YYSTACK_GAP_MAXIMUM; \
        yyptr += yynewbytes / YYSIZEOF (*yyptr);                        \
      }                                                                 \
    while (0)

#endif

#if defined YYCOPY_NEEDED && YYCOPY_NEEDED
/* Copy COUNT objects from SRC to DST.  The source and destination do
   not overlap.  */
# ifndef YYCOPY
#  if defined __GNUC__ && 1 < __GNUC__
#   define YYCOPY(Dst, Src, Count) \
      __builtin_memcpy (Dst, Src, YY_CAST (YYSIZE_T, (Count)) * sizeof (*(Src)))
#  else
#   define YYCOPY(Dst, Src, Count)              \
      do                                        \
        {                                       \
          YYPTRDIFF_T yyi;                      \
          for (yyi = 0; yyi < (Count); yyi++)   \
            (Dst)[yyi] = (Src)[yyi];            \
        }                                       \
      while (0)
#  endif
# endif
#endif /* !YYCOPY_NEEDED */

/* YYFINAL -- State number of the termination state.  */
#define YYFINAL  3
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   604

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  49
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  31
/* YYNRULES -- Number of rules.  */
#define YYNRULES  91
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  172

/* YYMAXUTOK -- Last valid token kind.  */
#define YYMAXUTOK   303


/* YYTRANSLATE(TOKEN-NUM) -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex, with out-of-bounds checking.  */
#define YYTRANSLATE(YYX)                                \
  (0 <= (YYX) && (YYX) <= YYMAXUTOK                     \
   ? YY_CAST (yysymbol_kind_t, yytranslate[YYX])        \
   : YYSYMBOL_YYUNDEF)

/* YYTRANSLATE[TOKEN-NUM] -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex.  */
static const yytype_int8 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     1,     2,     3,     4,
       5,     6,     7,     8,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    22,    23,    24,
      25,    26,    27,    28,    29,    30,    31,    32,    33,    34,
      35,    36,    37,    38,    39,    40,    41,    42,    43,    44,
      45,    46,    47,    48
};

#if YYDEBUG
/* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_int16 yyrline[] =
{
       0,   123,   123,   124,   127,   128,   129,   130,   131,   132,
     133,   134,   135,   136,   137,   140,   141,   144,   145,   150,
     155,   160,   163,   178,   195,   212,   229,   246,   263,   280,
     284,   288,   295,   299,   312,   323,   348,   372,   397,   421,
     428,   469,   473,   474,   479,   485,   491,   502,   512,   522,
     530,   538,   549,   550,   555,   559,   587,   594,   598,   604,
     614,   624,   628,   632,   638,   642,   648,   662,   678,   687,
     687,   699,   699,   712,   712,   736,   736,   763,   767,   771,
     776,   780,   784,   790,   795,   799,   802,   804,   809,   813,
     818,   819
};
#endif

/** Accessing symbol of state STATE.  */
#define YY_ACCESSING_SYMBOL(State) YY_CAST (yysymbol_kind_t, yystos[State])

#if 1
/* The user-facing name of the symbol whose (internal) number is
   YYSYMBOL.  No bounds checking.  */
static const char *yysymbol_name (yysymbol_kind_t yysymbol) YY_ATTRIBUTE_UNUSED;

static const char *
yysymbol_name (yysymbol_kind_t yysymbol)
{
  static const char *const yy_sname[] =
  {
  "end of file", "error", "invalid token", "TOK_IF", "TOK_ELSE",
  "TOK_WHILE", "TOK_FOR", "TOK_FUNCTION", "TOK_RETURN", "TOK_BREAK",
  "TOK_CONTINUE", "TOK_AND", "TOK_NOT", "TOK_OR", "TOK_LOCAL", "TOK_TRUE",
  "TOK_FALSE", "TOK_NIL", "TOK_ASSIGN", "TOK_PLUS", "TOK_MINUS",
  "TOK_MULTIPLY", "TOK_DIVIDE", "TOK_MODULO", "TOK_EQUAL", "TOK_N_EQUAL",
  "TOK_DBL_PLUS", "TOK_DBL_MINUS", "TOK_GREATER", "TOK_LESS",
  "TOK_GR_EQUAL", "TOK_LS_EQUAL", "TOK_L_CURLY_BR", "TOK_R_CURLY_BR",
  "TOK_L_BR", "TOK_R_BR", "TOK_L_PARENTH", "TOK_R_PARENTH",
  "TOK_SEMICOLON", "TOK_COMMA", "TOK_COLON", "TOK_DBL_COLON", "TOK_DOT",
  "TOK_DBL_DOT", "TOK_ID", "TOK_INT", "TOK_FLOAT", "TOK_STRING",
  "TOK_UMINUS", "$accept", "program", "statement", "statements",
  "expression", "term", "assignexpr", "primary", "lvalue", "member",
  "call", "callsuffix", "normcall", "methodcall", "elist", "indexed",
  "objectdef", "indexedelem", "block", "$@1", "funcdef_block", "$@2",
  "funcdef", "$@3", "$@4", "const", "idlist", "if_statement",
  "while_statement", "for_statement", "return_statement", YY_NULLPTR
  };
  return yy_sname[yysymbol];
}
#endif

#define YYPACT_NINF (-60)

#define yypact_value_is_default(Yyn) \
  ((Yyn) == YYPACT_NINF)

#define YYTABLE_NINF (-4)

#define yytable_value_is_error(Yyn) \
  ((Yyn) == YYTABLE_NINF)

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
static const yytype_int16 yypact[] =
{
       6,    23,   231,   -60,    16,     0,    40,    46,   -12,   267,
      47,    57,   326,    52,   -60,   -60,   -60,   326,     1,     1,
     -60,   303,    77,   -60,    58,   -60,   -60,   -60,   -60,   105,
      -3,   -60,   -60,   -60,    72,   -60,    94,   -60,   -60,   -60,
     -60,   -60,   -60,   -60,   -60,   -60,   326,   326,   326,   -60,
      73,   -60,   377,   -60,   -60,   -60,   -60,   -60,   109,    83,
      94,    83,   -60,   326,   545,    -6,    -5,   -60,   419,    92,
     -60,   326,   326,   326,   326,   326,   326,   326,   326,   326,
     326,   326,   326,   326,   -60,   326,   -60,   -60,   326,   326,
      76,    97,   -60,   -60,   -60,   326,   104,   -60,   440,   461,
     -27,   112,   -60,   -60,   123,   137,   355,   -60,   326,   -60,
     129,   -60,   126,    28,   560,    39,    39,   -60,   -60,   -60,
     573,   573,   269,   269,   269,   269,   545,   482,     7,   -60,
     126,   503,   -60,   231,   231,   326,   -60,    49,   112,   126,
     -60,   -60,   326,   545,   -60,   -60,   -60,   -60,   -60,   -60,
     161,   -60,   398,   135,   124,    96,   524,   231,   326,   -60,
     -60,   -60,   135,   -60,   -60,   100,   -60,   -60,   231,   184,
     -60,   -60
};

/* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
   Performed when YYTABLE does not specify something else to do.  Zero
   means the default is an error.  */
static const yytype_int8 yydefact[] =
{
      16,     0,     0,     1,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    81,    82,    80,     0,     0,     0,
      69,    63,     0,    13,     0,    46,    77,    78,    79,    15,
       0,    31,    17,    39,    41,    49,    42,    43,    11,    12,
      45,     5,     6,     7,     8,    14,     0,     0,    63,    75,
       0,    90,     0,    10,     9,    34,    47,    33,     0,    35,
       0,    37,    16,     0,    61,     0,     0,    64,     0,     0,
      48,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     4,     0,    36,    38,     0,    63,
       0,     0,    55,    57,    58,     0,     0,    54,     0,     0,
       0,    85,    73,    91,     0,     0,     0,    66,     0,    67,
       0,    32,    44,    29,    30,    18,    19,    20,    21,    22,
      27,    28,    23,    24,    25,    26,    40,     0,     0,    50,
       0,     0,    52,     0,     0,     0,    83,     0,    85,     0,
      70,    15,     0,    62,    65,    56,    51,    59,    60,    53,
      86,    88,     0,     0,     0,     0,     0,     0,    63,    71,
      76,    84,     0,    68,    87,     0,    16,    74,     0,     0,
      89,    72
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
     -60,   -60,    -2,   -59,    -8,   -60,   -60,   -60,    60,   -60,
     140,   -60,   -29,   -60,   -46,   -60,   -60,    62,   -60,   -60,
      12,   -60,   -17,   -60,   -60,   -60,    38,   -60,   -60,   -60,
     -60
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_uint8 yydefgoto[] =
{
       0,     1,   141,     2,    30,    31,    32,    33,    34,    35,
      36,    92,    93,    94,    65,    66,    37,    67,    38,    62,
     160,   166,    39,   138,   101,    40,   137,    41,    42,    43,
      44
};

/* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule whose
   number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
      29,    52,   100,   105,    55,    69,    -3,    97,    71,    57,
      72,   135,   108,    64,    68,    13,    73,    74,    75,    76,
      77,    78,    79,     3,    49,    80,    81,    82,    83,   107,
     109,    97,    50,   108,   110,    84,    46,    58,    98,    99,
      64,   104,    24,   128,   147,    25,   108,    73,    74,    75,
      76,    77,    78,    79,    45,   106,    80,    81,    82,    83,
      75,    76,    77,   113,   114,   115,   116,   117,   118,   119,
     120,   121,   122,   123,   124,   125,    47,   126,    59,    61,
     127,    64,    48,   145,     8,    53,   153,   131,   154,    12,
      85,    13,    14,    15,    16,    54,    56,    17,    86,    87,
     143,   148,    70,    18,    19,    -2,    88,   169,    89,   102,
     145,    21,   165,    22,    90,    91,     8,    88,    24,    89,
     129,    25,    26,    27,    28,    90,    91,   152,    95,   112,
      89,   150,   151,   162,   156,   154,    96,   168,     4,   108,
       5,   130,     6,     7,     8,     9,    10,    11,   132,    12,
      64,    13,    14,    15,    16,   164,   136,    17,    60,    60,
     139,    63,    89,    18,    19,   157,   170,   159,   161,    20,
     140,    21,   144,    22,   167,    23,   155,     0,    24,     0,
       0,    25,    26,    27,    28,     4,     0,     5,     0,     6,
       7,     8,     9,    10,    11,     0,    12,     0,    13,    14,
      15,    16,     0,     0,    17,     0,     0,     0,     0,     0,
      18,    19,     0,     0,     0,     0,    20,   171,    21,     0,
      22,     0,    23,     0,     0,    24,     0,     0,    25,    26,
      27,    28,     4,     0,     5,     0,     6,     7,     8,     9,
      10,    11,     0,    12,     0,    13,    14,    15,    16,     0,
       0,    17,     0,     0,     0,     0,     0,    18,    19,     0,
       0,     0,     0,    20,     0,    21,     0,    22,     0,    23,
       0,     0,    24,     0,     0,    25,    26,    27,    28,    12,
       0,    13,    14,    15,    16,     0,     0,    17,    73,    74,
      75,    76,    77,    18,    19,     0,     0,    -4,    -4,    -4,
      -4,    21,     0,    22,     0,    51,     0,     0,    24,     0,
       0,    25,    26,    27,    28,    12,     0,    13,    14,    15,
      16,     0,     0,    17,     0,     0,     0,     0,     0,    18,
      19,     0,     0,     0,     0,    63,     0,    21,    12,    22,
      13,    14,    15,    16,    24,     0,    17,    25,    26,    27,
      28,     0,    18,    19,     0,     0,     0,     0,     0,     0,
      21,     0,    22,     0,     0,     0,    71,    24,    72,     0,
      25,    26,    27,    28,    73,    74,    75,    76,    77,    78,
      79,     0,     0,    80,    81,    82,    83,     0,    71,     0,
      72,     0,     0,     0,     0,   142,    73,    74,    75,    76,
      77,    78,    79,     0,     0,    80,    81,    82,    83,    71,
       0,    72,     0,     0,     0,   103,     0,    73,    74,    75,
      76,    77,    78,    79,     0,     0,    80,    81,    82,    83,
      71,     0,    72,     0,     0,     0,   158,     0,    73,    74,
      75,    76,    77,    78,    79,     0,     0,    80,    81,    82,
      83,    71,     0,    72,     0,     0,   111,     0,     0,    73,
      74,    75,    76,    77,    78,    79,     0,     0,    80,    81,
      82,    83,    71,     0,    72,     0,     0,   133,     0,     0,
      73,    74,    75,    76,    77,    78,    79,     0,     0,    80,
      81,    82,    83,    71,     0,    72,     0,     0,   134,     0,
       0,    73,    74,    75,    76,    77,    78,    79,     0,     0,
      80,    81,    82,    83,    71,     0,    72,   146,     0,     0,
       0,     0,    73,    74,    75,    76,    77,    78,    79,     0,
       0,    80,    81,    82,    83,    71,     0,    72,   149,     0,
       0,     0,     0,    73,    74,    75,    76,    77,    78,    79,
       0,     0,    80,    81,    82,    83,    71,   163,    72,     0,
       0,     0,     0,     0,    73,    74,    75,    76,    77,    78,
      79,    71,     0,    80,    81,    82,    83,     0,     0,    73,
      74,    75,    76,    77,    78,    79,     0,     0,    80,    81,
      82,    83,    73,    74,    75,    76,    77,    -4,    -4,     0,
       0,    80,    81,    82,    83
};

static const yytype_int16 yycheck[] =
{
       2,     9,    48,    62,    12,    22,     0,    36,    11,    17,
      13,    38,    39,    21,    22,    14,    19,    20,    21,    22,
      23,    24,    25,     0,    36,    28,    29,    30,    31,    35,
      35,    60,    44,    39,    39,    38,    36,    36,    46,    47,
      48,    58,    41,    89,    37,    44,    39,    19,    20,    21,
      22,    23,    24,    25,    38,    63,    28,    29,    30,    31,
      21,    22,    23,    71,    72,    73,    74,    75,    76,    77,
      78,    79,    80,    81,    82,    83,    36,    85,    18,    19,
      88,    89,    36,   112,     7,    38,    37,    95,    39,    12,
      18,    14,    15,    16,    17,    38,    44,    20,    26,    27,
     108,   130,    44,    26,    27,     0,    34,   166,    36,    36,
     139,    34,   158,    36,    42,    43,     7,    34,    41,    36,
      44,    44,    45,    46,    47,    42,    43,   135,    34,    37,
      36,   133,   134,    37,   142,    39,    42,    37,     1,    39,
       3,    44,     5,     6,     7,     8,     9,    10,    44,    12,
     158,    14,    15,    16,    17,   157,    44,    20,    18,    19,
      37,    32,    36,    26,    27,     4,   168,    32,    44,    32,
      33,    34,   110,    36,   162,    38,   138,    -1,    41,    -1,
      -1,    44,    45,    46,    47,     1,    -1,     3,    -1,     5,
       6,     7,     8,     9,    10,    -1,    12,    -1,    14,    15,
      16,    17,    -1,    -1,    20,    -1,    -1,    -1,    -1,    -1,
      26,    27,    -1,    -1,    -1,    -1,    32,    33,    34,    -1,
      36,    -1,    38,    -1,    -1,    41,    -1,    -1,    44,    45,
      46,    47,     1,    -1,     3,    -1,     5,     6,     7,     8,
       9,    10,    -1,    12,    -1,    14,    15,    16,    17,    -1,
      -1,    20,    -1,    -1,    -1,    -1,    -1,    26,    27,    -1,
      -1,    -1,    -1,    32,    -1,    34,    -1,    36,    -1,    38,
      -1,    -1,    41,    -1,    -1,    44,    45,    46,    47,    12,
      -1,    14,    15,    16,    17,    -1,    -1,    20,    19,    20,
      21,    22,    23,    26,    27,    -1,    -1,    28,    29,    30,
      31,    34,    -1,    36,    -1,    38,    -1,    -1,    41,    -1,
      -1,    44,    45,    46,    47,    12,    -1,    14,    15,    16,
      17,    -1,    -1,    20,    -1,    -1,    -1,    -1,    -1,    26,
      27,    -1,    -1,    -1,    -1,    32,    -1,    34,    12,    36,
      14,    15,    16,    17,    41,    -1,    20,    44,    45,    46,
      47,    -1,    26,    27,    -1,    -1,    -1,    -1,    -1,    -1,
      34,    -1,    36,    -1,    -1,    -1,    11,    41,    13,    -1,
      44,    45,    46,    47,    19,    20,    21,    22,    23,    24,
      25,    -1,    -1,    28,    29,    30,    31,    -1,    11,    -1,
      13,    -1,    -1,    -1,    -1,    40,    19,    20,    21,    22,
      23,    24,    25,    -1,    -1,    28,    29,    30,    31,    11,
      -1,    13,    -1,    -1,    -1,    38,    -1,    19,    20,    21,
      22,    23,    24,    25,    -1,    -1,    28,    29,    30,    31,
      11,    -1,    13,    -1,    -1,    -1,    38,    -1,    19,    20,
      21,    22,    23,    24,    25,    -1,    -1,    28,    29,    30,
      31,    11,    -1,    13,    -1,    -1,    37,    -1,    -1,    19,
      20,    21,    22,    23,    24,    25,    -1,    -1,    28,    29,
      30,    31,    11,    -1,    13,    -1,    -1,    37,    -1,    -1,
      19,    20,    21,    22,    23,    24,    25,    -1,    -1,    28,
      29,    30,    31,    11,    -1,    13,    -1,    -1,    37,    -1,
      -1,    19,    20,    21,    22,    23,    24,    25,    -1,    -1,
      28,    29,    30,    31,    11,    -1,    13,    35,    -1,    -1,
      -1,    -1,    19,    20,    21,    22,    23,    24,    25,    -1,
      -1,    28,    29,    30,    31,    11,    -1,    13,    35,    -1,
      -1,    -1,    -1,    19,    20,    21,    22,    23,    24,    25,
      -1,    -1,    28,    29,    30,    31,    11,    33,    13,    -1,
      -1,    -1,    -1,    -1,    19,    20,    21,    22,    23,    24,
      25,    11,    -1,    28,    29,    30,    31,    -1,    -1,    19,
      20,    21,    22,    23,    24,    25,    -1,    -1,    28,    29,
      30,    31,    19,    20,    21,    22,    23,    24,    25,    -1,
      -1,    28,    29,    30,    31
};

/* YYSTOS[STATE-NUM] -- The symbol kind of the accessing symbol of
   state STATE-NUM.  */
static const yytype_int8 yystos[] =
{
       0,    50,    52,     0,     1,     3,     5,     6,     7,     8,
       9,    10,    12,    14,    15,    16,    17,    20,    26,    27,
      32,    34,    36,    38,    41,    44,    45,    46,    47,    51,
      53,    54,    55,    56,    57,    58,    59,    65,    67,    71,
      74,    76,    77,    78,    79,    38,    36,    36,    36,    36,
      44,    38,    53,    38,    38,    53,    44,    53,    36,    57,
      59,    57,    68,    32,    53,    63,    64,    66,    53,    71,
      44,    11,    13,    19,    20,    21,    22,    23,    24,    25,
      28,    29,    30,    31,    38,    18,    26,    27,    34,    36,
      42,    43,    60,    61,    62,    34,    42,    61,    53,    53,
      63,    73,    36,    38,    71,    52,    53,    35,    39,    35,
      39,    37,    37,    53,    53,    53,    53,    53,    53,    53,
      53,    53,    53,    53,    53,    53,    53,    53,    63,    44,
      44,    53,    44,    37,    37,    38,    44,    75,    72,    37,
      33,    51,    40,    53,    66,    61,    35,    37,    61,    35,
      51,    51,    53,    37,    39,    75,    53,     4,    38,    32,
      69,    44,    37,    33,    51,    63,    70,    69,    37,    52,
      51,    33
};

/* YYR1[RULE-NUM] -- Symbol kind of the left-hand side of rule RULE-NUM.  */
static const yytype_int8 yyr1[] =
{
       0,    49,    50,    50,    51,    51,    51,    51,    51,    51,
      51,    51,    51,    51,    51,    52,    52,    53,    53,    53,
      53,    53,    53,    53,    53,    53,    53,    53,    53,    53,
      53,    53,    54,    54,    54,    54,    54,    54,    54,    54,
      55,    56,    56,    56,    56,    56,    57,    57,    57,    57,
      58,    58,    58,    58,    59,    59,    59,    60,    60,    61,
      62,    63,    63,    63,    64,    64,    65,    65,    66,    68,
      67,    70,    69,    72,    71,    73,    71,    74,    74,    74,
      74,    74,    74,    75,    75,    75,    76,    76,    77,    78,
      79,    79
};

/* YYR2[RULE-NUM] -- Number of symbols on the right-hand side of rule RULE-NUM.  */
static const yytype_int8 yyr2[] =
{
       0,     2,     2,     0,     2,     1,     1,     1,     1,     2,
       2,     1,     1,     1,     2,     2,     0,     1,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     1,     3,     2,     2,     2,     2,     2,     2,     1,
       3,     1,     1,     1,     3,     1,     1,     2,     2,     1,
       3,     4,     3,     4,     2,     2,     4,     1,     1,     3,
       3,     1,     3,     0,     1,     3,     3,     3,     5,     0,
       4,     0,     4,     0,     7,     0,     6,     1,     1,     1,
       1,     1,     1,     1,     3,     0,     5,     7,     5,     9,
       2,     3
};


enum { YYENOMEM = -2 };

#define yyerrok         (yyerrstatus = 0)
#define yyclearin       (yychar = YYEMPTY)

#define YYACCEPT        goto yyacceptlab
#define YYABORT         goto yyabortlab
#define YYERROR         goto yyerrorlab
#define YYNOMEM         goto yyexhaustedlab


#define YYRECOVERING()  (!!yyerrstatus)

#define YYBACKUP(Token, Value)                                    \
  do                                                              \
    if (yychar == YYEMPTY)                                        \
      {                                                           \
        yychar = (Token);                                         \
        yylval = (Value);                                         \
        YYPOPSTACK (yylen);                                       \
        yystate = *yyssp;                                         \
        YY_LAC_DISCARD ("YYBACKUP");                              \
        goto yybackup;                                            \
      }                                                           \
    else                                                          \
      {                                                           \
        yyerror (YY_("syntax error: cannot back up")); \
        YYERROR;                                                  \
      }                                                           \
  while (0)

/* Backward compatibility with an undocumented macro.
   Use YYerror or YYUNDEF. */
#define YYERRCODE YYUNDEF


/* Enable debugging if requested.  */
#if YYDEBUG

# ifndef YYFPRINTF
#  include <stdio.h> /* INFRINGES ON USER NAME SPACE */
#  define YYFPRINTF fprintf
# endif

# define YYDPRINTF(Args)                        \
do {                                            \
  if (yydebug)                                  \
    YYFPRINTF Args;                             \
} while (0)




# define YY_SYMBOL_PRINT(Title, Kind, Value, Location)                    \
do {                                                                      \
  if (yydebug)                                                            \
    {                                                                     \
      YYFPRINTF (stderr, "%s ", Title);                                   \
      yy_symbol_print (stderr,                                            \
                  Kind, Value); \
      YYFPRINTF (stderr, "\n");                                           \
    }                                                                     \
} while (0)


/*-----------------------------------.
| Print this symbol's value on YYO.  |
`-----------------------------------*/

static void
yy_symbol_value_print (FILE *yyo,
                       yysymbol_kind_t yykind, YYSTYPE const * const yyvaluep)
{
  FILE *yyoutput = yyo;
  YY_USE (yyoutput);
  if (!yyvaluep)
    return;
  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  YY_USE (yykind);
  YY_IGNORE_MAYBE_UNINITIALIZED_END
}


/*---------------------------.
| Print this symbol on YYO.  |
`---------------------------*/

static void
yy_symbol_print (FILE *yyo,
                 yysymbol_kind_t yykind, YYSTYPE const * const yyvaluep)
{
  YYFPRINTF (yyo, "%s %s (",
             yykind < YYNTOKENS ? "token" : "nterm", yysymbol_name (yykind));

  yy_symbol_value_print (yyo, yykind, yyvaluep);
  YYFPRINTF (yyo, ")");
}

/*------------------------------------------------------------------.
| yy_stack_print -- Print the state stack from its BOTTOM up to its |
| TOP (included).                                                   |
`------------------------------------------------------------------*/

static void
yy_stack_print (yy_state_t *yybottom, yy_state_t *yytop)
{
  YYFPRINTF (stderr, "Stack now");
  for (; yybottom <= yytop; yybottom++)
    {
      int yybot = *yybottom;
      YYFPRINTF (stderr, " %d", yybot);
    }
  YYFPRINTF (stderr, "\n");
}

# define YY_STACK_PRINT(Bottom, Top)                            \
do {                                                            \
  if (yydebug)                                                  \
    yy_stack_print ((Bottom), (Top));                           \
} while (0)


/*------------------------------------------------.
| Report that the YYRULE is going to be reduced.  |
`------------------------------------------------*/

static void
yy_reduce_print (yy_state_t *yyssp, YYSTYPE *yyvsp,
                 int yyrule)
{
  int yylno = yyrline[yyrule];
  int yynrhs = yyr2[yyrule];
  int yyi;
  YYFPRINTF (stderr, "Reducing stack by rule %d (line %d):\n",
             yyrule - 1, yylno);
  /* The symbols being reduced.  */
  for (yyi = 0; yyi < yynrhs; yyi++)
    {
      YYFPRINTF (stderr, "   $%d = ", yyi + 1);
      yy_symbol_print (stderr,
                       YY_ACCESSING_SYMBOL (+yyssp[yyi + 1 - yynrhs]),
                       &yyvsp[(yyi + 1) - (yynrhs)]);
      YYFPRINTF (stderr, "\n");
    }
}

# define YY_REDUCE_PRINT(Rule)          \
do {                                    \
  if (yydebug)                          \
    yy_reduce_print (yyssp, yyvsp, Rule); \
} while (0)

/* Nonzero means print parse trace.  It is left uninitialized so that
   multiple parsers can coexist.  */
int yydebug;
#else /* !YYDEBUG */
# define YYDPRINTF(Args) ((void) 0)
# define YY_SYMBOL_PRINT(Title, Kind, Value, Location)
# define YY_STACK_PRINT(Bottom, Top)
# define YY_REDUCE_PRINT(Rule)
#endif /* !YYDEBUG */


/* YYINITDEPTH -- initial size of the parser's stacks.  */
#ifndef YYINITDEPTH
# define YYINITDEPTH 200
#endif

/* YYMAXDEPTH -- maximum size the stacks can grow to (effective only
   if the built-in stack extension method is used).

   Do not make this value too large; the results are undefined if
   YYSTACK_ALLOC_MAXIMUM < YYSTACK_BYTES (YYMAXDEPTH)
   evaluated with infinite-precision integer arithmetic.  */

#ifndef YYMAXDEPTH
# define YYMAXDEPTH 10000
#endif


/* Given a state stack such that *YYBOTTOM is its bottom, such that
   *YYTOP is either its top or is YYTOP_EMPTY to indicate an empty
   stack, and such that *YYCAPACITY is the maximum number of elements it
   can hold without a reallocation, make sure there is enough room to
   store YYADD more elements.  If not, allocate a new stack using
   YYSTACK_ALLOC, copy the existing elements, and adjust *YYBOTTOM,
   *YYTOP, and *YYCAPACITY to reflect the new capacity and memory
   location.  If *YYBOTTOM != YYBOTTOM_NO_FREE, then free the old stack
   using YYSTACK_FREE.  Return 0 if successful or if no reallocation is
   required.  Return YYENOMEM if memory is exhausted.  */
static int
yy_lac_stack_realloc (YYPTRDIFF_T *yycapacity, YYPTRDIFF_T yyadd,
#if YYDEBUG
                      char const *yydebug_prefix,
                      char const *yydebug_suffix,
#endif
                      yy_state_t **yybottom,
                      yy_state_t *yybottom_no_free,
                      yy_state_t **yytop, yy_state_t *yytop_empty)
{
  YYPTRDIFF_T yysize_old =
    *yytop == yytop_empty ? 0 : *yytop - *yybottom + 1;
  YYPTRDIFF_T yysize_new = yysize_old + yyadd;
  if (*yycapacity < yysize_new)
    {
      YYPTRDIFF_T yyalloc = 2 * yysize_new;
      yy_state_t *yybottom_new;
      /* Use YYMAXDEPTH for maximum stack size given that the stack
         should never need to grow larger than the main state stack
         needs to grow without LAC.  */
      if (YYMAXDEPTH < yysize_new)
        {
          YYDPRINTF ((stderr, "%smax size exceeded%s", yydebug_prefix,
                      yydebug_suffix));
          return YYENOMEM;
        }
      if (YYMAXDEPTH < yyalloc)
        yyalloc = YYMAXDEPTH;
      yybottom_new =
        YY_CAST (yy_state_t *,
                 YYSTACK_ALLOC (YY_CAST (YYSIZE_T,
                                         yyalloc * YYSIZEOF (*yybottom_new))));
      if (!yybottom_new)
        {
          YYDPRINTF ((stderr, "%srealloc failed%s", yydebug_prefix,
                      yydebug_suffix));
          return YYENOMEM;
        }
      if (*yytop != yytop_empty)
        {
          YYCOPY (yybottom_new, *yybottom, yysize_old);
          *yytop = yybottom_new + (yysize_old - 1);
        }
      if (*yybottom != yybottom_no_free)
        YYSTACK_FREE (*yybottom);
      *yybottom = yybottom_new;
      *yycapacity = yyalloc;
    }
  return 0;
}

/* Establish the initial context for the current lookahead if no initial
   context is currently established.

   We define a context as a snapshot of the parser stacks.  We define
   the initial context for a lookahead as the context in which the
   parser initially examines that lookahead in order to select a
   syntactic action.  Thus, if the lookahead eventually proves
   syntactically unacceptable (possibly in a later context reached via a
   series of reductions), the initial context can be used to determine
   the exact set of tokens that would be syntactically acceptable in the
   lookahead's place.  Moreover, it is the context after which any
   further semantic actions would be erroneous because they would be
   determined by a syntactically unacceptable token.

   YY_LAC_ESTABLISH should be invoked when a reduction is about to be
   performed in an inconsistent state (which, for the purposes of LAC,
   includes consistent states that don't know they're consistent because
   their default reductions have been disabled).  Iff there is a
   lookahead token, it should also be invoked before reporting a syntax
   error.  This latter case is for the sake of the debugging output.

   For parse.lac=full, the implementation of YY_LAC_ESTABLISH is as
   follows.  If no initial context is currently established for the
   current lookahead, then check if that lookahead can eventually be
   shifted if syntactic actions continue from the current context.
   Report a syntax error if it cannot.  */
#define YY_LAC_ESTABLISH                                                \
do {                                                                    \
  if (!yy_lac_established)                                              \
    {                                                                   \
      YYDPRINTF ((stderr,                                               \
                  "LAC: initial context established for %s\n",          \
                  yysymbol_name (yytoken)));                            \
      yy_lac_established = 1;                                           \
      switch (yy_lac (yyesa, &yyes, &yyes_capacity, yyssp, yytoken))    \
        {                                                               \
        case YYENOMEM:                                                  \
          YYNOMEM;                                                      \
        case 1:                                                         \
          goto yyerrlab;                                                \
        }                                                               \
    }                                                                   \
} while (0)

/* Discard any previous initial lookahead context because of Event,
   which may be a lookahead change or an invalidation of the currently
   established initial context for the current lookahead.

   The most common example of a lookahead change is a shift.  An example
   of both cases is syntax error recovery.  That is, a syntax error
   occurs when the lookahead is syntactically erroneous for the
   currently established initial context, so error recovery manipulates
   the parser stacks to try to find a new initial context in which the
   current lookahead is syntactically acceptable.  If it fails to find
   such a context, it discards the lookahead.  */
#if YYDEBUG
# define YY_LAC_DISCARD(Event)                                           \
do {                                                                     \
  if (yy_lac_established)                                                \
    {                                                                    \
      YYDPRINTF ((stderr, "LAC: initial context discarded due to "       \
                  Event "\n"));                                          \
      yy_lac_established = 0;                                            \
    }                                                                    \
} while (0)
#else
# define YY_LAC_DISCARD(Event) yy_lac_established = 0
#endif

/* Given the stack whose top is *YYSSP, return 0 iff YYTOKEN can
   eventually (after perhaps some reductions) be shifted, return 1 if
   not, or return YYENOMEM if memory is exhausted.  As preconditions and
   postconditions: *YYES_CAPACITY is the allocated size of the array to
   which *YYES points, and either *YYES = YYESA or *YYES points to an
   array allocated with YYSTACK_ALLOC.  yy_lac may overwrite the
   contents of either array, alter *YYES and *YYES_CAPACITY, and free
   any old *YYES other than YYESA.  */
static int
yy_lac (yy_state_t *yyesa, yy_state_t **yyes,
        YYPTRDIFF_T *yyes_capacity, yy_state_t *yyssp, yysymbol_kind_t yytoken)
{
  yy_state_t *yyes_prev = yyssp;
  yy_state_t *yyesp = yyes_prev;
  /* Reduce until we encounter a shift and thereby accept the token.  */
  YYDPRINTF ((stderr, "LAC: checking lookahead %s:", yysymbol_name (yytoken)));
  if (yytoken == YYSYMBOL_YYUNDEF)
    {
      YYDPRINTF ((stderr, " Always Err\n"));
      return 1;
    }
  while (1)
    {
      int yyrule = yypact[+*yyesp];
      if (yypact_value_is_default (yyrule)
          || (yyrule += yytoken) < 0 || YYLAST < yyrule
          || yycheck[yyrule] != yytoken)
        {
          /* Use the default action.  */
          yyrule = yydefact[+*yyesp];
          if (yyrule == 0)
            {
              YYDPRINTF ((stderr, " Err\n"));
              return 1;
            }
        }
      else
        {
          /* Use the action from yytable.  */
          yyrule = yytable[yyrule];
          if (yytable_value_is_error (yyrule))
            {
              YYDPRINTF ((stderr, " Err\n"));
              return 1;
            }
          if (0 < yyrule)
            {
              YYDPRINTF ((stderr, " S%d\n", yyrule));
              return 0;
            }
          yyrule = -yyrule;
        }
      /* By now we know we have to simulate a reduce.  */
      YYDPRINTF ((stderr, " R%d", yyrule - 1));
      {
        /* Pop the corresponding number of values from the stack.  */
        YYPTRDIFF_T yylen = yyr2[yyrule];
        /* First pop from the LAC stack as many tokens as possible.  */
        if (yyesp != yyes_prev)
          {
            YYPTRDIFF_T yysize = yyesp - *yyes + 1;
            if (yylen < yysize)
              {
                yyesp -= yylen;
                yylen = 0;
              }
            else
              {
                yyesp = yyes_prev;
                yylen -= yysize;
              }
          }
        /* Only afterwards look at the main stack.  */
        if (yylen)
          yyesp = yyes_prev -= yylen;
      }
      /* Push the resulting state of the reduction.  */
      {
        yy_state_fast_t yystate;
        {
          const int yylhs = yyr1[yyrule] - YYNTOKENS;
          const int yyi = yypgoto[yylhs] + *yyesp;
          yystate = (0 <= yyi && yyi <= YYLAST && yycheck[yyi] == *yyesp
                     ? yytable[yyi]
                     : yydefgoto[yylhs]);
        }
        if (yyesp == yyes_prev)
          {
            yyesp = *yyes;
            YY_IGNORE_USELESS_CAST_BEGIN
            *yyesp = YY_CAST (yy_state_t, yystate);
            YY_IGNORE_USELESS_CAST_END
          }
        else
          {
            if (yy_lac_stack_realloc (yyes_capacity, 1,
#if YYDEBUG
                                      " (", ")",
#endif
                                      yyes, yyesa, &yyesp, yyes_prev))
              {
                YYDPRINTF ((stderr, "\n"));
                return YYENOMEM;
              }
            YY_IGNORE_USELESS_CAST_BEGIN
            *++yyesp = YY_CAST (yy_state_t, yystate);
            YY_IGNORE_USELESS_CAST_END
          }
        YYDPRINTF ((stderr, " G%d", yystate));
      }
    }
}

/* Context of a parse error.  */
typedef struct
{
  yy_state_t *yyssp;
  yy_state_t *yyesa;
  yy_state_t **yyes;
  YYPTRDIFF_T *yyes_capacity;
  yysymbol_kind_t yytoken;
} yypcontext_t;

/* Put in YYARG at most YYARGN of the expected tokens given the
   current YYCTX, and return the number of tokens stored in YYARG.  If
   YYARG is null, return the number of expected tokens (guaranteed to
   be less than YYNTOKENS).  Return YYENOMEM on memory exhaustion.
   Return 0 if there are more than YYARGN expected tokens, yet fill
   YYARG up to YYARGN. */
static int
yypcontext_expected_tokens (const yypcontext_t *yyctx,
                            yysymbol_kind_t yyarg[], int yyargn)
{
  /* Actual size of YYARG. */
  int yycount = 0;

  int yyx;
  for (yyx = 0; yyx < YYNTOKENS; ++yyx)
    {
      yysymbol_kind_t yysym = YY_CAST (yysymbol_kind_t, yyx);
      if (yysym != YYSYMBOL_YYerror && yysym != YYSYMBOL_YYUNDEF)
        switch (yy_lac (yyctx->yyesa, yyctx->yyes, yyctx->yyes_capacity, yyctx->yyssp, yysym))
          {
          case YYENOMEM:
            return YYENOMEM;
          case 1:
            continue;
          default:
            if (!yyarg)
              ++yycount;
            else if (yycount == yyargn)
              return 0;
            else
              yyarg[yycount++] = yysym;
          }
    }
  if (yyarg && yycount == 0 && 0 < yyargn)
    yyarg[0] = YYSYMBOL_YYEMPTY;
  return yycount;
}




#ifndef yystrlen
# if defined __GLIBC__ && defined _STRING_H
#  define yystrlen(S) (YY_CAST (YYPTRDIFF_T, strlen (S)))
# else
/* Return the length of YYSTR.  */
static YYPTRDIFF_T
yystrlen (const char *yystr)
{
  YYPTRDIFF_T yylen;
  for (yylen = 0; yystr[yylen]; yylen++)
    continue;
  return yylen;
}
# endif
#endif

#ifndef yystpcpy
# if defined __GLIBC__ && defined _STRING_H && defined _GNU_SOURCE
#  define yystpcpy stpcpy
# else
/* Copy YYSRC to YYDEST, returning the address of the terminating '\0' in
   YYDEST.  */
static char *
yystpcpy (char *yydest, const char *yysrc)
{
  char *yyd = yydest;
  const char *yys = yysrc;

  while ((*yyd++ = *yys++) != '\0')
    continue;

  return yyd - 1;
}
# endif
#endif



static int
yy_syntax_error_arguments (const yypcontext_t *yyctx,
                           yysymbol_kind_t yyarg[], int yyargn)
{
  /* Actual size of YYARG. */
  int yycount = 0;
  /* There are many possibilities here to consider:
     - If this state is a consistent state with a default action, then
       the only way this function was invoked is if the default action
       is an error action.  In that case, don't check for expected
       tokens because there are none.
     - The only way there can be no lookahead present (in yychar) is if
       this state is a consistent state with a default action.  Thus,
       detecting the absence of a lookahead is sufficient to determine
       that there is no unexpected or expected token to report.  In that
       case, just report a simple "syntax error".
     - Don't assume there isn't a lookahead just because this state is a
       consistent state with a default action.  There might have been a
       previous inconsistent state, consistent state with a non-default
       action, or user semantic action that manipulated yychar.
       In the first two cases, it might appear that the current syntax
       error should have been detected in the previous state when yy_lac
       was invoked.  However, at that time, there might have been a
       different syntax error that discarded a different initial context
       during error recovery, leaving behind the current lookahead.
  */
  if (yyctx->yytoken != YYSYMBOL_YYEMPTY)
    {
      int yyn;
      YYDPRINTF ((stderr, "Constructing syntax error message\n"));
      if (yyarg)
        yyarg[yycount] = yyctx->yytoken;
      ++yycount;
      yyn = yypcontext_expected_tokens (yyctx,
                                        yyarg ? yyarg + 1 : yyarg, yyargn - 1);
      if (yyn == YYENOMEM)
        return YYENOMEM;
      else if (yyn == 0)
        YYDPRINTF ((stderr, "No expected tokens.\n"));
      else
        yycount += yyn;
    }
  return yycount;
}

/* Copy into *YYMSG, which is of size *YYMSG_ALLOC, an error message
   about the unexpected token YYTOKEN for the state stack whose top is
   YYSSP.  In order to see if a particular token T is a
   valid looakhead, invoke yy_lac (YYESA, YYES, YYES_CAPACITY, YYSSP, T).

   Return 0 if *YYMSG was successfully written.  Return -1 if *YYMSG is
   not large enough to hold the message.  In that case, also set
   *YYMSG_ALLOC to the required number of bytes.  Return YYENOMEM if the
   required number of bytes is too large to store or if
   yy_lac returned YYENOMEM.  */
static int
yysyntax_error (YYPTRDIFF_T *yymsg_alloc, char **yymsg,
                const yypcontext_t *yyctx)
{
  enum { YYARGS_MAX = 5 };
  /* Internationalized format string. */
  const char *yyformat = YY_NULLPTR;
  /* Arguments of yyformat: reported tokens (one for the "unexpected",
     one per "expected"). */
  yysymbol_kind_t yyarg[YYARGS_MAX];
  /* Cumulated lengths of YYARG.  */
  YYPTRDIFF_T yysize = 0;

  /* Actual size of YYARG. */
  int yycount = yy_syntax_error_arguments (yyctx, yyarg, YYARGS_MAX);
  if (yycount == YYENOMEM)
    return YYENOMEM;

  switch (yycount)
    {
#define YYCASE_(N, S)                       \
      case N:                               \
        yyformat = S;                       \
        break
    default: /* Avoid compiler warnings. */
      YYCASE_(0, YY_("syntax error"));
      YYCASE_(1, YY_("syntax error, unexpected %s"));
      YYCASE_(2, YY_("syntax error, unexpected %s, expecting %s"));
      YYCASE_(3, YY_("syntax error, unexpected %s, expecting %s or %s"));
      YYCASE_(4, YY_("syntax error, unexpected %s, expecting %s or %s or %s"));
      YYCASE_(5, YY_("syntax error, unexpected %s, expecting %s or %s or %s or %s"));
#undef YYCASE_
    }

  /* Compute error message size.  Don't count the "%s"s, but reserve
     room for the terminator.  */
  yysize = yystrlen (yyformat) - 2 * yycount + 1;
  {
    int yyi;
    for (yyi = 0; yyi < yycount; ++yyi)
      {
        YYPTRDIFF_T yysize1
          = yysize + yystrlen (yysymbol_name (yyarg[yyi]));
        if (yysize <= yysize1 && yysize1 <= YYSTACK_ALLOC_MAXIMUM)
          yysize = yysize1;
        else
          return YYENOMEM;
      }
  }

  if (*yymsg_alloc < yysize)
    {
      *yymsg_alloc = 2 * yysize;
      if (! (yysize <= *yymsg_alloc
             && *yymsg_alloc <= YYSTACK_ALLOC_MAXIMUM))
        *yymsg_alloc = YYSTACK_ALLOC_MAXIMUM;
      return -1;
    }

  /* Avoid sprintf, as that infringes on the user's name space.
     Don't have undefined behavior even if the translation
     produced a string with the wrong number of "%s"s.  */
  {
    char *yyp = *yymsg;
    int yyi = 0;
    while ((*yyp = *yyformat) != '\0')
      if (*yyp == '%' && yyformat[1] == 's' && yyi < yycount)
        {
          yyp = yystpcpy (yyp, yysymbol_name (yyarg[yyi++]));
          yyformat += 2;
        }
      else
        {
          ++yyp;
          ++yyformat;
        }
  }
  return 0;
}


/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

static void
yydestruct (const char *yymsg,
            yysymbol_kind_t yykind, YYSTYPE *yyvaluep)
{
  YY_USE (yyvaluep);
  if (!yymsg)
    yymsg = "Deleting";
  YY_SYMBOL_PRINT (yymsg, yykind, yyvaluep, yylocationp);

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  YY_USE (yykind);
  YY_IGNORE_MAYBE_UNINITIALIZED_END
}


/* Lookahead token kind.  */
int yychar;

/* The semantic value of the lookahead symbol.  */
YYSTYPE yylval;
/* Number of syntax errors so far.  */
int yynerrs;




/*----------.
| yyparse.  |
`----------*/

int
yyparse (void)
{
    yy_state_fast_t yystate = 0;
    /* Number of tokens to shift before error messages enabled.  */
    int yyerrstatus = 0;

    /* Refer to the stacks through separate pointers, to allow yyoverflow
       to reallocate them elsewhere.  */

    /* Their size.  */
    YYPTRDIFF_T yystacksize = YYINITDEPTH;

    /* The state stack: array, bottom, top.  */
    yy_state_t yyssa[YYINITDEPTH];
    yy_state_t *yyss = yyssa;
    yy_state_t *yyssp = yyss;

    /* The semantic value stack: array, bottom, top.  */
    YYSTYPE yyvsa[YYINITDEPTH];
    YYSTYPE *yyvs = yyvsa;
    YYSTYPE *yyvsp = yyvs;

    yy_state_t yyesa[20];
    yy_state_t *yyes = yyesa;
    YYPTRDIFF_T yyes_capacity = 20 < YYMAXDEPTH ? 20 : YYMAXDEPTH;

  /* Whether LAC context is established.  A Boolean.  */
  int yy_lac_established = 0;
  int yyn;
  /* The return value of yyparse.  */
  int yyresult;
  /* Lookahead symbol kind.  */
  yysymbol_kind_t yytoken = YYSYMBOL_YYEMPTY;
  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;

  /* Buffer for error messages, and its allocated size.  */
  char yymsgbuf[128];
  char *yymsg = yymsgbuf;
  YYPTRDIFF_T yymsg_alloc = sizeof yymsgbuf;

#define YYPOPSTACK(N)   (yyvsp -= (N), yyssp -= (N))

  /* The number of symbols on the RHS of the reduced rule.
     Keep to zero when no symbol should be popped.  */
  int yylen = 0;

  YYDPRINTF ((stderr, "Starting parse\n"));

  yychar = YYEMPTY; /* Cause a token to be read.  */

  goto yysetstate;


/*------------------------------------------------------------.
| yynewstate -- push a new state, which is found in yystate.  |
`------------------------------------------------------------*/
yynewstate:
  /* In all cases, when you get here, the value and location stacks
     have just been pushed.  So pushing a state here evens the stacks.  */
  yyssp++;


/*--------------------------------------------------------------------.
| yysetstate -- set current state (the top of the stack) to yystate.  |
`--------------------------------------------------------------------*/
yysetstate:
  YYDPRINTF ((stderr, "Entering state %d\n", yystate));
  YY_ASSERT (0 <= yystate && yystate < YYNSTATES);
  YY_IGNORE_USELESS_CAST_BEGIN
  *yyssp = YY_CAST (yy_state_t, yystate);
  YY_IGNORE_USELESS_CAST_END
  YY_STACK_PRINT (yyss, yyssp);

  if (yyss + yystacksize - 1 <= yyssp)
#if !defined yyoverflow && !defined YYSTACK_RELOCATE
    YYNOMEM;
#else
    {
      /* Get the current used size of the three stacks, in elements.  */
      YYPTRDIFF_T yysize = yyssp - yyss + 1;

# if defined yyoverflow
      {
        /* Give user a chance to reallocate the stack.  Use copies of
           these so that the &'s don't force the real ones into
           memory.  */
        yy_state_t *yyss1 = yyss;
        YYSTYPE *yyvs1 = yyvs;

        /* Each stack pointer address is followed by the size of the
           data in use in that stack, in bytes.  This used to be a
           conditional around just the two extra args, but that might
           be undefined if yyoverflow is a macro.  */
        yyoverflow (YY_("memory exhausted"),
                    &yyss1, yysize * YYSIZEOF (*yyssp),
                    &yyvs1, yysize * YYSIZEOF (*yyvsp),
                    &yystacksize);
        yyss = yyss1;
        yyvs = yyvs1;
      }
# else /* defined YYSTACK_RELOCATE */
      /* Extend the stack our own way.  */
      if (YYMAXDEPTH <= yystacksize)
        YYNOMEM;
      yystacksize *= 2;
      if (YYMAXDEPTH < yystacksize)
        yystacksize = YYMAXDEPTH;

      {
        yy_state_t *yyss1 = yyss;
        union yyalloc *yyptr =
          YY_CAST (union yyalloc *,
                   YYSTACK_ALLOC (YY_CAST (YYSIZE_T, YYSTACK_BYTES (yystacksize))));
        if (! yyptr)
          YYNOMEM;
        YYSTACK_RELOCATE (yyss_alloc, yyss);
        YYSTACK_RELOCATE (yyvs_alloc, yyvs);
#  undef YYSTACK_RELOCATE
        if (yyss1 != yyssa)
          YYSTACK_FREE (yyss1);
      }
# endif

      yyssp = yyss + yysize - 1;
      yyvsp = yyvs + yysize - 1;

      YY_IGNORE_USELESS_CAST_BEGIN
      YYDPRINTF ((stderr, "Stack size increased to %ld\n",
                  YY_CAST (long, yystacksize)));
      YY_IGNORE_USELESS_CAST_END

      if (yyss + yystacksize - 1 <= yyssp)
        YYABORT;
    }
#endif /* !defined yyoverflow && !defined YYSTACK_RELOCATE */


  if (yystate == YYFINAL)
    YYACCEPT;

  goto yybackup;


/*-----------.
| yybackup.  |
`-----------*/
yybackup:
  /* Do appropriate processing given the current state.  Read a
     lookahead token if we need one and don't already have one.  */

  /* First try to decide what to do without reference to lookahead token.  */
  yyn = yypact[yystate];
  if (yypact_value_is_default (yyn))
    goto yydefault;

  /* Not known => get a lookahead token if don't already have one.  */

  /* YYCHAR is either empty, or end-of-input, or a valid lookahead.  */
  if (yychar == YYEMPTY)
    {
      YYDPRINTF ((stderr, "Reading a token\n"));
      yychar = yylex ();
    }

  if (yychar <= YYEOF)
    {
      yychar = YYEOF;
      yytoken = YYSYMBOL_YYEOF;
      YYDPRINTF ((stderr, "Now at end of input.\n"));
    }
  else if (yychar == YYerror)
    {
      /* The scanner already issued an error message, process directly
         to error recovery.  But do not keep the error token as
         lookahead, it is too special and may lead us to an endless
         loop in error recovery. */
      yychar = YYUNDEF;
      yytoken = YYSYMBOL_YYerror;
      goto yyerrlab1;
    }
  else
    {
      yytoken = YYTRANSLATE (yychar);
      YY_SYMBOL_PRINT ("Next token is", yytoken, &yylval, &yylloc);
    }

  /* If the proper action on seeing token YYTOKEN is to reduce or to
     detect an error, take that action.  */
  yyn += yytoken;
  if (yyn < 0 || YYLAST < yyn || yycheck[yyn] != yytoken)
    {
      YY_LAC_ESTABLISH;
      goto yydefault;
    }
  yyn = yytable[yyn];
  if (yyn <= 0)
    {
      if (yytable_value_is_error (yyn))
        goto yyerrlab;
      yyn = -yyn;
      YY_LAC_ESTABLISH;
      goto yyreduce;
    }

  /* Count tokens shifted since error; after three, turn off error
     status.  */
  if (yyerrstatus)
    yyerrstatus--;

  /* Shift the lookahead token.  */
  YY_SYMBOL_PRINT ("Shifting", yytoken, &yylval, &yylloc);
  yystate = yyn;
  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END

  /* Discard the shifted token.  */
  yychar = YYEMPTY;
  YY_LAC_DISCARD ("shift");
  goto yynewstate;


/*-----------------------------------------------------------.
| yydefault -- do the default action for the current state.  |
`-----------------------------------------------------------*/
yydefault:
  yyn = yydefact[yystate];
  if (yyn == 0)
    goto yyerrlab;
  goto yyreduce;


/*-----------------------------.
| yyreduce -- do a reduction.  |
`-----------------------------*/
yyreduce:
  /* yyn is the number of a rule to reduce with.  */
  yylen = yyr2[yyn];

  /* If YYLEN is nonzero, implement the default value of the action:
     '$$ = $1'.

     Otherwise, the following line sets YYVAL to garbage.
     This behavior is undocumented and Bison
     users should not rely upon it.  Assigning to YYVAL
     unconditionally makes the parser a bit smaller, and it avoids a
     GCC warning that YYVAL may be used uninitialized.  */
  yyval = yyvsp[1-yylen];


  YY_REDUCE_PRINT (yyn);
  {
    int yychar_backup = yychar;
    switch (yyn)
      {
  case 2: /* program: statements statement  */
#line 123 "parser.y"
                                                                                                       {}
#line 1819 "parser.c"
    break;

  case 3: /* program: %empty  */
#line 124 "parser.y"
                                                                                                       {}
#line 1825 "parser.c"
    break;

  case 4: /* statement: expression TOK_SEMICOLON  */
#line 127 "parser.y"
                                                                                                       {}
#line 1831 "parser.c"
    break;

  case 5: /* statement: if_statement  */
#line 128 "parser.y"
                                                                                                       {}
#line 1837 "parser.c"
    break;

  case 6: /* statement: while_statement  */
#line 129 "parser.y"
                                                                                                       {}
#line 1843 "parser.c"
    break;

  case 7: /* statement: for_statement  */
#line 130 "parser.y"
                                                                                                       {}
#line 1849 "parser.c"
    break;

  case 8: /* statement: return_statement  */
#line 131 "parser.y"
                                                                                                       {}
#line 1855 "parser.c"
    break;

  case 9: /* statement: TOK_CONTINUE TOK_SEMICOLON  */
#line 132 "parser.y"
                                                                                                       {}
#line 1861 "parser.c"
    break;

  case 10: /* statement: TOK_BREAK TOK_SEMICOLON  */
#line 133 "parser.y"
                                                                                                       {}
#line 1867 "parser.c"
    break;

  case 11: /* statement: block  */
#line 134 "parser.y"
                                                                                                       {}
#line 1873 "parser.c"
    break;

  case 12: /* statement: funcdef  */
#line 135 "parser.y"
                                                                                                       {}
#line 1879 "parser.c"
    break;

  case 13: /* statement: TOK_SEMICOLON  */
#line 136 "parser.y"
                                                                                                       {}
#line 1885 "parser.c"
    break;

  case 14: /* statement: error TOK_SEMICOLON  */
#line 137 "parser.y"
                                {yyerrok; yyclearin;}
#line 1891 "parser.c"
    break;

  case 15: /* statements: statements statement  */
#line 140 "parser.y"
                                                                                                       {}
#line 1897 "parser.c"
    break;

  case 16: /* statements: %empty  */
#line 141 "parser.y"
                                                                                                       {}
#line 1903 "parser.c"
    break;

  case 17: /* expression: assignexpr  */
#line 144 "parser.y"
                                                                                                       {}
#line 1909 "parser.c"
    break;

  case 18: /* expression: expression TOK_PLUS expression  */
#line 145 "parser.y"
                                                                                                       {

                                                                                                            (yyval.expr) = manage_expr_plus_expr((yyval.expr), (yyvsp[-2].expr), (yyvsp[0].expr));
                    
                                                                                                       }
#line 1919 "parser.c"
    break;

  case 19: /* expression: expression TOK_MINUS expression  */
#line 150 "parser.y"
                                                                                                       {
                                                                                                            
                                                                                                            (yyval.expr) = manage_expr_minus_expr((yyval.expr), (yyvsp[-2].expr), (yyvsp[0].expr));
                                                                                                       
                                                                                                       }
#line 1929 "parser.c"
    break;

  case 20: /* expression: expression TOK_MULTIPLY expression  */
#line 155 "parser.y"
                                                                                                       {

                                                                                                            (yyval.expr) = manage_expr_mul_expr((yyval.expr), (yyvsp[-2].expr), (yyvsp[0].expr));        
                    
                                                                                                       }
#line 1939 "parser.c"
    break;

  case 21: /* expression: expression TOK_DIVIDE expression  */
#line 160 "parser.y"
                                                                                                       {    
                                                                                                            (yyval.expr) = manage_expr_div_expr((yyval.expr), (yyvsp[-2].expr), (yyvsp[0].expr));
                                                                                                       }
#line 1947 "parser.c"
    break;

  case 22: /* expression: expression TOK_MODULO expression  */
#line 163 "parser.y"
                                               {
                    //printf("expression -> expression modulo expression\n");
                    //handle_expression($1,$3, get_scope(), yylineno);
                    if (check_if_is_arithm((yyvsp[-2].expr)) == true && check_if_is_arithm((yyvsp[0].expr)) == true) {
                        SymbolType type = GLOBAL;
                        if (get_scope() != 0) {
                            type = LOCAL;
                        }
                        SymbolTableEntry new_temp = new_temp_var(-1, type);
                        //std::cout << new_temp.scope << std::endl;
                        (yyval.expr) = make_arithm_expression(ARITHM_EXPR, get_symbol(new_temp.name, get_scope()));
                        std::cout << "$1->type " << (yyvsp[-2].expr)->type << std::endl;
                        emit_mod(MOD, (yyval.expr), (yyvsp[-2].expr), (yyvsp[0].expr));
                    }
                }
#line 1967 "parser.c"
    break;

  case 23: /* expression: expression TOK_GREATER expression  */
#line 178 "parser.y"
                                                {
                    //printf("expression -> expression > expression\n");
                    //handle_expression($1,$3, get_scope(), yylineno);
                    SymbolType type = GLOBAL;
                    if (get_scope() != 0) {
                        type = LOCAL;
                    }
                    SymbolTableEntry new_temp = new_temp_var(-1, type);
                    //std::cout << new_temp.scope << std::endl;
                    (yyval.expr) = make_bool_expression(BOOL_EXPR, get_symbol(new_temp.name, get_scope()));
                    emit_if_greater(IF_GR, (yyvsp[-2].expr), (yyvsp[0].expr), next_quad_label()+3);
                    set_curr_quad_label((next_quad_label()+3));
                    emit_assign(ASSIGN, (yyval.expr), make_constbool_expression(CONSTBOOL_EXPR, 1));
                    emit_jump(JUMP, (next_quad_label()+2));
                    set_curr_quad_label((next_quad_label()+2));
                    emit_assign(ASSIGN, (yyval.expr), make_constbool_expression(CONSTBOOL_EXPR, 0));
                }
#line 1989 "parser.c"
    break;

  case 24: /* expression: expression TOK_LESS expression  */
#line 195 "parser.y"
                                             {
                    //printf("expression -> expression < expression\n");
                    //handle_expression($1,$3, get_scope(), yylineno);
                    SymbolType type = GLOBAL;
                    if (get_scope() != 0) {
                        type = LOCAL;
                    }
                    SymbolTableEntry new_temp = new_temp_var(-1, type);
                    //std::cout << new_temp.scope << std::endl;
                    (yyval.expr) = make_bool_expression(BOOL_EXPR, get_symbol(new_temp.name, get_scope()));
                    emit_if_less(IF_L, (yyvsp[-2].expr), (yyvsp[0].expr), next_quad_label()+3);
                    set_curr_quad_label((next_quad_label()+3));
                    emit_assign(ASSIGN, (yyval.expr), make_constbool_expression(CONSTBOOL_EXPR, 1));
                    emit_jump(JUMP, (next_quad_label()+2));
                    set_curr_quad_label((next_quad_label()+2));
                    emit_assign(ASSIGN, (yyval.expr), make_constbool_expression(CONSTBOOL_EXPR, 0));
                }
#line 2011 "parser.c"
    break;

  case 25: /* expression: expression TOK_GR_EQUAL expression  */
#line 212 "parser.y"
                                                 { 
                    //printf("expression -> expression >= expression\n");
                    //handle_expression($1,$3, get_scope(), yylineno);
                    SymbolType type = GLOBAL;
                    if (get_scope() != 0) {
                        type = LOCAL;
                    }
                    SymbolTableEntry new_temp = new_temp_var(-1, type);
                    //std::cout << new_temp.scope << std::endl;
                    (yyval.expr) = make_bool_expression(BOOL_EXPR, get_symbol(new_temp.name, get_scope()));
                    emit_if_greatereq(IF_GR_EQ, (yyvsp[-2].expr), (yyvsp[0].expr), next_quad_label()+3);
                    set_curr_quad_label((next_quad_label()+3));
                    emit_assign(ASSIGN, (yyval.expr), make_constbool_expression(CONSTBOOL_EXPR, 1));
                    emit_jump(JUMP, (next_quad_label()+2));
                    set_curr_quad_label((next_quad_label()+2));
                    emit_assign(ASSIGN, (yyval.expr), make_constbool_expression(CONSTBOOL_EXPR, 0));
                }
#line 2033 "parser.c"
    break;

  case 26: /* expression: expression TOK_LS_EQUAL expression  */
#line 229 "parser.y"
                                                 {
                    //printf("expression -> expression <= expression\n");
                    //handle_expression($1,$3, get_scope(), yylineno);
                    SymbolType type = GLOBAL;
                    if (get_scope() != 0) {
                        type = LOCAL;
                    }
                    SymbolTableEntry new_temp = new_temp_var(-1, type);
                    //std::cout << new_temp.scope << std::endl;
                    (yyval.expr) = make_bool_expression(BOOL_EXPR, get_symbol(new_temp.name, get_scope()));
                    emit_if_lesseq(IF_L_EQ, (yyvsp[-2].expr), (yyvsp[0].expr), next_quad_label()+3);
                    set_curr_quad_label((next_quad_label()+3));
                    emit_assign(ASSIGN, (yyval.expr), make_constbool_expression(CONSTBOOL_EXPR, 1));
                    emit_jump(JUMP, (next_quad_label()+2));
                    set_curr_quad_label((next_quad_label()+2));
                    emit_assign(ASSIGN, (yyval.expr), make_constbool_expression(CONSTBOOL_EXPR, 0));
                }
#line 2055 "parser.c"
    break;

  case 27: /* expression: expression TOK_EQUAL expression  */
#line 246 "parser.y"
                                              {
                    //printf("expression -> expression == expression\n");
                    //handle_expression($1,$3, get_scope(), yylineno);
                    SymbolType type = GLOBAL;
                    if (get_scope() != 0) {
                        type = LOCAL;
                    }
                    SymbolTableEntry new_temp = new_temp_var(-1, type);
                    //std::cout << new_temp.scope << std::endl;
                    (yyval.expr) = make_bool_expression(BOOL_EXPR, get_symbol(new_temp.name, get_scope()));
                    emit_if_equal(IF_EQ, (yyvsp[-2].expr), (yyvsp[0].expr), next_quad_label()+3);
                    set_curr_quad_label((next_quad_label()+3));
                    emit_assign(ASSIGN, (yyval.expr), make_constbool_expression(CONSTBOOL_EXPR, 1));
                    emit_jump(JUMP, (next_quad_label()+2));
                    set_curr_quad_label((next_quad_label()+2));
                    emit_assign(ASSIGN, (yyval.expr), make_constbool_expression(CONSTBOOL_EXPR, 0));
                }
#line 2077 "parser.c"
    break;

  case 28: /* expression: expression TOK_N_EQUAL expression  */
#line 263 "parser.y"
                                                {
                    //printf("expression -> expression != expression\n");
                    //handle_expression($1,$3, get_scope(), yylineno);
                    SymbolType type = GLOBAL;
                    if (get_scope() != 0) {
                        type = LOCAL;
                    }
                    SymbolTableEntry new_temp = new_temp_var(-1, type);
                    //std::cout << new_temp.scope << std::endl;
                    (yyval.expr) = make_bool_expression(BOOL_EXPR, get_symbol(new_temp.name, get_scope()));
                    emit_if_not_equal(IF_N_EQ, (yyvsp[-2].expr), (yyvsp[0].expr), next_quad_label()+3);
                    set_curr_quad_label((next_quad_label()+3));
                    emit_assign(ASSIGN, (yyval.expr), make_constbool_expression(CONSTBOOL_EXPR, 1));
                    emit_jump(JUMP, (next_quad_label()+2));
                    set_curr_quad_label((next_quad_label()+2));
                    emit_assign(ASSIGN, (yyval.expr), make_constbool_expression(CONSTBOOL_EXPR, 0));
                }
#line 2099 "parser.c"
    break;

  case 29: /* expression: expression TOK_AND expression  */
#line 280 "parser.y"
                                            {
                    //printf("expression -> expression and expression\n");
                    //handle_expression($1,$3, get_scope(), yylineno);
                }
#line 2108 "parser.c"
    break;

  case 30: /* expression: expression TOK_OR expression  */
#line 284 "parser.y"
                                           {
                    //printf("expression -> expression or expression\n");
                    //handle_expression($1,$3, get_scope(), yylineno);
                }
#line 2117 "parser.c"
    break;

  case 31: /* expression: term  */
#line 288 "parser.y"
                   {
                //printf("expression -> term\n");
                (yyval.expr) = (yyvsp[0].expr);
                //std::cout << "$$->type " << $$->type << "\t\t" << $$->symbol->name << std::endl;
            }
#line 2127 "parser.c"
    break;

  case 32: /* term: TOK_L_PARENTH expression TOK_R_PARENTH  */
#line 295 "parser.y"
                                             {
            //printf("term ->(expression)\n");
            (yyval.expr) = (yyvsp[-1].expr);
      }
#line 2136 "parser.c"
    break;

  case 33: /* term: TOK_MINUS expression  */
#line 299 "parser.y"
                                              {
            //printf("term -> - expression\n");
            if (check_if_is_arithm((yyvsp[0].expr))) {
                SymbolType type = GLOBAL;
                if (get_scope() != 0) {
                    type = LOCAL;
                }
                SymbolTableEntry new_temp = new_temp_var(-1, type);
                std::cout << new_temp.scope << std::endl;
                (yyval.expr) = make_arithm_expression(ARITHM_EXPR, get_symbol(new_temp.name, get_scope()));
                emit_arithm_one(UMINUS, (yyval.expr), (yyvsp[0].expr));
            }
      }
#line 2154 "parser.c"
    break;

  case 34: /* term: TOK_NOT expression  */
#line 312 "parser.y"
                            {
            //printf("term -> not expression\n");
            SymbolType type = GLOBAL;
            if (get_scope() != 0) {
                type = LOCAL;
            }
            SymbolTableEntry new_temp = new_temp_var(-1, type);
            std::cout << new_temp.scope << std::endl;
            (yyval.expr) = make_bool_expression(BOOL_EXPR, get_symbol(new_temp.name, get_scope()));
            emit_bool(NOT, (yyval.expr), (yyvsp[0].expr));
      }
#line 2170 "parser.c"
    break;

  case 35: /* term: TOK_DBL_PLUS lvalue  */
#line 323 "parser.y"
                            {
            /*printf("term -> ++lvalue\n");*/ 
            if (handle_assign_expr((yyvsp[0].expr)->symbol->name) == 0) {
                if (check_if_is_arithm((yyvsp[0].expr))) {
                    //std::cout << "--L-VALUE" << std::endl;
                    if ((yyvsp[0].expr)->type == TABLEITEM_EXPR) {
                        (yyval.expr) = emit_if_table_item((yyvsp[0].expr));
                        emit_add(ADD, (yyval.expr), (yyval.expr), make_constnum_expression(CONSTNUM_EXPR, 1));
                        emit_table_set_item(TABLESETELEM, (yyvsp[0].expr), (yyvsp[0].expr)->index, (yyval.expr));
                    } else {
                        //std::cout << "EMIT ADD 1" << std::endl;
                        emit_add(ADD, (yyvsp[0].expr), (yyvsp[0].expr), make_constnum_expression(CONSTNUM_EXPR, 1));
                        SymbolType type = GLOBAL;
                            if (get_scope() != 0) {
                                type = LOCAL;
                            }
                        SymbolTableEntry new_temp = new_temp_var(-1, type);
                        std::cout << new_temp.scope << std::endl;
                        (yyval.expr) = make_arithm_expression(ARITHM_EXPR, get_symbol(new_temp.name, get_scope()));
                        emit_assign(ASSIGN, (yyval.expr), (yyvsp[0].expr));
                        
                    }
                }
            }
      }
#line 2200 "parser.c"
    break;

  case 36: /* term: lvalue TOK_DBL_PLUS  */
#line 348 "parser.y"
                            {
            /*printf("term -> lvalue++\n");*/ 
            if (handle_assign_expr((yyvsp[-1].expr)->symbol->name) == 0) {
                if (check_if_is_arithm((yyvsp[-1].expr))) {
                    SymbolType type = GLOBAL;
                    if (get_scope() != 0) {
                        type = LOCAL;
                    }
                    SymbolTableEntry new_temp = new_temp_var(-1, type);
                    std::cout << new_temp.scope << std::endl;
                    (yyval.expr) = make_lvalue_expression(VAR_EXPR, get_symbol(new_temp.name, get_scope()));
                    if ((yyvsp[-1].expr)->type == TABLEITEM_EXPR) {
                        expression* val = emit_if_table_item((yyvsp[-1].expr));
                        emit_assign(ASSIGN, (yyval.expr), val);
                        emit_add(ADD, val, val, make_constnum_expression(CONSTNUM_EXPR, 1));
                        emit_table_set_item(TABLESETELEM, (yyvsp[-1].expr), (yyvsp[-1].expr)->index, val);
                    } else {
                        std::cout << "EMIT ASSIGN 1" << std::endl;
                        emit_assign(ASSIGN, (yyval.expr), (yyvsp[-1].expr));
                        emit_add(ADD, (yyvsp[-1].expr), (yyvsp[-1].expr), make_constnum_expression(CONSTNUM_EXPR, 1));
                    }
                }
            }
      }
#line 2229 "parser.c"
    break;

  case 37: /* term: TOK_DBL_MINUS lvalue  */
#line 372 "parser.y"
                             {
            /*printf("term -> --lvalue\n");*/ 
            if (handle_assign_expr((yyvsp[0].expr)->symbol->name) == 0) {
                if (check_if_is_arithm((yyvsp[0].expr))) {
                    //std::cout << "--L-VALUE" << std::endl;
                    if ((yyvsp[0].expr)->type == TABLEITEM_EXPR) {
                        (yyval.expr) = emit_if_table_item((yyvsp[0].expr));
                        emit_sub(SUB, (yyval.expr), (yyval.expr), make_constnum_expression(CONSTNUM_EXPR, 1));
                        emit_table_set_item(TABLESETELEM, (yyvsp[0].expr), (yyvsp[0].expr)->index, (yyval.expr));
                    } else {
                        //std::cout << "EMIT ADD 1" << std::endl;
                        emit_sub(SUB, (yyvsp[0].expr), (yyvsp[0].expr), make_constnum_expression(CONSTNUM_EXPR, 1));
                        SymbolType type = GLOBAL;
                        if (get_scope() != 0) {
                            type = LOCAL;
                        }
                        SymbolTableEntry new_temp = new_temp_var(-1, type);
                        std::cout << new_temp.scope << std::endl;
                        (yyval.expr) = make_arithm_expression(ARITHM_EXPR, get_symbol(new_temp.name, get_scope()));
                        emit_assign(ASSIGN, (yyval.expr), (yyvsp[0].expr));
                        
                    }
                }
            }
      }
#line 2259 "parser.c"
    break;

  case 38: /* term: lvalue TOK_DBL_MINUS  */
#line 397 "parser.y"
                             {
            /*printf("term -> lvalue--\n");*/ 
            if (handle_assign_expr((yyvsp[-1].expr)->symbol->name) == 0) {
                if (check_if_is_arithm((yyvsp[-1].expr))) {
                    SymbolType type = GLOBAL;
                    if (get_scope() != 0) {
                        type = LOCAL;
                    }
                    SymbolTableEntry new_temp = new_temp_var(-1, type);
                    std::cout << new_temp.scope << std::endl;
                    (yyval.expr) = make_lvalue_expression(VAR_EXPR, get_symbol(new_temp.name, get_scope()));
                    if ((yyvsp[-1].expr)->type == TABLEITEM_EXPR) {
                        expression* val = emit_if_table_item((yyvsp[-1].expr));
                        emit_assign(ASSIGN, (yyval.expr), val);
                        emit_sub(SUB, val, val, make_constnum_expression(CONSTNUM_EXPR, 1));
                        emit_table_set_item(TABLESETELEM, (yyvsp[-1].expr), (yyvsp[-1].expr)->index, val);
                    } else {
                        std::cout << "EMIT ASSIGN 1" << std::endl;
                        emit_assign(ASSIGN, (yyval.expr), (yyvsp[-1].expr));
                        emit_sub(SUB, (yyvsp[-1].expr), (yyvsp[-1].expr), make_constnum_expression(CONSTNUM_EXPR, 1));
                    }
                }
            }
      }
#line 2288 "parser.c"
    break;

  case 39: /* term: primary  */
#line 421 "parser.y"
                {
            printf("term -> primary\n");
            (yyval.expr) = (yyvsp[0].expr);
            //std::cout << "$$->type " << $$->type << "\t\t" << $$->symbol->name << std::endl;
      }
#line 2298 "parser.c"
    break;

  case 40: /* assignexpr: lvalue TOK_ASSIGN expression  */
#line 428 "parser.y"
                                         {
        printf("assignexpr -> lvalue = expression\n");
        //printf("%s\n", $1->name);
        std::cout << "$3->type " << (yyvsp[0].expr)->type << "\t\t" << (yyvsp[0].expr)->num_const << std::endl;
        if (handle_assign_expr((yyvsp[-2].expr)->symbol->name) == 0) {
            std::cout << "ASSIGN EXPR $$$" << std::endl;
            if ((yyvsp[-2].expr)->type == TABLEITEM_EXPR) {
                //std::cout << "Expression has symbol " << $3->symbol->name << std::endl;
                emit_table_set_item(TABLESETELEM, (yyvsp[-2].expr), (yyvsp[-2].expr)->index, (yyvsp[0].expr));
                (yyval.expr) = emit_if_table_item((yyvsp[-2].expr));
                (yyval.expr)->type = ASSIGN_EXPR;
                //std::cout << "ASSIGN EXPR 2" << std::endl;
            } else {
                std::cout << "ASSIGN EXPR $$$$" << std::endl;
                std::cout << "IN ASSIGN L-VALUE Name is "<< (yyvsp[-2].expr)->symbol->name << std::endl;
                //std::cout << "IN ASSIGN EXPRESSION is " << $3->symbol->name << std::endl;
                if (check_if_is_arithm_alt((yyvsp[-2].expr)) == true && check_if_is_arithm_alt((yyvsp[0].expr)) == true) {
                    std::cout << "ASSIGN EXPR $$$$$" << std::endl;
                    emit_assign(ASSIGN, (yyvsp[-2].expr), (yyvsp[0].expr));

                    SymbolType type = GLOBAL;
                    if (get_scope() != 0) {
                        type = LOCAL;
                    }
                    SymbolTableEntry new_temp = new_temp_var(-1, type);
                    std::cout << new_temp.scope << std::endl;
                    (yyval.expr) = make_assign_expression(ASSIGN_EXPR, get_symbol(new_temp.name, get_scope()));
                    //std::cout << $$->symbol->name << std::endl;
                    //std::cout << $1->type << std::endl;
                    emit_assign(ASSIGN, (yyval.expr), (yyvsp[-2].expr));
                    if (get_scope() > 0) {
                        quad_table_print();
                    }
                } else {
                    std::cout << "Error r-value is not an arithmetic expression" << std::endl;
                }
            }
        }
                                         }
#line 2342 "parser.c"
    break;

  case 41: /* primary: lvalue  */
#line 469 "parser.y"
                {
                        //printf("primary -> lvalue\n");
                        (yyval.expr) = emit_if_table_item((yyvsp[0].expr));
                  }
#line 2351 "parser.c"
    break;

  case 43: /* primary: objectdef  */
#line 474 "parser.y"
                     {
            printf("primary -> objectdef\n");
            std::cout << "OBJECTDEF in PRIMARY" << (yyvsp[0].expr)->symbol->name << std::endl;
            (yyval.expr) = (yyvsp[0].expr);
         }
#line 2361 "parser.c"
    break;

  case 44: /* primary: TOK_L_PARENTH funcdef TOK_R_PARENTH  */
#line 479 "parser.y"
                                               {
            //printf("primary ->(funcdef)\n");
            //std::cout << $2->symbol->name << std::endl;
            //quad_table_print();
            (yyval.expr) = (yyvsp[-1].expr);
         }
#line 2372 "parser.c"
    break;

  case 45: /* primary: const  */
#line 485 "parser.y"
                 {
            printf("primary -> const\n");
            (yyval.expr) = (yyvsp[0].expr);
         }
#line 2381 "parser.c"
    break;

  case 46: /* lvalue: TOK_ID  */
#line 491 "parser.y"
               {//printf("lvalue -> ID\n"); 
                    handle_id((yyvsp[0].strVal), get_scope(), curr_scope_space(), curr_scope_offset(), yylineno);
                    if (get_symbol((yyvsp[0].strVal), get_scope())->type == LIBFUNC) {
                        (yyval.expr) = make_lvalue_expression(LIBFUNC_EXPR, get_symbol((yyvsp[0].strVal), get_scope()));

                    } else if (get_symbol((yyvsp[0].strVal), get_scope())->type == USERFUNC) {
                        (yyval.expr) = make_lvalue_expression(PROGRAMFUNC_EXPR, get_symbol((yyvsp[0].strVal), get_scope()));
                    } else {
                        (yyval.expr) = make_lvalue_expression(VAR_EXPR, get_symbol((yyvsp[0].strVal), get_scope()));
                    }
               }
#line 2397 "parser.c"
    break;

  case 47: /* lvalue: TOK_LOCAL TOK_ID  */
#line 502 "parser.y"
                           {//printf("lvalue -> local id\n");
                    handle_local_id((yyvsp[0].strVal), get_scope(), curr_scope_space(), curr_scope_offset(), yylineno);
                    if (get_symbol((yyvsp[0].strVal), get_scope())->type == LIBFUNC) {
                        (yyval.expr) = make_lvalue_expression(LIBFUNC_EXPR, get_symbol((yyvsp[0].strVal), get_scope()));
                    } else if (get_symbol((yyvsp[0].strVal), get_scope())->type == USERFUNC) {
                        (yyval.expr) = make_lvalue_expression(PROGRAMFUNC_EXPR, get_symbol((yyvsp[0].strVal), get_scope()));
                    } else {
                        (yyval.expr) = make_lvalue_expression(VAR_EXPR, get_symbol((yyvsp[0].strVal), get_scope()));
                    }
                           }
#line 2412 "parser.c"
    break;

  case 48: /* lvalue: TOK_DBL_COLON TOK_ID  */
#line 512 "parser.y"
                                {//printf("lvalue -> :: id\n");
                                    handle_global_access_id((yyvsp[0].strVal));
                                    (yyval.expr) = make_lvalue_expression(VAR_EXPR, get_symbol((yyvsp[0].strVal),0));
                                    /*$$ = (SymbolTableEntry*)malloc(sizeof(SymbolTableEntry));
                                    $$->name = $2;
                                    $$->scope = 0;
                                    $$->line = yylineno;
                                    $$->isActive = true;
                                    $$->type = 1;*/
                                }
#line 2427 "parser.c"
    break;

  case 49: /* lvalue: member  */
#line 522 "parser.y"
                 {
                    //printf("lvalue -> member\n");
                    is_a_member_access = 1;
                    (yyval.expr) = (yyvsp[0].expr);

                 }
#line 2438 "parser.c"
    break;

  case 50: /* member: lvalue TOK_DOT TOK_ID  */
#line 530 "parser.y"
                              {
            printf("member -> lvalue.id\n");
            std::cout << "$3 -> " << (yyvsp[0].strVal) << std::endl;
            //$$ = member_item($1, get_symbol($3, get_scope())->name);
            (yyval.expr) = member_item((yyvsp[-2].expr), (yyvsp[0].strVal));
            //std::cout << "$$->index->str_const " << $$->index->str_const << std::endl;
            is_a_member_access = 0;
        }
#line 2451 "parser.c"
    break;

  case 51: /* member: lvalue TOK_L_BR expression TOK_R_BR  */
#line 538 "parser.y"
                                               {
            //printf("member -> lvalue[expression]\n");
            std::cout << "Expression is : " << (yyvsp[-1].expr)->type << std::endl;
            (yyvsp[-3].expr) = emit_if_table_item((yyvsp[-3].expr));
            if ((yyvsp[-1].expr)->type == CONSTNUM_EXPR) {
                (yyval.expr) = make_table_item_expression(TABLEITEM_EXPR, (yyvsp[-3].expr)->symbol, make_constnum_expression(CONSTNUM_EXPR, (yyvsp[-1].expr)->num_const));
            } else {
                (yyval.expr) = make_table_item_expression(TABLEITEM_EXPR, (yyvsp[-3].expr)->symbol, make_conststring_expression(CONSTSTRING_EXPR, (yyvsp[-1].expr)->symbol->name));
            }
            clear_expression_list();
        }
#line 2467 "parser.c"
    break;

  case 53: /* member: call TOK_L_BR expression TOK_R_BR  */
#line 550 "parser.y"
                                            {
            printf("member -> call[expression]\n");
        }
#line 2475 "parser.c"
    break;

  case 54: /* call: call normcall  */
#line 555 "parser.y"
                    {
            //printf("call -> call normcall\n");
            (yyval.expr) = make_call((yyvsp[-1].expr));
       }
#line 2484 "parser.c"
    break;

  case 55: /* call: lvalue callsuffix  */
#line 559 "parser.y"
                          {
            printf("call -> lvalue callsuffix\n");
            (yyvsp[-1].expr) = emit_if_table_item((yyvsp[-1].expr));
            std::cout << "CALLSUFFIX is " << (yyvsp[-1].expr)->symbol->name << std::endl; 
            if ((yyvsp[0].call_struct).method == 0) {
                std::cout << "$2 Method is " << (yyvsp[0].call_struct).method << std::endl; 
                if ((yyvsp[-1].expr)->type == LIBFUNC_EXPR || (yyvsp[-1].expr)->type == PROGRAMFUNC_EXPR || (yyvsp[-1].expr)->type == VAR_EXPR) {
                    //std::cout << "$2 Method is and $1->type is LIBFUNC_EXPR " << $2.method << std::endl; 
                    //$1 = emit_if_table_item(member_item($1, ""));

                } else {
                    std::cout << "L-VALUE is " << (yyvsp[-1].expr)->symbol->name << std::endl;
                    push_expression_list((yyvsp[-1].expr));
                    get_last_expression();
                    (yyvsp[-1].expr) = emit_if_table_item(member_item((yyvsp[-1].expr), (yyvsp[-1].expr)->index->str_const));
                }
            } else {
                std::cout << "$1->symbol->name " << (yyvsp[-1].expr)->symbol->name << std::endl;
                push_expression_list((yyvsp[-1].expr));
                get_last_expression();
                (yyvsp[-1].expr) = emit_if_table_item(member_item((yyvsp[-1].expr), (yyvsp[0].call_struct).name));
                
            }
            (yyval.expr) = make_call((yyvsp[-1].expr));
            if (get_scope() > 0) {
                quad_table_print();
            }
      }
#line 2517 "parser.c"
    break;

  case 56: /* call: TOK_L_PARENTH funcdef TOK_R_PARENTH normcall  */
#line 587 "parser.y"
                                                     {
            //printf("call -> (funcdef) normcall\n");
            expression* func = make_func_expression(PROGRAMFUNC_EXPR, get_symbol((yyvsp[-2].expr)->symbol->name, get_scope()));
            (yyval.expr) = make_call(func);     
      }
#line 2527 "parser.c"
    break;

  case 57: /* callsuffix: normcall  */
#line 594 "parser.y"
                     {
                //printf("callsuffix -> normcall\n");
                (yyval.call_struct) = (yyvsp[0].call_struct);
            }
#line 2536 "parser.c"
    break;

  case 58: /* callsuffix: methodcall  */
#line 598 "parser.y"
                         {
                //printf("callsuffix -> methodcall\n");
                (yyval.call_struct) = (yyvsp[0].call_struct);
            }
#line 2545 "parser.c"
    break;

  case 59: /* normcall: TOK_L_PARENTH elist TOK_R_PARENTH  */
#line 604 "parser.y"
                                            {
    (yyval.call_struct).elist = (yyvsp[-1].expr);
    (yyval.call_struct).method = 0;
    (yyval.call_struct).name = NULL;

    get_last_expression();
    //pop_expression_list();
}
#line 2558 "parser.c"
    break;

  case 60: /* methodcall: TOK_DBL_DOT TOK_ID normcall  */
#line 614 "parser.y"
                                        {
    //printf("methodcall -> ..id normcall\n");
    (yyval.call_struct).elist = (yyvsp[0].call_struct).elist;
    (yyval.call_struct).method = 1;
    (yyval.call_struct).name = (yyvsp[-1].strVal);

    //get_last_expression();
}
#line 2571 "parser.c"
    break;

  case 61: /* elist: expression  */
#line 624 "parser.y"
                  {
            //printf("elist -> expression\n");
            push_expression_list((yyvsp[0].expr));
        }
#line 2580 "parser.c"
    break;

  case 62: /* elist: elist TOK_COMMA expression  */
#line 628 "parser.y"
                                    {
            //printf("elist -> elist,\n");
            push_expression_list((yyvsp[0].expr));
        }
#line 2589 "parser.c"
    break;

  case 63: /* elist: %empty  */
#line 632 "parser.y"
                 {
            //printf("elist -> \n");
            //push_expression_list(NULL);
        }
#line 2598 "parser.c"
    break;

  case 64: /* indexed: indexedelem  */
#line 638 "parser.y"
                     {
            //printf("elist -> expression\n");
            (yyval.expr) = (yyvsp[0].expr);
        }
#line 2607 "parser.c"
    break;

  case 65: /* indexed: indexed TOK_COMMA indexedelem  */
#line 642 "parser.y"
                                       {
            //printf("elist -> elist,\n");
            (yyval.expr) = (yyvsp[-2].expr);
        }
#line 2616 "parser.c"
    break;

  case 66: /* objectdef: TOK_L_BR elist TOK_R_BR  */
#line 648 "parser.y"
                                   {
                std::cout << "OBJECTDEF" << std::endl;
                //printf("objectdef -> [elist]\n");
                SymbolType type = GLOBAL;
                if (get_scope() != 0) {
                    type = LOCAL;
                }
                SymbolTableEntry new_temp = new_temp_var(-1, type);
                //std::cout << new_temp.name << std::endl;
                expression* table = make_new_table_expression(NEWTABLE_EXPR, get_symbol(new_temp.name, get_scope()));
                emit_table_create(TABLECREATE, table);
                set_elist_expression(table);
                (yyval.expr) = table;
          }
#line 2635 "parser.c"
    break;

  case 67: /* objectdef: TOK_L_BR indexed TOK_R_BR  */
#line 662 "parser.y"
                                     {
                printf("objectdef -> [indexed]\n");
                SymbolType type = GLOBAL;
                if (get_scope() != 0) {
                    type = LOCAL;
                }
                SymbolTableEntry new_temp = new_temp_var(-1, type);
                std::cout << new_temp.name << std::endl;
                expression* table = make_new_table_expression(NEWTABLE_EXPR, get_symbol(new_temp.name, get_scope()));
                emit_table_create(TABLECREATE, table);
                set_indexed_map(table);
                (yyval.expr) = table;

         }
#line 2654 "parser.c"
    break;

  case 68: /* indexedelem: TOK_L_CURLY_BR expression TOK_COLON expression TOK_R_CURLY_BR  */
#line 679 "parser.y"
               {
    printf("indexedelem -> {expression:expression}\n");
    std::cout << (yyvsp[-3].expr)->str_const << (yyvsp[-1].expr)->type << std::endl;
    insert_indexed_map((yyvsp[-3].expr),(yyvsp[-1].expr));
    (yyval.expr) = (yyvsp[-3].expr);
}
#line 2665 "parser.c"
    break;

  case 69: /* $@1: %empty  */
#line 687 "parser.y"
                      {
    push_blocks_prec(1);
    push_vector();
    is_in_block_mode++;
    scope_counter++;
    set_prev_scope_space_counter(get_scope_space_counter());
    set_scope_space_counter(1);
/*std::cout<<scope_counter<<std::endl;*/}
#line 2678 "parser.c"
    break;

  case 70: /* block: TOK_L_CURLY_BR $@1 statements TOK_R_CURLY_BR  */
#line 695 "parser.y"
{/*printf("block -> {statements}\n");*/ is_in_block_mode--; handle_block_end(get_scope()); printf("Prev Scope Space Counter: %d\n", get_prev_scope_space_counter()); set_scope_space_counter(get_prev_scope_space_counter()); printf("Scope Space Counter: %d\n", get_scope_space_counter()); scope_counter--;
/*std::cout<< "Scope Counter is " << scope_counter<<std::endl;*/}
#line 2685 "parser.c"
    break;

  case 71: /* $@2: %empty  */
#line 699 "parser.y"
                              {push_blocks_prec(2);set_scope_space_counter(2); push_func_local_offset_table();}
#line 2691 "parser.c"
    break;

  case 72: /* funcdef_block: TOK_L_CURLY_BR $@2 statements TOK_R_CURLY_BR  */
#line 699 "parser.y"
                                                                                                                                          {
        /*printf("funcdef_block -> {statements}\n");*/
        
        handle_funcdef_block_end(get_scope());
        //scope_counter--; 
        is_in_function_mode = 0;
        //std::cout<<scope_counter<<std::endl;
        printf("The current scope space is: %d\n", curr_scope_space());

        reset_formal_arg_offset();
    }
#line 2707 "parser.c"
    break;

  case 73: /* $@3: %empty  */
#line 712 "parser.y"
                                           {
    
    is_in_function_mode = 1;
    handle_funcdef_w_name((yyvsp[-1].strVal), get_scope(), curr_scope_space(), curr_scope_offset(), yylineno);
    //printf("%s", get_symbol($2, get_scope())->name);
    //$$ = make_func_expression(PROGRAMFUNC_EXPR, get_symbol($2, get_scope()));
    emit_funcdef(FUNCSTART, make_func_expression(PROGRAMFUNC_EXPR, get_symbol((yyvsp[-1].strVal), get_scope())));
    scope_counter++;
    set_prev_scope_space_counter(get_scope_space_counter());
    set_scope_space_counter(3);
    push_formal_arg_offset_table();
    /*std::cout<<scope_counter<<std::endl;*/}
#line 2724 "parser.c"
    break;

  case 74: /* funcdef: TOK_FUNCTION TOK_ID TOK_L_PARENTH $@3 idlist TOK_R_PARENTH funcdef_block  */
#line 724 "parser.y"
              {
                   //printf("funcdef -> function id (idlist) funcdef_block\n");
                   //handle_funcdef_block_end(get_scope());
                   scope_counter--;
                   printf("Prev Scope Space Counter: %d\n", get_prev_scope_space_counter()); 
                   set_scope_space_counter(get_prev_scope_space_counter());
                   printf("Scope Space Counter: %d\n", get_scope_space_counter());
                   pop_func_local_offset_table();
                   reset_func_local_offset();
                   emit_funcdef(FUNCEND, make_func_expression(PROGRAMFUNC_EXPR, get_symbol((yyvsp[-5].strVal), get_scope())));
                   (yyval.expr) = make_func_expression(PROGRAMFUNC_EXPR, get_symbol((yyvsp[-5].strVal), get_scope()));
              }
#line 2741 "parser.c"
    break;

  case 75: /* $@4: %empty  */
#line 736 "parser.y"
                                      {
         is_in_function_mode = 1;
         //name = func_name_generator();
         handle_funcdef_anonym_name(get_scope(), curr_scope_space(), curr_scope_offset(), yylineno);
         emit_funcdef(FUNCSTART, make_func_expression(PROGRAMFUNC_EXPR, get_symbol(name, get_scope())));
         scope_counter++;
         set_prev_scope_space_counter(get_scope_space_counter());
         set_scope_space_counter(3);
         push_formal_arg_offset_table();
/*std::cout<<scope_counter<<std::endl;*/}
#line 2756 "parser.c"
    break;

  case 76: /* funcdef: TOK_FUNCTION TOK_L_PARENTH $@4 idlist TOK_R_PARENTH funcdef_block  */
#line 746 "parser.y"
              {
                    //printf("funcdef -> function (idlist) funcdef_block\n");
                    //handle_funcdef_block_end(get_scope());
                    scope_counter--;
                    printf("Prev Scope Space Counter: %d\n", get_prev_scope_space_counter()); 
                    set_scope_space_counter(get_prev_scope_space_counter());
                    printf("Scope Space Counter: %d\n", get_scope_space_counter());
                    pop_func_local_offset_table();
                    reset_func_local_offset();
                    emit_funcdef(FUNCEND, make_func_expression(PROGRAMFUNC_EXPR, get_symbol(name, get_scope())));
                    //exit_scope_space();
                    //exit_scope_space();
                    //printf("The current scope space is: %d\n", curr_scope_space());
                    (yyval.expr) = make_func_expression(PROGRAMFUNC_EXPR, get_symbol(name, get_scope()));
              }
#line 2776 "parser.c"
    break;

  case 77: /* const: TOK_INT  */
#line 763 "parser.y"
               {
            printf("const -> integer %d\n", (yyvsp[0].intVal)); 
            (yyval.expr) = make_constnum_expression(CONSTNUM_EXPR, (yyvsp[0].intVal));
        }
#line 2785 "parser.c"
    break;

  case 78: /* const: TOK_FLOAT  */
#line 767 "parser.y"
                   {
            printf("const -> float %f\n", (yyvsp[0].floatVal));
            (yyval.expr) = make_constnum_expression(CONSTNUM_EXPR, (yyvsp[0].floatVal));
        }
#line 2794 "parser.c"
    break;

  case 79: /* const: TOK_STRING  */
#line 771 "parser.y"
                    {
            //printf("const -> string %s\n", $1);
            std::cout << "CONST -> STRING " << (yyvsp[0].strVal) << std::endl;
            (yyval.expr) = make_conststring_expression(CONSTSTRING_EXPR, (yyvsp[0].strVal));
        }
#line 2804 "parser.c"
    break;

  case 80: /* const: TOK_NIL  */
#line 776 "parser.y"
                 {
            //printf("const -> nil\n");
            (yyval.expr) = make_nil_expression(NIL_EXPR);
        }
#line 2813 "parser.c"
    break;

  case 81: /* const: TOK_TRUE  */
#line 780 "parser.y"
                  {
            //printf("const -> true\n");
            (yyval.expr) = make_constbool_expression(CONSTBOOL_EXPR, (yyvsp[0].intVal));
        }
#line 2822 "parser.c"
    break;

  case 82: /* const: TOK_FALSE  */
#line 784 "parser.y"
                   {
            //printf("const -> false\n");
            (yyval.expr) = make_constbool_expression(CONSTBOOL_EXPR, (yyvsp[0].intVal));
        }
#line 2831 "parser.c"
    break;

  case 83: /* idlist: TOK_ID  */
#line 790 "parser.y"
               {
                    //printf("%s", $1);
                    handle_func_w_1arg((yyvsp[0].strVal), get_scope(), curr_scope_space(), curr_scope_offset(), yylineno);

               }
#line 2841 "parser.c"
    break;

  case 84: /* idlist: idlist TOK_COMMA TOK_ID  */
#line 795 "parser.y"
                                  {
                                       //printf("%s", $3);
                                       handle_func_w_1arg((yyvsp[0].strVal), get_scope(), curr_scope_space(), curr_scope_offset(), yylineno);
                                  }
#line 2850 "parser.c"
    break;

  case 85: /* idlist: %empty  */
#line 799 "parser.y"
                 { push_vector();}
#line 2856 "parser.c"
    break;


#line 2860 "parser.c"

        default: break;
      }
    if (yychar_backup != yychar)
      YY_LAC_DISCARD ("yychar change");
  }
  /* User semantic actions sometimes alter yychar, and that requires
     that yytoken be updated with the new translation.  We take the
     approach of translating immediately before every use of yytoken.
     One alternative is translating here after every semantic action,
     but that translation would be missed if the semantic action invokes
     YYABORT, YYACCEPT, or YYERROR immediately after altering yychar or
     if it invokes YYBACKUP.  In the case of YYABORT or YYACCEPT, an
     incorrect destructor might then be invoked immediately.  In the
     case of YYERROR or YYBACKUP, subsequent parser actions might lead
     to an incorrect destructor call or verbose syntax error message
     before the lookahead is translated.  */
  YY_SYMBOL_PRINT ("-> $$ =", YY_CAST (yysymbol_kind_t, yyr1[yyn]), &yyval, &yyloc);

  YYPOPSTACK (yylen);
  yylen = 0;

  *++yyvsp = yyval;

  /* Now 'shift' the result of the reduction.  Determine what state
     that goes to, based on the state we popped back to and the rule
     number reduced by.  */
  {
    const int yylhs = yyr1[yyn] - YYNTOKENS;
    const int yyi = yypgoto[yylhs] + *yyssp;
    yystate = (0 <= yyi && yyi <= YYLAST && yycheck[yyi] == *yyssp
               ? yytable[yyi]
               : yydefgoto[yylhs]);
  }

  goto yynewstate;


/*--------------------------------------.
| yyerrlab -- here on detecting error.  |
`--------------------------------------*/
yyerrlab:
  /* Make sure we have latest lookahead translation.  See comments at
     user semantic actions for why this is necessary.  */
  yytoken = yychar == YYEMPTY ? YYSYMBOL_YYEMPTY : YYTRANSLATE (yychar);
  /* If not already recovering from an error, report this error.  */
  if (!yyerrstatus)
    {
      ++yynerrs;
      {
        yypcontext_t yyctx
          = {yyssp, yyesa, &yyes, &yyes_capacity, yytoken};
        char const *yymsgp = YY_("syntax error");
        int yysyntax_error_status;
        if (yychar != YYEMPTY)
          YY_LAC_ESTABLISH;
        yysyntax_error_status = yysyntax_error (&yymsg_alloc, &yymsg, &yyctx);
        if (yysyntax_error_status == 0)
          yymsgp = yymsg;
        else if (yysyntax_error_status == -1)
          {
            if (yymsg != yymsgbuf)
              YYSTACK_FREE (yymsg);
            yymsg = YY_CAST (char *,
                             YYSTACK_ALLOC (YY_CAST (YYSIZE_T, yymsg_alloc)));
            if (yymsg)
              {
                yysyntax_error_status
                  = yysyntax_error (&yymsg_alloc, &yymsg, &yyctx);
                yymsgp = yymsg;
              }
            else
              {
                yymsg = yymsgbuf;
                yymsg_alloc = sizeof yymsgbuf;
                yysyntax_error_status = YYENOMEM;
              }
          }
        yyerror (yymsgp);
        if (yysyntax_error_status == YYENOMEM)
          YYNOMEM;
      }
    }

  if (yyerrstatus == 3)
    {
      /* If just tried and failed to reuse lookahead token after an
         error, discard it.  */

      if (yychar <= YYEOF)
        {
          /* Return failure if at end of input.  */
          if (yychar == YYEOF)
            YYABORT;
        }
      else
        {
          yydestruct ("Error: discarding",
                      yytoken, &yylval);
          yychar = YYEMPTY;
        }
    }

  /* Else will try to reuse lookahead token after shifting the error
     token.  */
  goto yyerrlab1;


/*---------------------------------------------------.
| yyerrorlab -- error raised explicitly by YYERROR.  |
`---------------------------------------------------*/
yyerrorlab:
  /* Pacify compilers when the user code never invokes YYERROR and the
     label yyerrorlab therefore never appears in user code.  */
  if (0)
    YYERROR;
  ++yynerrs;

  /* Do not reclaim the symbols of the rule whose action triggered
     this YYERROR.  */
  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);
  yystate = *yyssp;
  goto yyerrlab1;


/*-------------------------------------------------------------.
| yyerrlab1 -- common code for both syntax error and YYERROR.  |
`-------------------------------------------------------------*/
yyerrlab1:
  yyerrstatus = 3;      /* Each real token shifted decrements this.  */

  /* Pop stack until we find a state that shifts the error token.  */
  for (;;)
    {
      yyn = yypact[yystate];
      if (!yypact_value_is_default (yyn))
        {
          yyn += YYSYMBOL_YYerror;
          if (0 <= yyn && yyn <= YYLAST && yycheck[yyn] == YYSYMBOL_YYerror)
            {
              yyn = yytable[yyn];
              if (0 < yyn)
                break;
            }
        }

      /* Pop the current state because it cannot handle the error token.  */
      if (yyssp == yyss)
        YYABORT;


      yydestruct ("Error: popping",
                  YY_ACCESSING_SYMBOL (yystate), yyvsp);
      YYPOPSTACK (1);
      yystate = *yyssp;
      YY_STACK_PRINT (yyss, yyssp);
    }

  /* If the stack popping above didn't lose the initial context for the
     current lookahead token, the shift below will for sure.  */
  YY_LAC_DISCARD ("error recovery");

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END


  /* Shift the error token.  */
  YY_SYMBOL_PRINT ("Shifting", YY_ACCESSING_SYMBOL (yyn), yyvsp, yylsp);

  yystate = yyn;
  goto yynewstate;


/*-------------------------------------.
| yyacceptlab -- YYACCEPT comes here.  |
`-------------------------------------*/
yyacceptlab:
  yyresult = 0;
  goto yyreturnlab;


/*-----------------------------------.
| yyabortlab -- YYABORT comes here.  |
`-----------------------------------*/
yyabortlab:
  yyresult = 1;
  goto yyreturnlab;


/*-----------------------------------------------------------.
| yyexhaustedlab -- YYNOMEM (memory exhaustion) comes here.  |
`-----------------------------------------------------------*/
yyexhaustedlab:
  yyerror (YY_("memory exhausted"));
  yyresult = 2;
  goto yyreturnlab;


/*----------------------------------------------------------.
| yyreturnlab -- parsing is finished, clean up and return.  |
`----------------------------------------------------------*/
yyreturnlab:
  if (yychar != YYEMPTY)
    {
      /* Make sure we have latest lookahead translation.  See comments at
         user semantic actions for why this is necessary.  */
      yytoken = YYTRANSLATE (yychar);
      yydestruct ("Cleanup: discarding lookahead",
                  yytoken, &yylval);
    }
  /* Do not reclaim the symbols of the rule whose action triggered
     this YYABORT or YYACCEPT.  */
  YYPOPSTACK (yylen);
  YY_STACK_PRINT (yyss, yyssp);
  while (yyssp != yyss)
    {
      yydestruct ("Cleanup: popping",
                  YY_ACCESSING_SYMBOL (+*yyssp), yyvsp);
      YYPOPSTACK (1);
    }
#ifndef yyoverflow
  if (yyss != yyssa)
    YYSTACK_FREE (yyss);
#endif
  if (yyes != yyesa)
    YYSTACK_FREE (yyes);
  if (yymsg != yymsgbuf)
    YYSTACK_FREE (yymsg);
  return yyresult;
}

#line 822 "parser.y"


int main(int argc, char** argv) {
    //struct alpha_token_t *token = (struct
//alpha_token_t*)malloc(sizeof(struct alpha_token_t));
    init_vector();
    if (argc > 1) {
        if (!(yyin = fopen(argv[1], "r"))) {
            fprintf(stderr, "This file is not readable: %s\n", argv[1]);
            return 1;
        } else {
            yyparse();
        }
    }
    quad_table_print();
    printf("\n");
    print_symtable();
    print_symtable_inactive();
    printf("\n");
    //print_expression_list();
    printf("\n");
    //print_reversed_expression_list();


    return 0;
}

void yyerror(const char* s) {
    fprintf(stderr, "Parse error: %s at line %d\n", s, yylineno);
    exit(1);
}
