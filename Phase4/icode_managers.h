#ifndef _ICODE_MANAGERS_H
#define _ICODE_MANAGERS_H

#include <iostream>
#include <string>
#include <list>
#include "symbol_table.h"
struct quad {
    enum iopcode        op;
    expression*         result;
    expression*         arg1;
    expression*         arg2;
    unsigned            label;
    unsigned            line;
    unsigned            taddress;
};

struct break_stmt {
    int pos;
    unsigned label;
};

struct continue_stmt {
    int pos;
    unsigned label;
};

struct return_stmt {
    int pos;
    unsigned label;
};

// Phase 3
void clear_quads_file();
quad make_quad(iopcode, expression*, expression*, 
               expression*, unsigned, int);
expression* make_call(expression*, expression*);
expression* make_call_in(expression*, std::vector<expression*>);
void set_elist_expression(expression*);
void set_indexed_map(expression*);

void emit_arithm_one(iopcode, expression*, expression*);
void emit_bool(iopcode, expression*, expression*, expression*);
void emit_assign(iopcode, expression*, expression*);
void emit_add(iopcode, expression*, expression*, expression*);
void emit_sub(iopcode, expression*, expression*, expression*);
void emit_mul(iopcode, expression*, expression*, expression*);
void emit_div(iopcode, expression*, expression*, expression*);
void emit_mod(iopcode, expression*, expression*, expression*);

void emit_jump(iopcode, unsigned);
void emit_return(iopcode, expression*);
void emit_if_greater(iopcode, expression*, expression*, expression*, unsigned);
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
expression* get_last_expression();
void print_reversed_expression_list();
void print_indexed_map();

void quad_table_print();
void quad_creation(int, expression* result, expression* arg1, expression* arg2, unsigned, unsigned);

std::string new_temp_name(int);
int reset_temp_counter(int);
SymbolTableEntry* new_temp_var(int, SymbolType);

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

unsigned next_quad_label();
expression* manage_expr_and_expr(expression*, expression*, unsigned, expression*);
expression* manage_expr_or_expr(expression*, expression*, unsigned, expression*);
expression* manage_expr_plus_expr(expression*, expression*, expression*);
expression* manage_expr_minus_expr(expression*, expression*, expression*);
expression* manage_expr_mul_expr(expression*, expression*, expression*);
expression* manage_expr_div_expr(expression*, expression*, expression*);
expression* manage_expr_mod_expr(expression*, expression*, expression*);
expression* manage_expr_gr_expr(expression*, expression*, expression*);
expression* manage_expr_ls_expr(expression*, expression*, expression*);
expression* manage_expr_gr_eq_expr(expression*, expression*, expression*);
expression* manage_expr_ls_eq_expr(expression*, expression*, expression*);
expression* manage_expr_eq_expr(expression*, expression*, expression*);
expression* manage_expr_not_eq_expr(expression*, expression*, expression*);
expression* manage_uminus_expr(expression*, expression*);
expression* manage_not_expr(expression*, expression*);
expression* manage_db_plus_lvalue(expression*, expression*);
expression* manage_lvalue_db_plus(expression*, expression*);
expression* manage_db_minus_lvalue(expression*, expression*); 
expression* manage_lvalue_db_minus(expression*, expression*);
expression* manage_assign_expr(expression*, expression*, expression*);
expression* manage_lvalue_id(expression*, std::string, int);
expression* manage_lvalue_local_id(expression*, std::string, int);
expression* manage_lvalue_global_id(expression*, std::string, int);
expression* manage_lvalue_dot_id(expression*, expression*, std::string);
expression* manage_lvalue_lbr_expr_rbr(expression*, expression*, expression*);
expression* manage_call_dot_id();
expression* manage_call_lbr_expr_rbr();
expression* manage_call_normcall(expression*);
expression* manage_lvalue_callsuffix(expression*, expression*, std::string, expression*, int);
expression* manage_lpar_funcdef_rpar_normcall(expression*);
expression* manage_lpar_elist_rpar(expression*);
expression* manage_db_dot_id_normcall(expression*);
expression* manage_lbr_elist_rbr(expression*, expression*);
expression* manage_lbr_indexed_rbr(expression*, indexed_elem*);
expression* manage_lcbr_expr_rcbr_expr(expression*, expression*);
SymbolTableEntry* manage_funcprefix(SymbolTableEntry*, std::string, int);
SymbolTableEntry* manage_funcprefix_anonym(SymbolTableEntry*, int);
expression* manage_funcdef(SymbolTableEntry*, unsigned);
unsigned manage_ifprefix(unsigned, expression*);
unsigned manage_elseprefix(unsigned);

void push_breaklist();
void add_breakelem(unsigned, unsigned);
void pop_breaklist();
void push_contlist();
void add_contelem(unsigned, unsigned);
void pop_contlist();
void push_returnlist();
void add_returnelem(unsigned, unsigned);
void pop_returnlist();
unsigned breaklist_size();
unsigned contlist_size();
void patchlabel(unsigned, unsigned);
unsigned newlist(unsigned);
unsigned mergelist(unsigned, unsigned);
void patchlist_breaklist(unsigned);
void patchlist_contlist(unsigned); 
void push_state_stack(std::string);
void pop_state_stack();
std::string state_stack_top();
int state_stack_size();

void patchlist_returnlist(unsigned);
void push_func_stack(SymbolTableEntry*);
void pop_func_stack();
SymbolTableEntry* func_stack_top();
int func_stack_size();

void make_indexed_map_list();
std::map<expression*, expression*> get_indexed_map();

indexed_elem* make_indexed_elem(expression*, expression*);
void backpatch(unsigned, unsigned);

#endif