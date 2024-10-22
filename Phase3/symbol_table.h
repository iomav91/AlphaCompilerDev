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
    int               line;
};

struct expression {
    enum expr_value     type;
    SymbolTableEntry*   symbol;
    expression*         index;
    double              num_const; 
    std::string         str_const;
    int                 bool_const;
    //expression*         next_expr;         
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
SymbolTableEntry make_entry(const std::string&, const int&, const int&, scope_space, unsigned, SymbolType);
//SymbolTableEntry make_func_entry(const std::string&, SymbolType);

void insert(std::string, int, int, scope_space, unsigned, SymbolType);
SymbolTableEntry* get_symbol(std::string, int);
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

SymbolTableEntry new_temp_var(int, SymbolType);

// Phase 3

/*quad make_quad(iopcode, expression*, expression*, 
               expression*, unsigned, int);
expression* make_call(expression*);
void set_elist_expression(expression*);
void set_indexed_map(expression*);

void emit_arithm_one(iopcode, expression*, expression*);
void emit_bool(iopcode, expression*, expression*);
void emit_assign(iopcode, expression*, expression*);
void emit_add(iopcode, expression*, expression*, expression*);
void emit_sub(iopcode, expression*, expression*, expression*);
void emit_mul(iopcode, expression*, expression*, expression*);
void emit_div(iopcode, expression*, expression*, expression*);
void emit_mod(iopcode, expression*, expression*, expression*);

void emit_jump(iopcode, unsigned);
void emit_if_greater(iopcode, expression*, expression*, unsigned);
void emit_if_less(iopcode, expression*, expression*, unsigned);
void emit_if_greatereq(iopcode, expression*, expression*, unsigned);
void emit_if_lesseq(iopcode, expression*, expression*, unsigned);
void emit_if_equal(iopcode, expression*, expression*, unsigned);
void emit_if_not_equal(iopcode, expression*, expression*, unsigned);

void emit_table_get_item(iopcode, expression*, expression*, expression*);
void emit_table_set_item(iopcode, expression*, expression*, expression*);
expression* emit_if_table_item(expression*);
void emit_funcdef(iopcode, expression*);
void emit_param(iopcode, expression*);
void emit_call(iopcode, expression*);
void emit_get_ret_val(iopcode, expression*);
void emit_table_create(iopcode op, expression*);

expression* make_arithm_expression(expr_value, SymbolTableEntry*);
expression* make_bool_expression(expr_value, SymbolTableEntry*); 
expression* make_lvalue_expression(expr_value, SymbolTableEntry*);
expression* make_assign_expression(expr_value, SymbolTableEntry*);
expression* make_table_item_expression(expr_value, SymbolTableEntry*, expression*);
expression* make_constnum_expression(expr_value, double);
expression* make_conststring_expression(expr_value, std::string);
expression* make_constbool_expression(expr_value, int);
expression* make_nil_expression(expr_value);
expression* member_item(expression*, std::string);
expression* make_func_expression(expr_value, SymbolTableEntry*);
expression* make_new_table_expression(expr_value, SymbolTableEntry*); 

void insert_indexed_map(expression*, expression*);
void clear_expression_list();
void push_expression_list(expression*);
void pop_expression_list();
void print_expression_list();
void get_last_expression();
void print_reversed_expression_list();
void print_indexed_map();

void quad_table_print();

std::string new_temp_name();
void reset_temp_counter();
SymbolTableEntry new_temp_var(int, SymbolType);

void set_scope_space_counter(int);
int get_scope_space_counter();
void set_prev_scope_space_counter(int);
int get_prev_scope_space_counter();
enum scope_space curr_scope_space();
unsigned curr_scope_offset();
void inc_curr_scope_offset();
void enter_scope_space();
void exit_scope_space();
void reset_scope_space();
std::string get_space_to_word(unsigned);

void push_func_local_offset_table();

void pop_func_local_offset_table();

unsigned get_func_local_offset_table();

void set_func_local_offset_table();

void reset_func_local_offset();

void push_formal_arg_offset_table();

void pop_formal_arg_offset_table();

unsigned get_formal_arg_offset_table();

void set_formal_arg_offset_table();

void reset_formal_arg_offset();

bool check_if_is_arithm(expression*);

bool check_if_is_arithm_alt(expression*);

void set_curr_quad_label(unsigned);

unsigned next_quad_label();*/

#endif