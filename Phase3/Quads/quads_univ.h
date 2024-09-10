#ifndef _QUADS_UNIV_H
#define _QUADS_UNIV_H

#include <iostream>
#include <string>
#include <vector>
#include <limits.h>
#include "symbol_table.h"

int temp_counter = 0;

enum iopcode {
    ASSIGN,         ADD,           SUB,
    MUL,            DIV,           MOD,
    UMINUS,         AND,           OR,
    NOT,            IF_EQ,         IF_N_EQ,
    IF_L_EQ,        IF_GR_EQ,      IF_L,
    IF_GR,          CALL,          PARAM,
    RET,            GETRETVAL,     FUNCSTART,
    FUNCEND,        TABLECREATE,
    TABLEGETELEM,   TABLESETELEM
};

enum expr_value {
	VAR_EXPR, 
	TABLEITEM_EXPR,
	
	PROGRAMFUNC_EXPR,
	LIBFUNC_EXPR,

	ARITM_EXPR,
	BOOL_EXPR,
	ASSIGN_EXPR,
	NEWTABLE_EXPR,

	COSTNUM_EXPR,
	CONSTBOOL_EXPR,
	CONSTSTRING_EXPR,

	NIL_EXPR
};

struct expression;
struct quad;

quad make_quad(enum iopcode, expression*, expression*, 
               expression*, unsigned, unsigned);

expression* make_var_num_expression(expr_value type, SymbolTableEntry symbol, double num_const); 

void emit(enum iopcode, expression*, expression*, 
	      expression*, unsigned, unsigned);

void quad_table_print();

std::string new_temp_name();
void reset_temp_counter();
SymbolTableEntry new_temp_var(int, enum SymbolType);

#endif
