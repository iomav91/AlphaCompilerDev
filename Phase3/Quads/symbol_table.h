#ifndef _SYMBOL_TABLE_H
#define _SYMBOL_TABLE_H

#include <iostream>
#include <string>
#include <stack>
#include <vector>
#include <map>
#include <limits.h>
#include <assert.h>

int temp_counter = 0;

enum scope_space {
    program_var,
    function_local,
    formal_arg
};

unsigned program_var_offset = 0;
unsigned function_local_offset = 0;
unsigned formal_arg_offset = 0;
unsigned scope_space_counter = 1;

enum SymbolType {
    GLOBAL, LOCAL, FORMAL, USERFUNC, LIBFUNC
};

struct Key;
struct SymbolTableEntry;

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

Key make_key(const std::string &symbol_name, const int& symbol_scope);
SymbolTableEntry make_entry(const std::string&, const int&, const int&, scope_space, unsigned, SymbolType);
//SymbolTableEntry make_func_entry(const std::string&, SymbolType);

void insert(std::string, int, int, scope_space, unsigned, SymbolType);
bool lookup_at_scope(std::string, int);
bool lookup(std::string);
SymbolTableEntry lookup_and_ret_sym(std::string, int);
int lookup_and_return_scope(std::string);
int lookup_and_return_scope_second(std::string, int);
int is_a_func_name(std::string);
void hide();
int is_symbol_active(std::string, int);
int get_vector_size();

//std::stack<std::map<std::string, SymbolTableEntry>> symbol_table;
//std::vector<std::map<std::string, SymbolTableEntry>> symbol_table_alt;
int get_scope();
int is_scope_change(int, int);
//void init_stack();
//void edit_stack(int, std::string, int, int, SymbolType);


void init_vector();
void push_vector();
void push_vector_inactive(std::map<std::string, SymbolTableEntry>);
void edit_vector(int, std::string, int, int, SymbolType);
void print_symtable();
void print_symtable_inactive();
//std::vector<std::map<std::string, SymbolTableEntry>> get_symtable();
std::map<std::string, SymbolTableEntry> insert_lib_function(std::map<std::string, SymbolTableEntry>);

// Phase 3

quad make_quad(iopcode, std::string, std::string, 
               std::string, unsigned, int);

expression* make_var_num_expression(expr_value, SymbolTableEntry symbol, double num_const); 

void emit(iopcode, std::string, std::string, 
          std::string, unsigned, unsigned);

void quad_table_print();

std::string new_temp_name();
void reset_temp_counter();
SymbolTableEntry new_temp_var(int, SymbolType);

enum scope_space curr_scope_space();
unsigned curr_scope_offset();
void inc_curr_scope_offset();
void enter_scope_space();
void exit_scope_space();

std::string get_space_to_word(unsigned);

#endif