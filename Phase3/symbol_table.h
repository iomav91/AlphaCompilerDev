#ifndef _SYMBOL_TABLE_H
#define _SYMBOL_TABLE_H

#include <iostream>
#include <string>
#include <stack>
#include <vector>
#include <map>
#include <climits>
#include <assert.h>

//int temp_counter = 0;

enum scope_space {
    program_var,
    function_local,
    formal_arg
};

/*unsigned program_var_offset = 0;
unsigned function_local_offset = 0;
unsigned formal_arg_offset = 0;
unsigned scope_space_counter = 1;*/

enum SymbolType {
    GLOBAL, LOCAL, FORMAL, USERFUNC, LIBFUNC
};

enum iopcode {
    ASSIGN,         ADD,           SUB,
    MUL,            DIV,           MOD,
    UMINUS,         AND,           OR,
    NOT,            IF_EQ,         IF_N_EQ,
    IF_L_EQ,        IF_GR_EQ,      IF_L,
    IF_GR,          CALL,          PARAM,
    RET,            GETRETVAL,     FUNCSTART,
    FUNCEND,        TABLECREATE,   TABLEGETELEM,   
    TABLESETELEM,   JUMP
};

enum expr_value {
    VAR_EXPR, 
    TABLEITEM_EXPR,
    
    PROGRAMFUNC_EXPR,
    LIBFUNC_EXPR,

    ARITHM_EXPR,
    BOOL_EXPR,
    ASSIGN_EXPR,
    NEWTABLE_EXPR,

    CONSTNUM_EXPR,
    CONSTBOOL_EXPR,
    CONSTSTRING_EXPR,

    NIL_EXPR
};

struct Key {
    std::string name;
    int scope;
};

struct SymbolTableEntry {
    bool              isActive;
    enum SymbolType   type;
    std::string       name;
    enum scope_space  space;
    unsigned          offset;
    unsigned          scope;
    unsigned          i_address;
    unsigned          total_locals;
    std::string       func_name;
    int               line;
};

struct expression {
    enum expr_value     type;
    SymbolTableEntry*   symbol;
    expression*         index;
    double              num_const; 
    std::string         str_const;
    int                 bool_const;
    expression*         next;         
};

/*struct quad {
    enum iopcode        op;
    expression*         result;
    expression*         arg1;
    expression*         arg2;
    unsigned            label;
    unsigned            line;
};*/

Key make_key(const std::string &symbol_name, const int& symbol_scope);
SymbolTableEntry* make_entry(const std::string&, const int&, const int&, scope_space, unsigned, SymbolType);
//SymbolTableEntry make_func_entry(const std::string&, SymbolType);

void insert(std::string, int, int, scope_space, unsigned, SymbolType);
SymbolTableEntry* get_symbol(std::string, int);
SymbolTableEntry* get_symbol_inactive(std::string, int);
bool lookup_at_scope(std::string, int);
bool lookup(std::string);
SymbolTableEntry* lookup_and_ret_sym(std::string, int);
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
void push_vector_inactive(std::map<std::string, SymbolTableEntry*>);
void edit_vector(int, std::string, int, int, SymbolType);
void print_symtable();
void print_symtable_inactive();
//std::vector<std::map<std::string, SymbolTableEntry>> get_symtable();
std::map<std::string, SymbolTableEntry*> insert_lib_function(std::map<std::string, SymbolTableEntry*>);

SymbolTableEntry* new_temp_var(int, SymbolType);

#endif