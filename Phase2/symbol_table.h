#ifndef _SYMBOL_TABLE_H
#define _SYMBOL_TABLE_H

#include <iostream>
#include <string>
#include <stack>
#include <vector>
#include <map>

struct Key;
struct SymbolTableEntry;

enum SymbolType {
    GLOBAL, LOCAL, FORMAL, USERFUNC, LIBFUNC
};

Key make_key(const std::string &symbol_name, const int& symbol_scope);
SymbolTableEntry make_entry(const std::string&, const int&, const int&, SymbolType);
//SymbolTableEntry make_func_entry(const std::string&, SymbolType);

void insert(std::string, int, int, SymbolType);
bool lookup_at_scope(std::string, int);
bool lookup(std::string);
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
std::map<std::string, SymbolTableEntry> insert_lib_function(std::map<std::string, SymbolTableEntry>);

#endif