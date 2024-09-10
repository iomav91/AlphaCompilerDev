#ifndef _HANDLERS_H
#define _HANDLERS_H

#include <iostream>
#include <string>
#include <list>
#include "symbol_table.h"

void push_blocks_prec(int);
bool handle_assign_expr(std::string, int);
void handle_id(std::string, int, scope_space, unsigned, int);
void handle_local_id(std::string, int, scope_space, unsigned, int);
void handle_global_access_id(std::string);
void handle_funcdef_w_name(std::string, int, scope_space, unsigned, int);
void handle_funcdef_anonym_name(std::string, int, scope_space, unsigned, int);
void handle_func_w_1arg(std::string, int, scope_space, unsigned, int);
void handle_expression(std::string, std::string, int, int);
void handle_funcdef_block_end(int);
void handle_block_end(int);
int is_a_lib_func(std::string);
std::string func_name_generator();

#endif