#ifndef _HANDLERS_H
#define _HANDLERS_H

#include <iostream>
#include <string>
#include <list>
#include "symbol_table.h"

void push_blocks_prec(int);
void pop_blocks_prec();
void handle_return_st();
void handle_break_st();
void handle_continue_st();
int handle_expr(std::string, int);
int has_access(std::string);
void handle_id(std::string, int, int, scope_space, unsigned);
void handle_local_id(std::string, int, int, scope_space, unsigned);
void handle_global_access_id(std::string);
void handle_funcdef_w_name(std::string, int, int, scope_space, unsigned);
void handle_funcdef_anonym_name(int, int, scope_space, unsigned);
void handle_func_w_1arg(std::string, int, int, scope_space, unsigned);
void handle_expression(std::string, std::string, int, int);
void handle_funcdef_block_end(int);
void handle_block_end(int);
int is_a_lib_func(std::string);
std::string func_name_generator();

#endif
