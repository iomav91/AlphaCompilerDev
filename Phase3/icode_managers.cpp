#include "icode_managers.h"
#include "handlers.h"
#include <iostream>
#include <fstream>
#include <string>

unsigned line_op_counter = 0;
unsigned prev_scope_space_counter = 1;
unsigned program_var_offset = 0;
unsigned function_local_offset = 0;
unsigned formal_arg_offset = 0;
unsigned scope_space_counter = 1;
unsigned curr_quad_label = 0;

std::vector<quad> quad_table;

quad make_quad(iopcode op, expression* result, expression* arg1, 
               expression* arg2, unsigned label, unsigned line) {
    quad new_quad;

    new_quad.op = op;
    new_quad.result = result;
    new_quad.arg1 = arg1;
    new_quad.arg2 = arg2;
    new_quad.label = label;
    new_quad.line = line;

    return new_quad;
}

std::vector<expression*> expression_list;
std::vector<expression*> reversed_expression_list;
std::map<expression*, expression*> indexed_map;
std::vector<std::map<expression*, expression*>> indexed_map_list;
std::vector<std::string> state_stack;

void set_elist_expression(expression* table) {
    int i = 0;
    for (auto& expr : expression_list) {
        //std::cout << "Expression in Brackets is of type " << expr->type << std::endl;
        emit_table_set_item(TABLESETELEM, table, make_constnum_expression(CONSTNUM_EXPR, i), expr);
        i = i + 1;
    }
    return;
}

void clear_expression_list() {
    reversed_expression_list.clear();
    expression_list.clear();
}

void set_indexed_map(expression* table) {
    int i = 0;
    for (const auto& index : indexed_map) {
        //std::cout << "Index is " << index.first->str_const << std::endl;
        emit_table_set_item(TABLESETELEM, table, index.first, index.second);
    }
    return;
}

void insert_indexed_map(expression* index,expression* value) {
    //std::cout << "Index is " << index->str_const << std::endl;
    indexed_map[index] = value;
    return;
}

void push_expression_list(expression* elist) {
    expression_list.push_back(elist);
    return;
}

void pop_expression_list() {
    expression_list.pop_back();
    return;
}

expression* get_last_expression() {
    //std::cout << "Expression List Size is " << expression_list.size() << std::endl; 
    if (expression_list.size() == 0) {
        return NULL;
    }

    for (int i = expression_list.size()-1; i >= 0; i--) {
        //std::cout << expression_list.at(i)->symbol->name << std::endl;
        if (expression_list.at(i) != NULL) {
            reversed_expression_list.push_back(expression_list.at(i));
            expression_list.pop_back();
        }
    }
    //expression_list.clear();
    return reversed_expression_list.back();
}

void print_expression_list() {
    if (expression_list.size() == 0) {
        return;
    }
    
    for (const auto& expr : expression_list) {
        std::cout << expr->symbol->name << std::endl; 
    } 
    
    return;
}

void print_indexed_map() {
    for (const auto& index : indexed_map) {
        std::cout << index.first << std::endl; 
    } 
    
    return;
}

void print_reversed_expression_list() {
    if (reversed_expression_list.size() == 0) {
        return;
    }
    
    for (const auto& expr : reversed_expression_list) {
        std::cout << expr->symbol->name << std::endl; 
    } 
    
    return;
}

void quad_creation(int op, expression* result, expression* arg1, expression* arg2, unsigned label, unsigned line) {
    //std::cout << "HERE After PUSHBACK" << std::endl;
    std::string filename = "outfile1.asc";
    std::ofstream outfile;

    // Open the file in append mode
    outfile.open(filename, std::ios_base::app);

    // Check if the file is open
    if (outfile.is_open()) {
        // Write to the file
        if (op == 0) {
            if (arg1->type == CONSTNUM_EXPR) {
                outfile << "\n" << line << "\t\t" << "ASSIGN" << "\t\t" << result->symbol->name << "\t\t" << arg1->num_const;
            } else if (arg1->type == CONSTSTRING_EXPR) {
                outfile << "\n" << line << "\t\t" << "ASSIGN" << "\t\t" << result->symbol->name << "\t\t" << arg1->str_const;
            } else if (arg1->type == CONSTBOOL_EXPR) {
                outfile << "\n" << line << "\t\t" << "ASSIGN" << "\t\t" << result->symbol->name << "\t\t" << arg1->bool_const;
            } else {
                outfile << "\n" << line << "\t\t" << "ASSIGN" << "\t\t" << result->symbol->name << "\t\t" << arg1->symbol->name;
            }
        }
        if (op == 1) {
            if (arg1->type == CONSTNUM_EXPR) {
                if (arg2->type == CONSTNUM_EXPR) {
                    outfile << "\n" << line << "\t\t" << "ADD" << "\t\t" << result->num_const << "\t\t" << arg1->num_const << "\t\t" << arg2->num_const;
                } else if (arg2->type == CONSTSTRING_EXPR) {
                    outfile << "\n" << line << "\t\t" << "ADD" << "\t\t" << result->symbol->name << "\t\t" << arg1->num_const << "\t\t" << arg2->str_const;
                } else if (arg2->type == CONSTBOOL_EXPR) {
                    outfile << "\n" << line << "\t\t" << "ADD" << "\t\t" << result->symbol->name << "\t\t" << arg1->num_const << "\t\t" << arg2->bool_const;
                } else {
                    outfile << "\n" << line << "\t\t" << "ADD" << "\t\t" << result->symbol->name << "\t\t" << arg1->num_const << "\t\t" << arg2->symbol->name;
                }
            } else if (arg2-> type == CONSTNUM_EXPR) {
                if (arg1->type == CONSTNUM_EXPR) {
                    outfile << "\n" << line << "\t\t" << "ADD" << "\t\t" << result->num_const << "\t\t" << arg1->num_const << "\t\t" << arg2->num_const;
                } else if (arg1->type == CONSTSTRING_EXPR) {
                    outfile << "\n" << line << "\t\t" << "ADD" << "\t\t" << result->symbol->name << "\t\t" << arg1->str_const << "\t\t" << arg2->num_const;
                } else if (arg1->type == CONSTBOOL_EXPR) {
                    outfile << "\n" << line << "\t\t" << "ADD" << "\t\t" << result->symbol->name << "\t\t" << arg1->bool_const << "\t\t" << arg2->num_const;
                } else {
                    outfile << "\n" << line << "\t\t" << "ADD" << "\t\t" << result->symbol->name << "\t\t" << arg1->symbol->name << "\t\t" << arg2->num_const;
                }
            } else if (arg1->type == CONSTBOOL_EXPR) {
                if (arg2->type == CONSTNUM_EXPR) {
                    outfile << "\n" << line << "\t\t" << "ADD" << "\t\t" << result->symbol->name << "\t\t" << arg1->bool_const << "\t\t" << arg2->num_const;
                } else if (arg2->type == CONSTSTRING_EXPR) {
                    outfile << "\n" << line << "\t\t" << "ADD" << "\t\t" << result->symbol->name << "\t\t" << arg1->bool_const << "\t\t" << arg2->str_const;
                } else if (arg2->type == CONSTBOOL_EXPR) {
                    outfile << "\n" << line << "\t\t" << "ADD" << "\t\t" << result->symbol->name << "\t\t" << arg1->bool_const << "\t\t" << arg2->bool_const;
                } else {
                    outfile << "\n" << line << "\t\t" << "ADD" << "\t\t" << result->symbol->name << "\t\t" << arg1->bool_const << "\t\t" << arg2->symbol->name;
                }
            } else if (arg2->type == CONSTBOOL_EXPR) {
                if (arg1->type == CONSTNUM_EXPR) {
                    outfile << "\n" << line << "\t\t" << "ADD" << "\t\t" << result->symbol->name << "\t\t" << arg1->num_const << "\t\t" << arg2->bool_const;
                } else if (arg1->type == CONSTSTRING_EXPR) {
                    outfile << "\n" << line << "\t\t" << "ADD" << "\t\t" << result->symbol->name << "\t\t" << arg1->str_const << "\t\t" << arg2->bool_const;
                } else if (arg1->type == CONSTBOOL_EXPR) {
                    outfile << "\n" << line << "\t\t" << "ADD" << "\t\t" << result->symbol->name << "\t\t" << arg1->bool_const << "\t\t" << arg2->bool_const;
                } else {
                    outfile << "\n" << line << "\t\t" << "ADD" << "\t\t" << result->symbol->name << "\t\t" << arg1->symbol->name << "\t\t" << arg2->bool_const;
                }
            } else {
                outfile << "\n" << line << "\t\t" << "ADD" << "\t\t" << result->symbol->name << "\t\t" << arg1->symbol->name << "\t\t" << arg2->symbol->name;
            }
        }
        if (op == 2) {
            if (arg1->type == CONSTNUM_EXPR) {
                if (arg2->type == CONSTNUM_EXPR) {
                    outfile << "\n" << line << "\t\t" << "SUB" << "\t\t" << result->num_const << "\t\t" << arg1->num_const << "\t\t" << arg2->num_const;
                } else if (arg2->type == CONSTSTRING_EXPR) {
                    outfile << "\n" << line << "\t\t" << "SUB" << "\t\t" << result->symbol->name << "\t\t" << arg1->num_const << "\t\t" << arg2->str_const;
                } else if (arg2->type == CONSTBOOL_EXPR) {
                    outfile << "\n" << line << "\t\t" << "SUB" << "\t\t" << result->symbol->name << "\t\t" << arg1->num_const << "\t\t" << arg2->bool_const;
                } else {
                    outfile << "\n" << line << "\t\t" << "SUB" << "\t\t" << result->symbol->name << "\t\t" << arg1->num_const << "\t\t" << arg2->symbol->name;
                }
            } else if (arg2-> type == CONSTNUM_EXPR) {
                if (arg1->type == CONSTNUM_EXPR) {
                    outfile << "\n" << line << "\t\t" << "SUB" << "\t\t" << result->symbol->name << "\t\t" << arg1->num_const << "\t\t" << arg2->num_const;
                } else if (arg1->type == CONSTSTRING_EXPR) {
                    outfile << "\n" << line << "\t\t" << "SUB" << "\t\t" << result->symbol->name << "\t\t" << arg1->str_const << "\t\t" << arg2->num_const;
                } else if (arg1->type == CONSTBOOL_EXPR) {
                    outfile << "\n" << line << "\t\t" << "SUB" << "\t\t" << result->symbol->name << "\t\t" << arg1->bool_const << "\t\t" << arg2->num_const;
                } else {
                    outfile << "\n" << line << "\t\t" << "SUB" << "\t\t" << result->symbol->name << "\t\t" << arg1->symbol->name << "\t\t" << arg2->num_const;
                }
            } else if (arg1->type == CONSTBOOL_EXPR) {
                if (arg2->type == CONSTNUM_EXPR) {
                    outfile << "\n" << line << "\t\t" << "SUB" << "\t\t" << result->symbol->name << "\t\t" << arg1->bool_const << "\t\t" << arg2->num_const;
                } else if (arg2->type == CONSTSTRING_EXPR) {
                    outfile << "\n" << line << "\t\t" << "SUB" << "\t\t" << result->symbol->name << "\t\t" << arg1->bool_const << "\t\t" << arg2->str_const;
                } else if (arg2->type == CONSTBOOL_EXPR) {
                    outfile << "\n" << line << "\t\t" << "SUB" << "\t\t" << result->symbol->name << "\t\t" << arg1->bool_const << "\t\t" << arg2->bool_const;
                } else {
                    outfile << "\n" << line << "\t\t" << "SUB" << "\t\t" << result->symbol->name << "\t\t" << arg1->bool_const << "\t\t" << arg2->symbol->name;
                }
            } else if (arg2->type == CONSTBOOL_EXPR) {
                if (arg1->type == CONSTNUM_EXPR) {
                    outfile << "\n" << line << "\t\t" << "SUB" << "\t\t" << result->symbol->name << "\t\t" << arg1->num_const << "\t\t" << arg2->bool_const;
                } else if (arg1->type == CONSTSTRING_EXPR) {
                    outfile << "\n" << line << "\t\t" << "SUB" << "\t\t" << result->symbol->name << "\t\t" << arg1->str_const << "\t\t" << arg2->bool_const;
                } else if (arg1->type == CONSTBOOL_EXPR) {
                    outfile << "\n" << line << "\t\t" << "SUB" << "\t\t" << result->symbol->name << "\t\t" << arg1->bool_const << "\t\t" << arg2->bool_const;
                } else {
                    outfile << "\n" << line << "\t\t" << "SUB" << "\t\t" << result->symbol->name << "\t\t" << arg1->symbol->name << "\t\t" << arg2->bool_const;
                }
            }  else {
                outfile << "\n" << line << "\t\t" << "SUB" << "\t\t" << result->symbol->name << "\t\t" << arg1->symbol->name << "\t\t" << arg2->symbol->name;
            }
        }
        if (op == 3) {
            if (arg1->type == CONSTNUM_EXPR) {
                if (arg2->type == CONSTNUM_EXPR) {
                    outfile << "\n" << line << "\t\t" << "MUL" << "\t\t" << result->num_const << "\t\t" << arg1->num_const << "\t\t" << arg2->num_const;
                } else if (arg2->type == CONSTSTRING_EXPR) {
                    outfile << "\n" << line << "\t\t" << "MUL" << "\t\t" << result->symbol->name << "\t\t" << arg1->num_const << "\t\t" << arg2->str_const;
                } else if (arg2->type == CONSTBOOL_EXPR) {
                    outfile << "\n" << line << "\t\t" << "MUL" << "\t\t" << result->symbol->name << "\t\t" << arg1->num_const << "\t\t" << arg2->bool_const;
                } else {
                    outfile << "\n" << line << "\t\t" << "MUL" << "\t\t" << result->symbol->name << "\t\t" << arg1->num_const << "\t\t" << arg2->symbol->name;
                }
            } else if (arg2-> type == CONSTNUM_EXPR) {
                if (arg1->type == CONSTNUM_EXPR) {
                    outfile << "\n" << line << "\t\t" << "MUL" << "\t\t" << result->symbol->name << "\t\t" << arg1->num_const << "\t\t" << arg2->num_const;
                } else if (arg1->type == CONSTSTRING_EXPR) {
                    outfile << "\n" << line << "\t\t" << "MUL" << "\t\t" << result->symbol->name << "\t\t" << arg1->str_const << "\t\t" << arg2->num_const;
                } else if (arg1->type == CONSTBOOL_EXPR) {
                    outfile << "\n" << line << "\t\t" << "MUL" << "\t\t" << result->symbol->name << "\t\t" << arg1->bool_const << "\t\t" << arg2->num_const;
                } else {
                    outfile << "\n" << line << "\t\t" << "MUL" << "\t\t" << result->symbol->name << "\t\t" << arg1->symbol->name << "\t\t" << arg2->num_const;
                }
            } else if (arg1->type == CONSTBOOL_EXPR) {
                if (arg2->type == CONSTNUM_EXPR) {
                    outfile << "\n" << line << "\t\t" << "MUL" << "\t\t" << result->symbol->name << "\t\t" << arg1->bool_const << "\t\t" << arg2->num_const;
                } else if (arg2->type == CONSTSTRING_EXPR) {
                    outfile << "\n" << line << "\t\t" << "MUL" << "\t\t" << result->symbol->name << "\t\t" << arg1->bool_const << "\t\t" << arg2->str_const;
                } else if (arg2->type == CONSTBOOL_EXPR) {
                    outfile << "\n" << line << "\t\t" << "MUL" << "\t\t" << result->symbol->name << "\t\t" << arg1->bool_const << "\t\t" << arg2->bool_const;
                } else {
                    outfile << "\n" << line << "\t\t" << "MUL" << "\t\t" << result->symbol->name << "\t\t" << arg1->bool_const << "\t\t" << arg2->symbol->name;
                }
            } else if (arg2->type == CONSTBOOL_EXPR) {
                if (arg1->type == CONSTNUM_EXPR) {
                    outfile << "\n" << line << "\t\t" << "MUL" << "\t\t" << result->symbol->name << "\t\t" << arg1->num_const << "\t\t" << arg2->bool_const;
                } else if (arg1->type == CONSTSTRING_EXPR) {
                    outfile << "\n" << line << "\t\t" << "MUL" << "\t\t" << result->symbol->name << "\t\t" << arg1->str_const << "\t\t" << arg2->bool_const;
                } else if (arg1->type == CONSTBOOL_EXPR) {
                    outfile << "\n" << line << "\t\t" << "MUL" << "\t\t" << result->symbol->name << "\t\t" << arg1->bool_const << "\t\t" << arg2->bool_const;
                } else {
                    outfile << "\n" << line << "\t\t" << "MUL" << "\t\t" << result->symbol->name << "\t\t" << arg1->symbol->name << "\t\t" << arg2->bool_const;
                }
            } else {
                outfile << "\n" << line << "\t\t" << "MUL" << "\t\t" << result->symbol->name << "\t\t" << arg1->symbol->name << "\t\t" << arg2->symbol->name;
            }
        }
        if (op == 4) {
            if (arg1->type == CONSTNUM_EXPR) {
                if (arg2->type == CONSTNUM_EXPR) {
                    outfile << "\n" << line << "\t\t" << "DIV" << "\t\t" << result->num_const << "\t\t" << arg1->num_const << "\t\t" << arg2->num_const;
                } else if (arg2->type == CONSTSTRING_EXPR) {
                    outfile << "\n" << line << "\t\t" << "DIV" << "\t\t" << result->symbol->name << "\t\t" << arg1->num_const << "\t\t" << arg2->str_const;
                } else if (arg2->type == CONSTBOOL_EXPR) {
                    outfile << "\n" << line << "\t\t" << "DIV" << "\t\t" << result->symbol->name << "\t\t" << arg1->num_const << "\t\t" << arg2->bool_const;
                } else {
                    outfile << "\n" << line << "\t\t" << "DIV" << "\t\t" << result->symbol->name << "\t\t" << arg1->num_const << "\t\t" << arg2->symbol->name;
                }
            } else if (arg2-> type == CONSTNUM_EXPR) {
                if (arg1->type == CONSTNUM_EXPR) {
                    outfile << "\n" << line << "\t\t" << "DIV" << "\t\t" << result->symbol->name << "\t\t" << arg1->num_const << "\t\t" << arg2->num_const;
                } else if (arg1->type == CONSTSTRING_EXPR) {
                    outfile << "\n" << line << "\t\t" << "DIV" << "\t\t" << result->symbol->name << "\t\t" << arg1->str_const << "\t\t" << arg2->num_const;
                } else if (arg1->type == CONSTBOOL_EXPR) {
                    outfile << "\n" << line << "\t\t" << "DIV" << "\t\t" << result->symbol->name << "\t\t" << arg1->bool_const << "\t\t" << arg2->num_const;
                } else {
                    outfile << "\n" << line << "\t\t" << "DIV" << "\t\t" << result->symbol->name << "\t\t" << arg1->symbol->name << "\t\t" << arg2->num_const;
                }
            } else if (arg1->type == CONSTBOOL_EXPR) {
                if (arg2->type == CONSTNUM_EXPR) {
                    outfile << "\n" << line << "\t\t" << "DIV" << "\t\t" << result->symbol->name << "\t\t" << arg1->bool_const << "\t\t" << arg2->num_const;
                } else if (arg2->type == CONSTSTRING_EXPR) {
                    outfile << "\n" << line << "\t\t" << "DIV" << "\t\t" << result->symbol->name << "\t\t" << arg1->bool_const << "\t\t" << arg2->str_const;
                } else if (arg2->type == CONSTBOOL_EXPR) {
                    outfile << "\n" << line << "\t\t" << "DIV" << "\t\t" << result->symbol->name << "\t\t" << arg1->bool_const << "\t\t" << arg2->bool_const;
                } else {
                    outfile << "\n" << line << "\t\t" << "DIV" << "\t\t" << result->symbol->name << "\t\t" << arg1->bool_const << "\t\t" << arg2->symbol->name;
                }
            } else if (arg2->type == CONSTBOOL_EXPR) {
                if (arg1->type == CONSTNUM_EXPR) {
                    outfile << "\n" << line << "\t\t" << "DIV" << "\t\t" << result->symbol->name << "\t\t" << arg1->num_const << "\t\t" << arg2->bool_const;
                } else if (arg1->type == CONSTSTRING_EXPR) {
                    outfile << "\n" << line << "\t\t" << "DIV" << "\t\t" << result->symbol->name << "\t\t" << arg1->str_const << "\t\t" << arg2->bool_const;
                } else if (arg1->type == CONSTBOOL_EXPR) {
                    outfile << "\n" << line << "\t\t" << "DIV" << "\t\t" << result->symbol->name << "\t\t" << arg1->bool_const << "\t\t" << arg2->bool_const;
                } else {
                    outfile << "\n" << line << "\t\t" << "DIV" << "\t\t" << result->symbol->name << "\t\t" << arg1->symbol->name << "\t\t" << arg2->bool_const;
                }
            } else {
                outfile << "\n" << line << "\t\t" << "DIV" << "\t\t" << result->symbol->name << "\t\t" << arg1->symbol->name << "\t\t" << arg2->symbol->name;
            }
        }
        if (op == 5) {
            if (arg1->type == CONSTNUM_EXPR) {
                if (arg2->type == CONSTNUM_EXPR) {
                    outfile << "\n" << line << "\t\t" << "MOD" << "\t\t" << result->num_const << "\t\t" << arg1->num_const << "\t\t" << arg2->num_const;
                } else if (arg2->type == CONSTSTRING_EXPR) {
                    outfile << "\n" << line << "\t\t" << "MOD" << "\t\t" << result->symbol->name << "\t\t" << arg1->num_const << "\t\t" << arg2->str_const;
                } else if (arg2->type == CONSTBOOL_EXPR) {
                    outfile << "\n" << line << "\t\t" << "MOD" << "\t\t" << result->symbol->name << "\t\t" << arg1->num_const << "\t\t" << arg2->bool_const;
                } else {
                    outfile << "\n" << line << "\t\t" << "MOD" << "\t\t" << result->symbol->name << "\t\t" << arg1->num_const << "\t\t" << arg2->symbol->name;
                }
            } else if (arg2-> type == CONSTNUM_EXPR) {
                if (arg1->type == CONSTNUM_EXPR) {
                    outfile << "\n" << line << "\t\t" << "MOD" << "\t\t" << result->symbol->name << "\t\t" << arg1->num_const << "\t\t" << arg2->num_const;
                } else if (arg1->type == CONSTSTRING_EXPR) {
                    outfile << "\n" << line << "\t\t" << "MOD" << "\t\t" << result->symbol->name << "\t\t" << arg1->str_const << "\t\t" << arg2->num_const;
                } else if (arg1->type == CONSTBOOL_EXPR) {
                    outfile << "\n" << line << "\t\t" << "MOD" << "\t\t" << result->symbol->name << "\t\t" << arg1->bool_const << "\t\t" << arg2->num_const;
                } else {
                    outfile << "\n" << line << "\t\t" << "MOD" << "\t\t" << result->symbol->name << "\t\t" << arg1->symbol->name << "\t\t" << arg2->num_const;
                }
            } else if (arg1->type == CONSTBOOL_EXPR) {
                if (arg2->type == CONSTNUM_EXPR) {
                    outfile << "\n" << line << "\t\t" << "MOD" << "\t\t" << result->symbol->name << "\t\t" << arg1->bool_const << "\t\t" << arg2->num_const;
                } else if (arg2->type == CONSTSTRING_EXPR) {
                    outfile << "\n" << line << "\t\t" << "MOD" << "\t\t" << result->symbol->name << "\t\t" << arg1->bool_const << "\t\t" << arg2->str_const;
                } else if (arg2->type == CONSTBOOL_EXPR) {
                    outfile << "\n" << line << "\t\t" << "MOD" << "\t\t" << result->symbol->name << "\t\t" << arg1->bool_const << "\t\t" << arg2->bool_const;
                } else {
                    outfile << "\n" << line << "\t\t" << "MOD" << "\t\t" << result->symbol->name << "\t\t" << arg1->bool_const << "\t\t" << arg2->symbol->name;
                }
            } else if (arg2->type == CONSTBOOL_EXPR) {
                if (arg1->type == CONSTNUM_EXPR) {
                    outfile << "\n" << line << "\t\t" << "MOD" << "\t\t" << result->symbol->name << "\t\t" << arg1->num_const << "\t\t" << arg2->bool_const;
                } else if (arg1->type == CONSTSTRING_EXPR) {
                    outfile << "\n" << line << "\t\t" << "MOD" << "\t\t" << result->symbol->name << "\t\t" << arg1->str_const << "\t\t" << arg2->bool_const;
                } else if (arg1->type == CONSTBOOL_EXPR) {
                    outfile << "\n" << line << "\t\t" << "MOD" << "\t\t" << result->symbol->name << "\t\t" << arg1->bool_const << "\t\t" << arg2->bool_const;
                } else {
                    outfile << "\n" << line << "\t\t" << "MOD" << "\t\t" << result->symbol->name << "\t\t" << arg1->symbol->name << "\t\t" << arg2->bool_const;
                }
            } else {
                outfile << "\n" << line << "\t\t" << "MOD" << "\t\t" << result->symbol->name << "\t\t" << arg1->symbol->name << "\t\t" << arg2->symbol->name;
            }
        }
        if (op == 6) {
            if (arg1->type == CONSTNUM_EXPR) {
                outfile << "\n" << line << "\t\t" << "UMINUS" << "\t\t" << result->symbol->name << "\t\t" << arg1->num_const;
            } else if (arg1->type == CONSTBOOL_EXPR) {
                outfile << "\n" << line << "\t\t" << "UMINUS" << "\t\t" << result->symbol->name << "\t\t" << arg1->bool_const;
            } else {
                outfile << "\n" << line << "\t\t" << "UMINUS" << "\t\t" << result->symbol->name << "\t\t" << arg1->symbol->name;
            }
        }
        if (op == 7) {
            outfile << "\n" << line << "\t\t" << "AND" << "\t\t" << result->symbol->name << "\t\t" << arg1->symbol->name << "\t\t" << arg2->symbol->name;
        }
        if (op == 8) {
            outfile << "\n" << line << "\t\t" << "OR" << "\t\t" << result->symbol->name << "\t\t" << arg1->symbol->name << "\t\t" << arg2->symbol->name;
        }
        if (op == 9) {

        }
        if (op == 10) {
            if (result->type == CONSTNUM_EXPR) {
                if (arg1->type == CONSTNUM_EXPR) { 
                    outfile << "\n" << line << "\t" << "IF EQUAL" << "\t\t" << result->num_const << "\t\t" << arg1->num_const << "\t\t" << label; 
                } else if (arg1->type == CONSTSTRING_EXPR) {
                    outfile << "\n" << line << "Error: the symbol" << "\t\t" << arg1->str_const << " is a string";
                } else {
                    outfile << "\n" << line << "\t" << "IF EQUAL" << "\t\t" << result->num_const << "\t\t" << arg1->symbol->name << "\t\t" << label;
                }
            } else if (result->type == CONSTSTRING_EXPR) {
                outfile << "\n" << line << "Error: the symbol" << "\t\t" << result->str_const << " is a string";
            } else {
                if (arg1->type == CONSTNUM_EXPR) { 
                    outfile << "\n" << line << "\t" << "IF EQUAL" << "\t\t" << result->symbol->name << "\t\t" << arg1->num_const << "\t\t" << label; 
                } else if (arg1->type == CONSTSTRING_EXPR) {
                    outfile << "\n" << line << "Error: the symbol" << "\t\t" << arg1->str_const << " is a string";
                } else if (arg1->type == CONSTBOOL_EXPR) {
                    outfile << "\n" << line << "\t" << "IF EQUAL" << "\t\t" << result->symbol->name << "\t\t" << arg1->bool_const << "\t\t" << label;
                }
            }
        }
        if (op == 11) {
            if (result->type == CONSTNUM_EXPR) {
                if (arg1->type == CONSTNUM_EXPR) { 
                    outfile << "\n" << line << "\t" << "IF NOT EQUAL" << "\t\t" << result->num_const << "\t\t" << arg1->num_const << "\t\t" << label; 
                } else if (arg1->type == CONSTSTRING_EXPR) {
                    outfile << "\n" << line << "Error: the symbol" << "\t\t" << arg1->str_const << " is a string";
                } else {
                    outfile << "\n" << line << "\t" << "IF NOT EQUAL" << "\t\t" << result->num_const << "\t\t" << arg1->symbol->name << "\t\t" << label;
                }
            } else if (result->type == CONSTSTRING_EXPR) {
                outfile << "\n" << line << "Error: the symbol" << "\t\t" << result->str_const << " is a string";
            } else {
                if (arg1->type == CONSTNUM_EXPR) { 
                    outfile << "\n" << line << "\t" << "IF NOT EQUAL" << "\t\t" << result->symbol->name << "\t\t" << arg1->num_const << "\t\t" << label; 
                } else if (arg1->type == CONSTSTRING_EXPR) {
                    outfile << "\n" << line << "Error: the symbol" << "\t\t" << arg1->str_const << " is a string";
                } else {
                    outfile << "\n" << line << "\t" << "IF NOT EQUAL" << "\t\t" << result->symbol->name << "\t\t" << arg1->symbol->name << "\t\t" << label;
                }
            }
        }
        if (op == 12) {
            if (result->type == CONSTNUM_EXPR) {
                if (arg1->type == CONSTNUM_EXPR) { 
                    outfile << "\n" << line << "\t" << "IF LESS EQUAL" << "\t\t" << result->num_const << "\t\t" << arg1->num_const << "\t\t" << label; 
                } else if (arg1->type == CONSTSTRING_EXPR) {
                    outfile << "\n" << line << "Error: the symbol" << "\t\t" << arg1->str_const << " is a string";
                } else {
                    outfile << "\n" << line << "\t" << "IF LESS EQUAL" << "\t\t" << result->num_const << "\t\t" << arg1->symbol->name << "\t\t" << label;
                }
            } else if (result->type == CONSTSTRING_EXPR) {
                outfile << "\n" << line << "Error: the symbol" << "\t\t" << result->str_const << " is a string";
            } else {
                if (arg1->type == CONSTNUM_EXPR) { 
                    outfile << "\n" << line << "\t" << "IF LESS EQUAL" << "\t\t" << result->symbol->name << "\t\t" << arg1->num_const << "\t\t" << label; 
                } else if (arg1->type == CONSTSTRING_EXPR) {
                    outfile << "\n" << line << "Error: the symbol" << "\t\t" << arg1->str_const << " is a string";
                } else {
                    outfile << "\n" << line << "\t" << "IF LESS EQUAL" << "\t\t" << result->symbol->name << "\t\t" << arg1->symbol->name << "\t\t" << label;
                }
            }
        }
        if (op == 13) {
            if (result->type == CONSTNUM_EXPR) {
                if (arg1->type == CONSTNUM_EXPR) { 
                    outfile << "\n" << line << "\t" << "IF GREATER EQUAL" << "\t\t" << result->num_const << "\t\t" << arg1->num_const << "\t\t" << label; 
                } else if (arg1->type == CONSTSTRING_EXPR) {
                    outfile << "\n" << line << "Error: the symbol" << "\t\t" << arg1->str_const << " is a string";
                } else {
                    outfile << "\n" << line << "\t" << "IF GREATER EQUAL" << "\t\t" << result->num_const << "\t\t" << arg1->symbol->name << "\t\t" << label;
                }
            } else if (result->type == CONSTSTRING_EXPR) {
                outfile << "\n" << line << "Error: the symbol" << "\t\t" << result->str_const << " is a string";
            } else {
                if (arg1->type == CONSTNUM_EXPR) { 
                    outfile << "\n" << line << "\t" << "IF GREATER EQUAL" << "\t\t" << result->symbol->name << "\t\t" << arg1->num_const << "\t\t" << label; 
                } else if (arg1->type == CONSTSTRING_EXPR) {
                    outfile << "\n" << line << "Error: the symbol" << "\t\t" << arg1->str_const << " is a string";
                } else {
                    outfile << "\n" << line << "\t" << "IF GREATER EQUAL" << "\t\t" << result->symbol->name << "\t\t" << arg1->symbol->name << "\t\t" << label;
                }
            }
        }
        if (op == 14) {
            if (result->type == CONSTNUM_EXPR) {
                if (arg1->type == CONSTNUM_EXPR) { 
                    outfile << "\n" << line << "\t" << "IF LESS" << "\t\t" << result->num_const << "\t\t" << arg1->num_const << "\t\t" << label; 
                } else if (arg1->type == CONSTSTRING_EXPR) {
                    outfile << "\n" << line << "Error: the symbol" << "\t\t" << arg1->str_const << " is a string";
                } else {
                    outfile << "\n" << line << "\t" << "IF LESS" << "\t\t" << result->num_const << "\t\t" << arg1->symbol->name << "\t\t" << label;
                }
            } else if (result->type == CONSTSTRING_EXPR) {
                outfile << "\n" << line << "Error: the symbol" << "\t\t" << result->str_const << " is a string";
            } else {
                if (arg1->type == CONSTNUM_EXPR) { 
                    outfile << "\n" << line << "\t" << "IF LESS" << "\t\t" << result->symbol->name << "\t\t" << arg1->num_const << "\t\t" << label; 
                } else if (arg1->type == CONSTSTRING_EXPR) {
                    outfile << "\n" << line << "Error: the symbol" << "\t\t" << arg1->str_const << " is a string";
                } else {
                    outfile << "\n" << line << "\t" << "IF LESS" << "\t\t" << result->symbol->name << "\t\t" << arg1->symbol->name << "\t\t" << label;
                }
            }
        }
        if (op == 15) {
            if (result->type == CONSTNUM_EXPR) {
                if (arg1->type == CONSTNUM_EXPR) { 
                    outfile << "\n" << line << "\t" << "IF GREATER" << "\t\t" << result->num_const << "\t\t" << arg1->num_const << "\t\t" << label; 
                } else if (arg1->type == CONSTSTRING_EXPR) {
                    outfile << "\n" << line << "Error: the symbol" << "\t\t" << arg1->str_const << " is a string";
                } else {
                    outfile << "\n" << line << "\t" << "IF GREATER" << "\t\t" << result->num_const << "\t\t" << arg1->symbol->name << "\t\t" << label;
                }
            } else if (result->type == CONSTSTRING_EXPR) {
                outfile << "\n" << line << "Error: the symbol" << "\t\t" << result->str_const << " is a string";
            } else {
                if (arg1->type == CONSTNUM_EXPR) { 
                    outfile << "\n" << line << "\t" << "IF GREATER" << "\t\t" << result->symbol->name << "\t\t" << arg1->num_const << "\t\t" << label; 
                } else if (arg1->type == CONSTSTRING_EXPR) {
                    outfile << "\n" << line << "Error: the symbol" << "\t\t" << arg1->str_const << " is a string";
                } else {
                    outfile << "\n" << line << "\t" << "IF GREATER" << "\t\t" << result->symbol->name << "\t\t" << arg1->symbol->name << "\t\t" << label;
                }
            }
        }
        if (op == 16) {
            outfile << "\n" << line << "\t" << "CALL" << "\t\t" << result->symbol->name;
        }
        if (op == 17) {
            if (result->type == CONSTNUM_EXPR) {
                outfile << "\n" << line << "\t" << "PARAM" << "\t\t" << result->num_const;
            } else {
                outfile << "\n" << line << "\t" << "PARAM" << "\t\t" << result->symbol->name;
            }
        }
        if (op == 18) {
            outfile << "\n" << line << "\t" << "RETURN" << "\t\t" << result->symbol->name;
        }
        if (op == 19) {
            outfile << "\n" << line << "\t" << "GETRETVAL" << "\t\t" << result->symbol->name;
        }
        if (op == 20) {
            outfile << "\n" << line << "\t" << "FUNCSTART" << "\t" << result->symbol->name;
            //std::cout << "Content appended successfully.\n";
        }
        if (op == 21) {
            outfile << "\n" << line << "\t" << "FUNCEND" << "\t\t" << result->symbol->name;
            //std::cout << "Content appended successfully.\n";
        } 
        if (op == 22) {
            outfile << "\n" << line << "\t" << "TABLECREATE" << "\t\t" << result->symbol->name;
        }
        if (op == 23) {
            if (arg2->type == CONSTNUM_EXPR) {
                if (arg1->type == CONSTNUM_EXPR) {
                    outfile << "\n" << line << "\t" << "TABLEGETELEM" << "\t\t" << result->symbol->name << "\t\t" << arg1->num_const << "\t\t" << arg2->num_const;
                } else if (arg1->type == CONSTSTRING_EXPR) {
                    outfile << "\n" << line << "\t" << "TABLEGETELEM" << "\t\t" << result->symbol->name << "\t\t" << arg1->str_const << "\t\t" << arg2->num_const;
                } else if (arg1->type == CONSTBOOL_EXPR) {
                    outfile << "\n" << line << "\t" << "TABLEGETELEM" << "\t\t" << result->symbol->name << "\t\t" << arg1->bool_const << "\t\t" << arg2->num_const;
                } else {
                    outfile << "\n" << line << "\t" << "TABLEGETELEM" << "\t\t" << result->symbol->name << "\t\t" << arg1->symbol->name << "\t\t" << arg2->num_const;
                }
            } else if (arg2->type == CONSTSTRING_EXPR) {
                if (arg1->type == CONSTNUM_EXPR) {
                    outfile << "\n" << line << "\t" << "TABLEGETELEM" << "\t\t" << result->symbol->name << "\t\t" << arg1->num_const << "\t\t" << arg2->str_const;
                } else if (arg1->type == CONSTSTRING_EXPR) {
                    outfile << "\n" << line << "\t" << "TABLEGETELEM" << "\t\t" << result->symbol->name << "\t\t" << arg1->str_const << "\t\t" << arg2->str_const;
                } else if (arg1->type == CONSTBOOL_EXPR) {
                    outfile << "\n" << line << "\t" << "TABLEGETELEM" << "\t\t" << result->symbol->name << "\t\t" << arg1->bool_const << "\t\t" << arg2->str_const;
                } else {
                    outfile << "\n" << line << "\t" << "TABLEGETELEM" << "\t\t" << result->symbol->name << "\t\t" << arg1->symbol->name << "\t\t" << arg2->str_const;
                }
            } else if (arg2->type == CONSTBOOL_EXPR) {
                if (arg1->type == CONSTNUM_EXPR) {
                    outfile << "\n" << line << "\t" << "TABLEGETELEM" << "\t\t" << result->symbol->name << "\t\t" << arg1->num_const << "\t\t" << arg2->bool_const;
                } else if (arg1->type == CONSTSTRING_EXPR) {
                    outfile << "\n" << line << "\t" << "TABLEGETELEM" << "\t\t" << result->symbol->name << "\t\t" << arg1->str_const << "\t\t" << arg2->bool_const;
                } else if (arg1->type == CONSTBOOL_EXPR) {
                    outfile << "\n" << line << "\t" << "TABLEGETELEM" << "\t\t" << result->symbol->name << "\t\t" << arg1->bool_const << "\t\t" << arg2->bool_const;
                } else {
                    outfile << "\n" << line << "\t" << "TABLEGETELEM" << "\t\t" << result->symbol->name << "\t\t" << arg1->symbol->name << "\t\t" << arg2->bool_const;
                }
            } else {
                if (arg1->type == CONSTNUM_EXPR) {
                    outfile << "\n" << line << "\t" << "TABLEGETELEM" << "\t\t" << result->symbol->name << "\t\t" << arg1->num_const << "\t\t" << arg2->symbol->name;
                } else if (arg1->type == CONSTSTRING_EXPR) {
                    outfile << "\n" << line << "\t" << "TABLEGETELEM" << "\t\t" << result->symbol->name << "\t\t" << arg1->str_const << "\t\t" << arg2->symbol->name;
                } else if (arg1->type == CONSTBOOL_EXPR) {
                    outfile << "\n" << line << "\t" << "TABLEGETELEM" << "\t\t" << result->symbol->name << "\t\t" << arg1->bool_const << "\t\t" << arg2->symbol->name;
                } else {
                    outfile << "\n" << line << "\t" << "TABLEGETELEM" << "\t\t" << result->symbol->name << "\t\t" << arg1->symbol->name << "\t\t" << arg2->symbol->name;
                }
            } 
        }
        if (op == 24) {
            if (arg2->type == CONSTNUM_EXPR) {
                if (arg1->type == CONSTNUM_EXPR) {
                    outfile << "\n" << line << "\t" << "TABLESETELEM" << "\t\t" << result->symbol->name << "\t\t" << arg1->num_const << "\t\t" << arg2->num_const;
                } else if (arg1->type == CONSTSTRING_EXPR) {
                    outfile << "\n" << line << "\t" << "TABLESETELEM" << "\t\t" << result->symbol->name << "\t\t" << arg1->str_const << "\t\t" << arg2->num_const;
                } else if (arg1->type == CONSTBOOL_EXPR) {
                    outfile << "\n" << line << "\t" << "TABLESETELEM" << "\t\t" << result->symbol->name << "\t\t" << arg1->bool_const << "\t\t" << arg2->num_const;
                } else {
                    outfile << "\n" << line << "\t" << "TABLESETELEM" << "\t\t" << result->symbol->name << "\t\t" << arg1->symbol->name << "\t\t" << arg2->num_const;
                }
            } else if (arg2->type == CONSTSTRING_EXPR) {
                if (arg1->type == CONSTNUM_EXPR) {
                    outfile << "\n" << line << "\t" << "TABLESETELEM" << "\t\t" << result->symbol->name << "\t\t" << arg1->num_const << "\t\t" << arg2->str_const;
                } else if (arg1->type == CONSTSTRING_EXPR) {
                    outfile << "\n" << line << "\t" << "TABLESETELEM" << "\t\t" << result->symbol->name << "\t\t" << arg1->str_const << "\t\t" << arg2->str_const;
                } else if (arg1->type == CONSTBOOL_EXPR) {
                    outfile << "\n" << line << "\t" << "TABLESETELEM" << "\t\t" << result->symbol->name << "\t\t" << arg1->bool_const << "\t\t" << arg2->str_const;
                } else {
                    outfile << "\n" << line << "\t" << "TABLESETELEM" << "\t\t" << result->symbol->name << "\t\t" << arg1->symbol->name << "\t\t" << arg2->str_const;
                }
            } else if (arg2->type == CONSTBOOL_EXPR) {
                if (arg1->type == CONSTNUM_EXPR) {
                    outfile << "\n" << line << "\t" << "TABLESETELEM" << "\t\t" << result->symbol->name << "\t\t" << arg1->num_const << "\t\t" << arg2->bool_const;
                } else if (arg1->type == CONSTSTRING_EXPR) {
                    outfile << "\n" << line << "\t" << "TABLESETELEM" << "\t\t" << result->symbol->name << "\t\t" << arg1->str_const << "\t\t" << arg2->bool_const;
                } else if (arg1->type == CONSTBOOL_EXPR) {
                    outfile << "\n" << line << "\t" << "TABLESETELEM" << "\t\t" << result->symbol->name << "\t\t" << arg1->bool_const << "\t\t" << arg2->bool_const;
                } else {
                    outfile << "\n" << line << "\t" << "TABLESETELEM" << "\t\t" << result->symbol->name << "\t\t" << arg1->symbol->name << "\t\t" << arg2->bool_const;
                }
            } else {
                if (arg1->type == CONSTNUM_EXPR) {
                    outfile << "\n" << line << "\t" << "TABLESETELEM" << "\t\t" << result->symbol->name << "\t\t" << arg1->num_const << "\t\t" << arg2->symbol->name;
                } else if (arg1->type == CONSTSTRING_EXPR) {
                    outfile << "\n" << line << "\t" << "TABLESETELEM" << "\t\t" << result->symbol->name << "\t\t" << arg1->str_const << "\t\t" << arg2->symbol->name;
                } else if (arg1->type == CONSTBOOL_EXPR) {
                    outfile << "\n" << line << "\t" << "TABLESETELEM" << "\t\t" << result->symbol->name << "\t\t" << arg1->bool_const << "\t\t" << arg2->symbol->name;
                } else {
                    outfile << "\n" << line << "\t" << "TABLESETELEM" << "\t\t" << result->symbol->name << "\t\t" << arg1->symbol->name << "\t\t" << arg2->symbol->name;
                }
            } 
        }
        if (op == 25) {
            
            outfile << "\n" << line << "\t" << "JUMP" << "\t\t" << label;
                
        }
        //std::cout << "Content appended successfully.\n";
    } else {
        std::cerr << "Error opening file for appending.\n";
    }

    // Close the file
    outfile.close();
}

expression* make_call(expression* lvalue, expression* reversed_elist) {
    //std::cout << "MAKE CALL " << lvalue->symbol->name << std::endl;
    
    expression* func = emit_if_table_item(lvalue);
    //std::cout << "MAKE CALL EMIT IF TABLE ITEM" << func->symbol->name << std::endl;
    while (reversed_elist) {
        //std::cout << "MAKE CALL IN WHILE " << reversed_elist->symbol->name << std::endl;
        //std::cout << "MAKE CALL elist " << reversed_elist->symbol->name << std::endl;
        emit_param(PARAM, reversed_elist);
        reversed_elist = reversed_elist->next;
    }
    
    emit_call(CALL, func);
    SymbolTableEntry* new_temp = new_temp_var(lvalue->symbol->line, lvalue->symbol->type);
    //std::cout << new_temp.name << std::endl;
    expression* result = make_lvalue_expression(VAR_EXPR, get_symbol(new_temp->name, get_scope()));
    //std::cout << "Result is " << result->symbol->name << std::endl;
    emit_get_ret_val(GETRETVAL, result);
    return result;
}

expression* make_call_in(expression* lvalue, std::vector<expression*> callsuffix) {
    //std::cout << "MAKE CALL " << lvalue->symbol->name << std::endl;
     
    expression* func = emit_if_table_item(lvalue);
    //std::cout << "MAKE CALL EMIT IF TABLE ITEM" << func->symbol->name << std::endl;
    /*while (reversed_elist) {
        //std::cout << "MAKE CALL IN WHILE " << reversed_elist->symbol->name << std::endl;
        std::cout << "MAKE CALL elist " << reversed_elist->symbol->name << std::endl;
        emit_param(PARAM, reversed_elist);
        reversed_elist = reversed_elist->next;
    }*/

    for (const auto& elem : callsuffix) {
        emit_param(PARAM, elem);
    }
    
    emit_call(CALL, func);
    SymbolTableEntry* new_temp = new_temp_var(lvalue->symbol->line, lvalue->symbol->type);
    //std::cout << new_temp.name << std::endl;
    expression* result = make_lvalue_expression(VAR_EXPR, get_symbol(new_temp->name, get_scope()));
    //std::cout << "Result is " << result->symbol->name << std::endl;
    emit_get_ret_val(GETRETVAL, result);
    return result;
}

void emit_arithm_one(iopcode op, expression* result, expression* arg1) {
    //std::cout << "term -> - expression emit_arithm_one " << result->type << std::endl;
    quad_table.push_back(make_quad(op, result, arg1, NULL, UINT_MAX, line_op_counter));
    //std::cout << "term -> - expression emit_arithm_one after quad_table pushback" << std::endl;
    //quad_creation(op, result, arg1, NULL, UINT_MAX, line_op_counter);
    line_op_counter++;
    return;
}

void emit_bool(iopcode op, expression* result, expression* arg1, expression* arg2) {
    if (op == AND || op == OR) {
        quad_table.push_back(make_quad(op, result, arg1, arg2, UINT_MAX, line_op_counter));
        //quad_creation(op, result, arg1, arg2, UINT_MAX, line_op_counter);
        line_op_counter++;
    } else {
        quad_table.push_back(make_quad(op, result, arg1, NULL, UINT_MAX, line_op_counter));
        //quad_creation(op, result, arg1, NULL, UINT_MAX, line_op_counter);
        line_op_counter++;
    }
    return;
}

void emit_assign(iopcode op, expression* result, expression* arg1) {
    quad_table.push_back(make_quad(op, result, arg1, NULL, UINT_MAX, line_op_counter));
    //quad_creation(op, result, arg1, NULL, UINT_MAX, line_op_counter);
    line_op_counter++;
    return;
}

void emit_add(iopcode op, expression* result, expression* arg1, expression* arg2) {
    quad_table.push_back(make_quad(op, result, arg1, arg2, UINT_MAX, line_op_counter));
    //quad_creation(op, result, arg1, arg2, UINT_MAX, line_op_counter);
    line_op_counter++;
    return;
}

void emit_sub(iopcode op, expression* result, expression* arg1, expression* arg2) {
    quad_table.push_back(make_quad(op, result, arg1, arg2, UINT_MAX, line_op_counter));
    //quad_creation(op, result, arg1, arg2, UINT_MAX, line_op_counter);
    line_op_counter++;
    return;
}

void emit_mul(iopcode op, expression* result, expression* arg1, expression* arg2) {
    quad_table.push_back(make_quad(op, result, arg1, arg2, UINT_MAX, line_op_counter));
    //quad_creation(op, result, arg1, arg2, UINT_MAX, line_op_counter);
    line_op_counter++;
    return;
}

void emit_div(iopcode op, expression* result, expression* arg1, expression* arg2) {
    quad_table.push_back(make_quad(op, result, arg1, arg2, UINT_MAX, line_op_counter));
    //quad_creation(op, result, arg1, arg2, UINT_MAX, line_op_counter);
    line_op_counter++;
    return;
}

void emit_mod(iopcode op, expression* result, expression* arg1, expression* arg2) {
    quad_table.push_back(make_quad(op, result, arg1, arg2, UINT_MAX, line_op_counter));
    //quad_creation(op, result, arg1, arg2, UINT_MAX, line_op_counter);
    line_op_counter++;
    return;
}

void emit_if_greater(iopcode op, expression* result, expression* arg1, unsigned label) {
    //std::cout << "Quad Label is " << label << std::endl;
    quad_table.push_back(make_quad(op, result, arg1, NULL, label, line_op_counter));
    //quad_creation(op, result, arg1, NULL, label, line_op_counter);
    line_op_counter++;
    return;
}

void emit_if_less(iopcode op, expression* result, expression* arg1, unsigned label) {
    //std::cout << "Quad Label is " << label << std::endl;
    quad_table.push_back(make_quad(op, result, arg1, NULL, label, line_op_counter));
    //quad_creation(op, result, arg1, NULL, label, line_op_counter);
    line_op_counter++;
    return;
}

void emit_if_greatereq(iopcode op, expression* result, expression* arg1, unsigned label) {
    //std::cout << "Quad Label is " << label << std::endl;
    quad_table.push_back(make_quad(op, result, arg1, NULL, label, line_op_counter));
    //quad_creation(op, result, arg1, NULL, label, line_op_counter);
    line_op_counter++;
    return;
}

void emit_if_lesseq(iopcode op, expression* result, expression* arg1, unsigned label) {
    //std::cout << "Quad Label is " << label << std::endl;
    quad_table.push_back(make_quad(op, result, arg1, NULL, label, line_op_counter));
    //quad_creation(op, result, arg1, NULL, label, line_op_counter);
    line_op_counter++;
    return;
}

void emit_if_equal(iopcode op, expression* result, expression* arg1, unsigned label) {
    //std::cout << "Quad Label is " << label << std::endl;
    quad_table.push_back(make_quad(op, result, arg1, NULL, label, line_op_counter));
    //quad_creation(op, result, arg1, NULL, label, line_op_counter);
    line_op_counter++;
    return;
}

void emit_if_not_equal(iopcode op, expression* result, expression* arg1, unsigned label) {
    //std::cout << "Quad Label is " << label << std::endl;
    quad_table.push_back(make_quad(op, result, arg1, NULL, label, line_op_counter));
    //quad_creation(op, result, arg1, NULL, label, line_op_counter);
    line_op_counter++;
    return;
}

void emit_jump(iopcode op, unsigned label) {
    //std::cout << "Quad Label is " << op << std::endl;
    quad_table.push_back(make_quad(op, NULL, NULL, NULL, label, line_op_counter));
    //quad_creation(op, NULL, NULL, NULL, label, line_op_counter);
    line_op_counter++;
    return;
}

void emit_return(iopcode op, expression* expr) {
    //std::cout << "Quad Label is " << op << std::endl;
    quad_table.push_back(make_quad(op, expr, NULL, NULL, UINT_MAX, line_op_counter));
    //quad_creation(op, NULL, NULL, NULL, label, line_op_counter);
    line_op_counter++;
    return;
}

void emit_table_get_item(iopcode op, expression* result, expression* arg1, expression* arg2) {
    quad_table.push_back(make_quad(op, result, arg1, arg2, UINT_MAX, line_op_counter));
    //quad_creation(op, result, arg1, arg2, UINT_MAX, line_op_counter);
    line_op_counter++;
    return;
}

void emit_table_set_item(iopcode op, expression* result, expression* arg1, expression* arg2) {
    //std::cout << arg1->type << std::endl;
    quad_table.push_back(make_quad(op, result, arg1, arg2, UINT_MAX, line_op_counter));
    //quad_creation(op, result, arg1, arg2, UINT_MAX, line_op_counter);
    line_op_counter++;
    return;
}

expression* emit_if_table_item(expression* lvalue) {
    //std::cout << "L-value TYPE: " << lvalue->type << std::endl;
    if (lvalue->type != TABLEITEM_EXPR) {
        //std::cout << "HERE 1" << std::endl;
        return lvalue;
    } else {
        //std::cout << "HERE 1" << std::endl;
        SymbolTableEntry* new_temp = new_temp_var(lvalue->symbol->line, lvalue->symbol->type);
        //std::cout << new_temp.name << std::endl;
        expression* result = make_lvalue_expression(VAR_EXPR, get_symbol(new_temp->name, get_scope()));
        //std::cout << "L-VALUE->INDEX symbol " << lvalue->symbol->name << std::endl;
        //std::cout << "L-VALUE->INDEX type " << lvalue->index->bool_const << std::endl;
        emit_table_get_item(TABLEGETELEM, result, lvalue, lvalue->index);
        return result;
    }
}

void emit_funcdef(iopcode op, expression* result) {
    quad_table.push_back(make_quad(op, result, NULL, NULL, UINT_MAX, line_op_counter));
    //quad_creation(op, result, NULL, NULL, UINT_MAX, line_op_counter);
    line_op_counter++;

    return;
}

void emit_param(iopcode op, expression* result) {
    quad_table.push_back(make_quad(op, result, NULL, NULL, UINT_MAX, line_op_counter));
    //quad_creation(op, result, NULL, NULL, UINT_MAX, line_op_counter);
    line_op_counter++;
    return;
}

void emit_call(iopcode op, expression* result) {
    quad_table.push_back(make_quad(op, result, NULL, NULL, UINT_MAX, line_op_counter));
    //quad_creation(op, result, NULL, NULL, UINT_MAX, line_op_counter);
    line_op_counter++;
    return;
}

void emit_get_ret_val(iopcode op, expression* result) {
    quad_table.push_back(make_quad(op, result, NULL, NULL, UINT_MAX, line_op_counter));
    //quad_creation(op, result, NULL, NULL, UINT_MAX, line_op_counter);
    line_op_counter++;
    return;
}

void emit_table_create(iopcode op, expression* result) {
    quad_table.push_back(make_quad(op, result, NULL, NULL, UINT_MAX, line_op_counter));
    //quad_creation(op, result, NULL, NULL, UINT_MAX, line_op_counter);
    line_op_counter++;
    return;
}


expression* make_assign_expression(expr_value type, SymbolTableEntry* symbol) {
    expression* new_expression = new expression;

    new_expression->type = type;
    new_expression->symbol = symbol;

    //expression_list.push_back(new_expression);

    return new_expression;
}

expression* make_arithm_expression(expr_value type, SymbolTableEntry* symbol) {
    expression* new_expression = new expression;

    new_expression->type = type;
    new_expression->symbol = symbol;

    //expression_list.push_back(new_expression);

    return new_expression;
}

expression* make_bool_expression(expr_value type, SymbolTableEntry* symbol) {
    expression* new_expression = new expression;

    new_expression->type = type;
    new_expression->symbol = symbol;
    return new_expression;
}

expression* make_lvalue_expression(expr_value type, SymbolTableEntry* symbol) {
    //std::cout << "MAKE LVALUE EXPR" << std::endl;
    //std::cout << "MAKE LVALUE EXPR Symbol: " << symbol->name << std::endl;
    expression* new_expression = new expression;

    new_expression->type = type;
    new_expression->symbol = symbol;

    //std::cout << "MAKE LVALUE EXPR NEW EXPR Symbol: " << new_expression->symbol->name << std::endl;
    return new_expression;
}

expression* make_table_item_expression(expr_value type, SymbolTableEntry* symbol, expression* index) {
    expression* new_expression = new expression;

    new_expression->type = type;
    new_expression->symbol = symbol;
    new_expression->index = index;
    //expression_list.push_back(new_expression);

    return new_expression;

}

expression* make_constnum_expression(expr_value type, double const_num) {
    expression* new_expression = new expression;

    new_expression->type = type;
    //new_expression->symbol = symbol;
    //new_expression->index = index;
    //expression_list.push_back(new_expression);
    new_expression->num_const = const_num;
    return new_expression;
}

expression* make_conststring_expression(expr_value type, std::string name) {
    expression* new_expression = new expression;

    new_expression->type = type;
    //new_expression->symbol = symbol;
    //new_expression->index = index;
    //expression_list.push_back(new_expression);
    new_expression->str_const = name;
    return new_expression;
}

expression* make_constbool_expression(expr_value type, int value) {
    expression* new_expression = new expression;

    new_expression->type = type;
    //new_expression->symbol = symbol;
    //new_expression->index = index;
    //expression_list.push_back(new_expression);
    new_expression->bool_const = value;
    return new_expression;
}

expression* make_nil_expression(expr_value type) {
    expression* new_expression = new expression;

    new_expression->type = type;
    //new_expression->symbol = symbol;
    //new_expression->index = index;
    //expression_list.push_back(new_expression);
    //new_expression->bool_const = name;
    return new_expression;
}

expression* member_item(expression* lvalue, std::string name) {
    
    lvalue = emit_if_table_item(lvalue);
    //std::cout << "Name " << name << std::endl;
    expression* table_item = make_table_item_expression(TABLEITEM_EXPR, lvalue->symbol, make_conststring_expression(CONSTSTRING_EXPR, name));
    //std::cout << "Table Item Index is " << table_item->index->str_const << std::endl;
    return table_item;
}

expression* make_func_expression(expr_value type, SymbolTableEntry* symbol) {
    //std::cout << "MAKE FUNC EXPRESSION" << std::endl;
    expression* new_expression = new expression;

    new_expression->type = type;
    new_expression->symbol = symbol;
    
    //expression_list.push_back(new_expression);

    return new_expression;
}

expression* make_new_table_expression(expr_value type, SymbolTableEntry* symbol) {
    expression* new_expression = new expression;

    new_expression->type = type;
    new_expression->symbol = symbol;
    
    //expression_list.push_back(new_expression);

    return new_expression;
}

void quad_table_print() {
    std::cout << "INSTR. LINE" << "\t\t" << "OPERATION" << "\t\t" << "RESULT" << "\t\t" << "Arg 1" << "\t\t" << "Arg 2" << std::endl;
    for (auto& quad : quad_table) {
        switch (quad.op) {
            case 0:
                if (quad.arg1->type == CONSTNUM_EXPR) {
                    std::cout << quad.line << "\t\t" << "ASSIGN" << "\t\t" << quad.result->symbol->name << "\t\t" << quad.arg1->num_const << std::endl;
                } else if (quad.arg1->type == CONSTSTRING_EXPR) {
                    std::cout << quad.line << "\t\t" << "ASSIGN" << "\t\t" << quad.result->symbol->name << "\t\t" << quad.arg1->str_const << std::endl;
                } else if (quad.arg1->type == CONSTBOOL_EXPR) {
                    std::cout << quad.line << "\t\t" << "ASSIGN" << "\t\t" << quad.result->symbol->name << "\t\t" << quad.arg1->bool_const << std::endl;
                } else {
                    std::cout << quad.line << "\t\t" << "ASSIGN" << "\t\t" << quad.result->symbol->name << "\t\t" << quad.arg1->symbol->name << std::endl;
                }
                break;
            case 1:
                if (quad.arg1->type == CONSTNUM_EXPR) {
                    if (quad.arg2->type == CONSTNUM_EXPR) {
                        std::cout << quad.line << "\t\t" << "ADD" << "\t\t" << quad.result->num_const << "\t\t" << quad.arg1->num_const << "\t\t" << quad.arg2->num_const << std::endl;
                    } else if (quad.arg2->type == CONSTSTRING_EXPR) {
                        std::cout << quad.line << "\t\t" << "ADD" << "\t\t" << quad.result->symbol->name << "\t\t" << quad.arg1->num_const << "\t\t" << quad.arg2->str_const << std::endl;
                    } else if (quad.arg2->type == CONSTBOOL_EXPR) {
                        std::cout << quad.line << "\t\t" << "ADD" << "\t\t" << quad.result->symbol->name << "\t\t" << quad.arg1->num_const << "\t\t" << quad.arg2->bool_const << std::endl;
                    } else {
                        std::cout << quad.line << "\t\t" << "ADD" << "\t\t" << quad.result->symbol->name << "\t\t" << quad.arg1->num_const << "\t\t" << quad.arg2->symbol->name << std::endl;
                    }
                } else if (quad.arg2-> type == CONSTNUM_EXPR) {
                    if (quad.arg1->type == CONSTNUM_EXPR) {
                        std::cout << quad.line << "\t\t" << "ADD" << "\t\t" << quad.result->num_const << "\t\t" << quad.arg1->num_const << "\t\t" << quad.arg2->num_const << std::endl;
                    } else if (quad.arg1->type == CONSTSTRING_EXPR) {
                        std::cout << quad.line << "\t\t" << "ADD" << "\t\t" << quad.result->symbol->name << "\t\t" << quad.arg1->str_const << "\t\t" << quad.arg2->num_const << std::endl;
                    } else if (quad.arg1->type == CONSTBOOL_EXPR) {
                        std::cout << quad.line << "\t\t" << "ADD" << "\t\t" << quad.result->symbol->name << "\t\t" << quad.arg1->bool_const << "\t\t" << quad.arg2->num_const << std::endl;
                    } else {
                        std::cout << quad.line << "\t\t" << "ADD" << "\t\t" << quad.result->symbol->name << "\t\t" << quad.arg1->symbol->name << "\t\t" << quad.arg2->num_const << std::endl;
                    }
                } else if (quad.arg1->type == CONSTBOOL_EXPR) {
                    if (quad.arg2->type == CONSTNUM_EXPR) {
                        std::cout << quad.line << "\t\t" << "ADD" << "\t\t" << quad.result->symbol->name << "\t\t" << quad.arg1->bool_const << "\t\t" << quad.arg2->num_const << std::endl;
                    } else if (quad.arg2->type == CONSTSTRING_EXPR) {
                        std::cout << quad.line << "\t\t" << "ADD" << "\t\t" << quad.result->symbol->name << "\t\t" << quad.arg1->bool_const << "\t\t" << quad.arg2->str_const << std::endl;
                    } else if (quad.arg2->type == CONSTBOOL_EXPR) {
                        std::cout << quad.line << "\t\t" << "ADD" << "\t\t" << quad.result->symbol->name << "\t\t" << quad.arg1->bool_const << "\t\t" << quad.arg2->bool_const << std::endl;
                    } else {
                        std::cout << quad.line << "\t\t" << "ADD" << "\t\t" << quad.result->symbol->name << "\t\t" << quad.arg1->bool_const << "\t\t" << quad.arg2->symbol->name << std::endl;
                    }
                } else if (quad.arg2->type == CONSTBOOL_EXPR) {
                    if (quad.arg1->type == CONSTNUM_EXPR) {
                        std::cout << quad.line << "\t\t" << "ADD" << "\t\t" << quad.result->symbol->name << "\t\t" << quad.arg1->num_const << "\t\t" << quad.arg2->bool_const << std::endl;
                    } else if (quad.arg1->type == CONSTSTRING_EXPR) {
                        std::cout << quad.line << "\t\t" << "ADD" << "\t\t" << quad.result->symbol->name << "\t\t" << quad.arg1->str_const << "\t\t" << quad.arg2->bool_const << std::endl;
                    } else if (quad.arg1->type == CONSTBOOL_EXPR) {
                        std::cout << quad.line << "\t\t" << "ADD" << "\t\t" << quad.result->symbol->name << "\t\t" << quad.arg1->bool_const << "\t\t" << quad.arg2->bool_const << std::endl;
                    } else {
                        std::cout << quad.line << "\t\t" << "ADD" << "\t\t" << quad.result->symbol->name << "\t\t" << quad.arg1->symbol->name << "\t\t" << quad.arg2->bool_const << std::endl;
                    }
                } else {
                    std::cout << quad.line << "\t\t" << "ADD" << "\t\t" << quad.result->symbol->name << "\t\t" << quad.arg1->symbol->name << "\t\t" << quad.arg2->symbol->name << std::endl;
                }
                break;
            case 2:
                if (quad.arg1->type == CONSTNUM_EXPR) {
                    if (quad.arg2->type == CONSTNUM_EXPR) {
                        std::cout << quad.line << "\t\t" << "SUB" << "\t\t" << quad.result->num_const << "\t\t" << quad.arg1->num_const << "\t\t" << quad.arg2->num_const << std::endl;
                    } else if (quad.arg2->type == CONSTSTRING_EXPR) {
                        std::cout << quad.line << "\t\t" << "SUB" << "\t\t" << quad.result->symbol->name << "\t\t" << quad.arg1->num_const << "\t\t" << quad.arg2->str_const << std::endl;
                    } else if (quad.arg2->type == CONSTBOOL_EXPR) {
                        std::cout << quad.line << "\t\t" << "SUB" << "\t\t" << quad.result->symbol->name << "\t\t" << quad.arg1->num_const << "\t\t" << quad.arg2->bool_const << std::endl;
                    } else {
                        std::cout << quad.line << "\t\t" << "SUB" << "\t\t" << quad.result->symbol->name << "\t\t" << quad.arg1->num_const << "\t\t" << quad.arg2->symbol->name << std::endl;
                    }
                } else if (quad.arg2-> type == CONSTNUM_EXPR) {
                    if (quad.arg1->type == CONSTNUM_EXPR) {
                        std::cout << quad.line << "\t\t" << "SUB" << "\t\t" << quad.result->num_const << "\t\t" << quad.arg1->num_const << "\t\t" << quad.arg2->num_const << std::endl;
                    } else if (quad.arg1->type == CONSTSTRING_EXPR) {
                        std::cout << quad.line << "\t\t" << "SUB" << "\t\t" << quad.result->symbol->name << "\t\t" << quad.arg1->str_const << "\t\t" << quad.arg2->num_const << std::endl;
                    } else if (quad.arg1->type == CONSTBOOL_EXPR) {
                        std::cout << quad.line << "\t\t" << "SUB" << "\t\t" << quad.result->symbol->name << "\t\t" << quad.arg1->bool_const << "\t\t" << quad.arg2->num_const << std::endl;
                    } else {
                        std::cout << quad.line << "\t\t" << "SUB" << "\t\t" << quad.result->symbol->name << "\t\t" << quad.arg1->symbol->name << "\t\t" << quad.arg2->num_const << std::endl;
                    }
                } else if (quad.arg1->type == CONSTBOOL_EXPR) {
                    if (quad.arg2->type == CONSTNUM_EXPR) {
                        std::cout << quad.line << "\t\t" << "SUB" << "\t\t" << quad.result->symbol->name << "\t\t" << quad.arg1->bool_const << "\t\t" << quad.arg2->num_const << std::endl;
                    } else if (quad.arg2->type == CONSTSTRING_EXPR) {
                        std::cout << quad.line << "\t\t" << "SUB" << "\t\t" << quad.result->symbol->name << "\t\t" << quad.arg1->bool_const << "\t\t" << quad.arg2->str_const << std::endl;
                    } else if (quad.arg2->type == CONSTBOOL_EXPR) {
                        std::cout << quad.line << "\t\t" << "SUB" << "\t\t" << quad.result->symbol->name << "\t\t" << quad.arg1->bool_const << "\t\t" << quad.arg2->bool_const << std::endl;
                    } else {
                        std::cout << quad.line << "\t\t" << "SUB" << "\t\t" << quad.result->symbol->name << "\t\t" << quad.arg1->bool_const << "\t\t" << quad.arg2->symbol->name << std::endl;
                    }
                } else if (quad.arg2->type == CONSTBOOL_EXPR) {
                    if (quad.arg1->type == CONSTNUM_EXPR) {
                        std::cout << quad.line << "\t\t" << "SUB" << "\t\t" << quad.result->symbol->name << "\t\t" << quad.arg1->num_const << "\t\t" << quad.arg2->bool_const << std::endl;
                    } else if (quad.arg1->type == CONSTSTRING_EXPR) {
                        std::cout << quad.line << "\t\t" << "SUB" << "\t\t" << quad.result->symbol->name << "\t\t" << quad.arg1->str_const << "\t\t" << quad.arg2->bool_const << std::endl;
                    } else if (quad.arg1->type == CONSTBOOL_EXPR) {
                        std::cout << quad.line << "\t\t" << "SUB" << "\t\t" << quad.result->symbol->name << "\t\t" << quad.arg1->bool_const << "\t\t" << quad.arg2->bool_const << std::endl;
                    } else {
                        std::cout << quad.line << "\t\t" << "SUB" << "\t\t" << quad.result->symbol->name << "\t\t" << quad.arg1->symbol->name << "\t\t" << quad.arg2->bool_const << std::endl;
                    }
                } else {
                    std::cout << quad.line << "\t\t" << "SUB" << "\t\t" << quad.result->symbol->name << "\t\t" << quad.arg1->symbol->name << "\t\t" << quad.arg2->symbol->name << std::endl;
                }
                break;
            case 3:
                if (quad.arg1->type == CONSTNUM_EXPR) {
                    if (quad.arg2->type == CONSTNUM_EXPR) {
                        std::cout << quad.line << "\t\t" << "MUL" << "\t\t" << quad.result->num_const << "\t\t" << quad.arg1->num_const << "\t\t" << quad.arg2->num_const << std::endl; 
                    } else if (quad.arg2->type == CONSTSTRING_EXPR) {
                        std::cout << quad.line << "\t\t" << "MUL" << "\t\t" << quad.result->symbol->name << "\t\t" << quad.arg1->num_const << "\t\t" << quad.arg2->str_const << std::endl;
                    } else if (quad.arg2->type == CONSTBOOL_EXPR) {
                        std::cout << quad.line << "\t\t" << "MUL" << "\t\t" << quad.result->symbol->name << "\t\t" << quad.arg1->num_const << "\t\t" << quad.arg2->bool_const << std::endl;
                    } else {
                        std::cout << quad.line << "\t\t" << "MUL" << "\t\t" << quad.result->symbol->name << "\t\t" << quad.arg1->num_const << "\t\t" << quad.arg2->symbol->name << std::endl;
                    }
                } else if (quad.arg2-> type == CONSTNUM_EXPR) {
                    if (quad.arg1->type == CONSTNUM_EXPR) {
                        std::cout << quad.line << "\t\t" << "MUL" << "\t\t" << quad.result->symbol->name << "\t\t" << quad.arg1->num_const << "\t\t" << quad.arg2->num_const << std::endl;
                    } else if (quad.arg1->type == CONSTSTRING_EXPR) {
                        std::cout << quad.line << "\t\t" << "MUL" << "\t\t" << quad.result->symbol->name << "\t\t" << quad.arg1->str_const << "\t\t" << quad.arg2->num_const << std::endl;
                    } else if (quad.arg1->type == CONSTBOOL_EXPR) {
                        std::cout << quad.line << "\t\t" << "MUL" << "\t\t" << quad.result->symbol->name << "\t\t" << quad.arg1->bool_const << "\t\t" << quad.arg2->num_const << std::endl;
                    } else {
                        std::cout << quad.line << "\t\t" << "MUL" << "\t\t" << quad.result->symbol->name << "\t\t" << quad.arg1->symbol->name << "\t\t" << quad.arg2->num_const << std::endl;
                    }
                } else if (quad.arg1->type == CONSTBOOL_EXPR) {
                    if (quad.arg2->type == CONSTNUM_EXPR) {
                        std::cout << quad.line << "\t\t" << "MUL" << "\t\t" << quad.result->symbol->name << "\t\t" << quad.arg1->bool_const << "\t\t" << quad.arg2->num_const << std::endl;
                    } else if (quad.arg2->type == CONSTSTRING_EXPR) {
                        std::cout << quad.line << "\t\t" << "MUL" << "\t\t" << quad.result->symbol->name << "\t\t" << quad.arg1->bool_const << "\t\t" << quad.arg2->str_const << std::endl;
                    } else if (quad.arg2->type == CONSTBOOL_EXPR) {
                        std::cout << quad.line << "\t\t" << "MUL" << "\t\t" << quad.result->symbol->name << "\t\t" << quad.arg1->bool_const << "\t\t" << quad.arg2->bool_const << std::endl;
                    } else {
                        std::cout << quad.line << "\t\t" << "MUL" << "\t\t" << quad.result->symbol->name << "\t\t" << quad.arg1->bool_const << "\t\t" << quad.arg2->symbol->name << std::endl;
                    }
                } else if (quad.arg2->type == CONSTBOOL_EXPR) {
                    if (quad.arg1->type == CONSTNUM_EXPR) {
                        std::cout << quad.line << "\t\t" << "MUL" << "\t\t" << quad.result->symbol->name << "\t\t" << quad.arg1->num_const << "\t\t" << quad.arg2->bool_const << std::endl;
                    } else if (quad.arg1->type == CONSTSTRING_EXPR) {
                        std::cout << quad.line << "\t\t" << "MUL" << "\t\t" << quad.result->symbol->name << "\t\t" << quad.arg1->str_const << "\t\t" << quad.arg2->bool_const << std::endl;
                    } else if (quad.arg1->type == CONSTBOOL_EXPR) {
                        std::cout << quad.line << "\t\t" << "MUL" << "\t\t" << quad.result->symbol->name << "\t\t" << quad.arg1->bool_const << "\t\t" << quad.arg2->bool_const << std::endl;
                    } else {
                        std::cout << quad.line << "\t\t" << "MUL" << "\t\t" << quad.result->symbol->name << "\t\t" << quad.arg1->symbol->name << "\t\t" << quad.arg2->bool_const << std::endl;
                    }
                } else {
                    std::cout << quad.line << "\t\t" << "MUL" << "\t\t" << quad.result->symbol->name << "\t\t" << quad.arg1->symbol->name << "\t\t" << quad.arg2->symbol->name << std::endl;
                }
                break;
            case 4:
                if (quad.arg1->type == CONSTNUM_EXPR) {
                    if (quad.arg2->type == CONSTNUM_EXPR) {
                        std::cout << quad.line << "\t\t" << "DIV" << "\t\t" << quad.result->num_const << "\t\t" << quad.arg1->num_const << "\t\t" << quad.arg2->num_const << std::endl; 
                    } else if (quad.arg2->type == CONSTSTRING_EXPR) {
                        std::cout << quad.line << "\t\t" << "DIV" << "\t\t" << quad.result->symbol->name << "\t\t" << quad.arg1->num_const << "\t\t" << quad.arg2->str_const << std::endl;
                    } else if (quad.arg2->type == CONSTBOOL_EXPR) {
                        std::cout << quad.line << "\t\t" << "DIV" << "\t\t" << quad.result->symbol->name << "\t\t" << quad.arg1->num_const << "\t\t" << quad.arg2->bool_const << std::endl;
                    } else {
                        std::cout << quad.line << "\t\t" << "DIV" << "\t\t" << quad.result->symbol->name << "\t\t" << quad.arg1->num_const << "\t\t" << quad.arg2->symbol->name << std::endl;
                    }
                } else if (quad.arg2-> type == CONSTNUM_EXPR) {
                    if (quad.arg1->type == CONSTNUM_EXPR) {
                        std::cout << quad.line << "\t\t" << "DIV" << "\t\t" << quad.result->symbol->name << "\t\t" << quad.arg1->num_const << "\t\t" << quad.arg2->num_const << std::endl;
                    } else if (quad.arg1->type == CONSTSTRING_EXPR) {
                        std::cout << quad.line << "\t\t" << "DIV" << "\t\t" << quad.result->symbol->name << "\t\t" << quad.arg1->str_const << "\t\t" << quad.arg2->num_const << std::endl;
                    } else if (quad.arg1->type == CONSTBOOL_EXPR) {
                        std::cout << quad.line << "\t\t" << "DIV" << "\t\t" << quad.result->symbol->name << "\t\t" << quad.arg1->bool_const << "\t\t" << quad.arg2->num_const << std::endl;
                    } else {
                        std::cout << quad.line << "\t\t" << "DIV" << "\t\t" << quad.result->symbol->name << "\t\t" << quad.arg1->symbol->name << "\t\t" << quad.arg2->num_const << std::endl;
                    }
                } else if (quad.arg1->type == CONSTBOOL_EXPR) {
                    if (quad.arg2->type == CONSTNUM_EXPR) {
                        std::cout << quad.line << "\t\t" << "DIV" << "\t\t" << quad.result->symbol->name << "\t\t" << quad.arg1->bool_const << "\t\t" << quad.arg2->num_const << std::endl;
                    } else if (quad.arg2->type == CONSTSTRING_EXPR) {
                        std::cout << quad.line << "\t\t" << "DIV" << "\t\t" << quad.result->symbol->name << "\t\t" << quad.arg1->bool_const << "\t\t" << quad.arg2->str_const << std::endl;
                    } else if (quad.arg2->type == CONSTBOOL_EXPR) {
                        std::cout << quad.line << "\t\t" << "DIV" << "\t\t" << quad.result->symbol->name << "\t\t" << quad.arg1->bool_const << "\t\t" << quad.arg2->bool_const << std::endl;
                    } else {
                        std::cout << quad.line << "\t\t" << "DIV" << "\t\t" << quad.result->symbol->name << "\t\t" << quad.arg1->bool_const << "\t\t" << quad.arg2->symbol->name << std::endl;
                    }
                } else if (quad.arg2->type == CONSTBOOL_EXPR) {
                    if (quad.arg1->type == CONSTNUM_EXPR) {
                        std::cout << quad.line << "\t\t" << "DIV" << "\t\t" << quad.result->symbol->name << "\t\t" << quad.arg1->num_const << "\t\t" << quad.arg2->bool_const << std::endl;
                    } else if (quad.arg1->type == CONSTSTRING_EXPR) {
                        std::cout << quad.line << "\t\t" << "DIV" << "\t\t" << quad.result->symbol->name << "\t\t" << quad.arg1->str_const << "\t\t" << quad.arg2->bool_const << std::endl;
                    } else if (quad.arg1->type == CONSTBOOL_EXPR) {
                        std::cout << quad.line << "\t\t" << "DIV" << "\t\t" << quad.result->symbol->name << "\t\t" << quad.arg1->bool_const << "\t\t" << quad.arg2->bool_const << std::endl;
                    } else {
                        std::cout << quad.line << "\t\t" << "DIV" << "\t\t" << quad.result->symbol->name << "\t\t" << quad.arg1->symbol->name << "\t\t" << quad.arg2->bool_const << std::endl;
                    }
                } else {
                    std::cout << quad.line << "\t\t" << "DIV" << "\t\t" << quad.result->symbol->name << "\t\t" << quad.arg1->symbol->name << "\t\t" << quad.arg2->symbol->name << std::endl;
                }
                break;
            case 5:
                if (quad.arg1->type == CONSTNUM_EXPR) {
                    if (quad.arg2->type == CONSTNUM_EXPR) {
                        std::cout << quad.line << "\t\t" << "MOD" << "\t\t" << quad.result->num_const << "\t\t" << quad.arg1->num_const << "\t\t" << quad.arg2->num_const << std::endl; 
                    } else if (quad.arg2->type == CONSTSTRING_EXPR) {
                        std::cout << quad.line << "\t\t" << "MOD" << "\t\t" << quad.result->symbol->name << "\t\t" << quad.arg1->num_const << "\t\t" << quad.arg2->str_const << std::endl;
                    } else if (quad.arg2->type == CONSTBOOL_EXPR) {
                        std::cout << quad.line << "\t\t" << "MOD" << "\t\t" << quad.result->symbol->name << "\t\t" << quad.arg1->num_const << "\t\t" << quad.arg2->bool_const << std::endl;
                    } else {
                        std::cout << quad.line << "\t\t" << "MOD" << "\t\t" << quad.result->symbol->name << "\t\t" << quad.arg1->num_const << "\t\t" << quad.arg2->symbol->name << std::endl;
                    }
                } else if (quad.arg2-> type == CONSTNUM_EXPR) {
                    if (quad.arg1->type == CONSTNUM_EXPR) {
                        std::cout << quad.line << "\t\t" << "MOD" << "\t\t" << quad.result->symbol->name << "\t\t" << quad.arg1->num_const << "\t\t" << quad.arg2->num_const << std::endl;
                    } else if (quad.arg1->type == CONSTSTRING_EXPR) {
                        std::cout << quad.line << "\t\t" << "MOD" << "\t\t" << quad.result->symbol->name << "\t\t" << quad.arg1->str_const << "\t\t" << quad.arg2->num_const << std::endl;
                    } else if (quad.arg1->type == CONSTBOOL_EXPR) {
                        std::cout << quad.line << "\t\t" << "MOD" << "\t\t" << quad.result->symbol->name << "\t\t" << quad.arg1->bool_const << "\t\t" << quad.arg2->num_const << std::endl;
                    } else {
                        std::cout << quad.line << "\t\t" << "MOD" << "\t\t" << quad.result->symbol->name << "\t\t" << quad.arg1->symbol->name << "\t\t" << quad.arg2->num_const << std::endl;
                    }
                } else if (quad.arg1->type == CONSTBOOL_EXPR) {
                    if (quad.arg2->type == CONSTNUM_EXPR) {
                        std::cout << quad.line << "\t\t" << "MOD" << "\t\t" << quad.result->symbol->name << "\t\t" << quad.arg1->bool_const << "\t\t" << quad.arg2->num_const << std::endl;
                    } else if (quad.arg2->type == CONSTSTRING_EXPR) {
                        std::cout << quad.line << "\t\t" << "MOD" << "\t\t" << quad.result->symbol->name << "\t\t" << quad.arg1->bool_const << "\t\t" << quad.arg2->str_const << std::endl;
                    } else if (quad.arg2->type == CONSTBOOL_EXPR) {
                        std::cout << quad.line << "\t\t" << "MOD" << "\t\t" << quad.result->symbol->name << "\t\t" << quad.arg1->bool_const << "\t\t" << quad.arg2->bool_const << std::endl;
                    } else {
                        std::cout << quad.line << "\t\t" << "MOD" << "\t\t" << quad.result->symbol->name << "\t\t" << quad.arg1->bool_const << "\t\t" << quad.arg2->symbol->name << std::endl;
                    }
                } else if (quad.arg2->type == CONSTBOOL_EXPR) {
                    if (quad.arg1->type == CONSTNUM_EXPR) {
                        std::cout << quad.line << "\t\t" << "MOD" << "\t\t" << quad.result->symbol->name << "\t\t" << quad.arg1->num_const << "\t\t" << quad.arg2->bool_const << std::endl;
                    } else if (quad.arg1->type == CONSTSTRING_EXPR) {
                        std::cout << quad.line << "\t\t" << "MOD" << "\t\t" << quad.result->symbol->name << "\t\t" << quad.arg1->str_const << "\t\t" << quad.arg2->bool_const << std::endl;
                    } else if (quad.arg1->type == CONSTBOOL_EXPR) {
                        std::cout << quad.line << "\t\t" << "MOD" << "\t\t" << quad.result->symbol->name << "\t\t" << quad.arg1->bool_const << "\t\t" << quad.arg2->bool_const << std::endl;
                    } else {
                        std::cout << quad.line << "\t\t" << "MOD" << "\t\t" << quad.result->symbol->name << "\t\t" << quad.arg1->symbol->name << "\t\t" << quad.arg2->bool_const << std::endl;
                    }
                } else {
                    std::cout << quad.line << "\t\t" << "MOD" << "\t\t" << quad.result->symbol->name << "\t\t" << quad.arg1->symbol->name << "\t\t" << quad.arg2->symbol->name << std::endl;
                }
                break;
            case 6:
                if (quad.arg1->type == CONSTNUM_EXPR){
                    std::cout << quad.line << "\t\t" << "UMINUS" << "\t\t" << quad.result->symbol->name << "\t\t" << quad.arg1->num_const << std::endl;
                } else {
                    std::cout << quad.line << "\t\t" << "UMINUS" << "\t\t" << quad.result->symbol->name << "\t\t" << quad.arg1->symbol->name << std::endl;
                }
                break;
            case 7:
                std::cout << quad.line << "\t\t" << "AND" << "\t\t" << quad.result->symbol->name << "\t\t" << quad.arg1->symbol->name << "\t\t" << quad.arg2->symbol->name << std::endl;
                break;
            case 8:
                std::cout << quad.line << "\t\t" << "OR" << "\t\t" << quad.result->symbol->name << "\t\t" << quad.arg1->symbol->name << "\t\t" << quad.arg2->symbol->name << std::endl;
                break;
            case 9:
                if (quad.arg1->type == CONSTNUM_EXPR) {
                    std::cout << quad.line << "\t\t" << "NOT" << "\t\t" << quad.result->bool_const << "\t\t" << quad.arg1->num_const << std::endl;
                } else if (quad.arg1->type == CONSTBOOL_EXPR) {
                    std::cout << quad.line << "\t\t" << "NOT" << "\t\t" << quad.result->bool_const << "\t\t" << quad.arg1->bool_const << std::endl;
                } else {
                    std::cout << quad.line << "\t\t" << "NOT" << "\t\t" << quad.result->bool_const << "\t\t" << quad.arg1->symbol->name << std::endl;
                }
                break;
            case 10:
                if (quad.result->type == CONSTNUM_EXPR) {
                    if (quad.arg1->type == CONSTNUM_EXPR) { 
                        std::cout << quad.line << "\t" << "IF EQUAL" << "\t\t" << quad.result->num_const << "\t\t" << quad.arg1->num_const << "\t\t" << quad.label << std::endl; 
                    } else if (quad.arg1->type == CONSTSTRING_EXPR) {
                        std::cout << quad.line << "Error: the symbol" << "\t\t" << quad.arg1->str_const << " is a string" << std::endl;
                    } else {
                        std::cout << quad.line << "\t" << "IF EQUAL" << "\t\t" << quad.result->num_const << "\t\t" << quad.arg1->bool_const << "\t\t" << quad.label << std::endl;
                    }
                } else if (quad.result->type == CONSTSTRING_EXPR) {
                    std::cout << quad.line << "Error: the symbol" << "\t\t" << quad.result->str_const << " is a string" << std::endl;
                } else if (quad.result->type == CONSTBOOL_EXPR) {
                    std::cout << quad.line << "\t" << "IF EQUAL" << "\t\t" << quad.result->bool_const << "\t\t" << quad.arg1->bool_const << "\t\t" << quad.label << std::endl;
                } else {
                    if (quad.arg1->type == CONSTNUM_EXPR) { 
                        std::cout << quad.line << "\t" << "IF EQUAL" << "\t\t" << quad.result->symbol->name << "\t\t" << quad.arg1->num_const << "\t\t" << quad.label << std::endl; 
                    } else if (quad.arg1->type == CONSTSTRING_EXPR) {
                        std::cout << quad.line << "Error: the symbol" << "\t\t" << quad.arg1->str_const << " is a string" << std::endl;
                    } else if (quad.arg1->type == CONSTBOOL_EXPR) {
                        std::cout << quad.line << "\t" << "IF EQUAL" << "\t\t" << quad.result->symbol->name << "\t\t" << quad.arg1->bool_const << "\t\t" << quad.label << std::endl;
                    } else {
                        std::cout << quad.line << "\t" << "IF EQUAL" << "\t\t" << quad.result->symbol->name << "\t\t" << quad.arg1->symbol->name << "\t\t" << quad.label << std::endl;
                    }
                }
                break;
            case 11:
                if (quad.result->type == CONSTNUM_EXPR) {
                    if (quad.arg1->type == CONSTNUM_EXPR) { 
                        std::cout << quad.line << "\t" << "IF NOT EQUAL" << "\t\t" << quad.result->num_const << "\t\t" << quad.arg1->num_const << "\t\t" << quad.label << std::endl; 
                    } else if (quad.arg1->type == CONSTSTRING_EXPR) {
                        std::cout << quad.line << "Error: the symbol" << "\t\t" << quad.arg1->str_const << " is a string" << std::endl;
                    } else {
                        std::cout << quad.line << "\t" << "IF NOT EQUAL" << "\t\t" << quad.result->num_const << "\t\t" << quad.arg1->bool_const << "\t\t" << quad.label << std::endl;
                    }
                } else if (quad.result->type == CONSTSTRING_EXPR) {
                    std::cout << quad.line << "Error: the symbol" << "\t\t" << quad.result->str_const << " is a string" << std::endl;
                } else if (quad.result->type == CONSTBOOL_EXPR) {
                    std::cout << quad.line << "\t" << "IF NOT EQUAL" << "\t\t" << quad.result->bool_const << "\t\t" << quad.arg1->bool_const << "\t\t" << quad.label << std::endl;
                } else {
                    if (quad.arg1->type == CONSTNUM_EXPR) { 
                        std::cout << quad.line << "\t" << "IF NOT EQUAL" << "\t\t" << quad.result->symbol->name << "\t\t" << quad.arg1->num_const << "\t\t" << quad.label << std::endl; 
                    } else if (quad.arg1->type == CONSTSTRING_EXPR) {
                        std::cout << quad.line << "Error: the symbol" << "\t\t" << quad.arg1->str_const << " is a string" << std::endl;
                    } else if (quad.arg1->type == CONSTBOOL_EXPR) {
                        std::cout << quad.line << "\t" << "IF NOT EQUAL" << "\t\t" << quad.result->symbol->name << "\t\t" << quad.arg1->bool_const << "\t\t" << quad.label << std::endl;
                    } else {
                        std::cout << quad.line << "\t" << "IF NOT EQUAL" << "\t\t" << quad.result->symbol->name << "\t\t" << quad.arg1->symbol->name << "\t\t" << quad.label << std::endl;
                    }
                }
                break;
            case 12:
                if (quad.result->type == CONSTNUM_EXPR) {
                    if (quad.arg1->type == CONSTNUM_EXPR) { 
                        std::cout << quad.line << "\t" << "IF LESS EQUAL" << "\t\t" << quad.result->num_const << "\t\t" << quad.arg1->num_const << "\t\t" << quad.label << std::endl; 
                    } else if (quad.arg1->type == CONSTSTRING_EXPR) {
                        std::cout << quad.line << "Error: the symbol" << "\t\t" << quad.arg1->str_const << " is a string" << std::endl;
                    } else {
                        std::cout << quad.line << "\t" << "IF LESS EQUAL" << "\t\t" << quad.result->num_const << "\t\t" << quad.arg1->bool_const << "\t\t" << quad.label << std::endl;
                    }
                } else if (quad.result->type == CONSTSTRING_EXPR) {
                    std::cout << quad.line << "Error: the symbol" << "\t\t" << quad.result->str_const << " is a string" << std::endl;
                } else if (quad.result->type == CONSTBOOL_EXPR) {
                    std::cout << quad.line << "\t" << "IF LESS EQUAL" << "\t\t" << quad.result->bool_const << "\t\t" << quad.arg1->bool_const << "\t\t" << quad.label << std::endl;
                } else {
                    if (quad.arg1->type == CONSTNUM_EXPR) { 
                        std::cout << quad.line << "\t" << "IF LESS EQUAL" << "\t\t" << quad.result->symbol->name << "\t\t" << quad.arg1->num_const << "\t\t" << quad.label << std::endl; 
                    } else if (quad.arg1->type == CONSTSTRING_EXPR) {
                        std::cout << quad.line << "Error: the symbol" << "\t\t" << quad.arg1->str_const << " is a string" << std::endl;
                    } else if (quad.arg1->type == CONSTBOOL_EXPR) {
                        std::cout << quad.line << "\t" << "IF LESS EQUAL" << "\t\t" << quad.result->symbol->name << "\t\t" << quad.arg1->bool_const << "\t\t" << quad.label << std::endl;
                    } else {
                        std::cout << quad.line << "\t" << "IF LESS EQUAL" << "\t\t" << quad.result->symbol->name << "\t\t" << quad.arg1->symbol->name << "\t\t" << quad.label << std::endl;
                    }
                }
                break;
            case 13:
                if (quad.result->type == CONSTNUM_EXPR) {
                    if (quad.arg1->type == CONSTNUM_EXPR) { 
                        std::cout << quad.line << "\t" << "IF GREATER EQUAL" << "\t\t" << quad.result->num_const << "\t\t" << quad.arg1->num_const << "\t\t" << quad.label << std::endl; 
                    } else if (quad.arg1->type == CONSTSTRING_EXPR) {
                        std::cout << quad.line << "Error: the symbol" << "\t\t" << quad.arg1->str_const << " is a string" << std::endl;
                    } else {
                        std::cout << quad.line << "\t" << "IF GREATER EQUAL" << "\t\t" << quad.result->num_const << "\t\t" << quad.arg1->bool_const << "\t\t" << quad.label << std::endl;
                    }
                } else if (quad.result->type == CONSTSTRING_EXPR) {
                    std::cout << quad.line << "Error: the symbol" << "\t\t" << quad.result->str_const << " is a string" << std::endl;
                } else if (quad.result->type == CONSTBOOL_EXPR) {
                    std::cout << quad.line << "\t" << "IF GREATER EQUAL" << "\t\t" << quad.result->bool_const << "\t\t" << quad.arg1->bool_const << "\t\t" << quad.label << std::endl;
                } else {
                    if (quad.arg1->type == CONSTNUM_EXPR) { 
                        std::cout << quad.line << "\t" << "IF GREATER EQUAL" << "\t\t" << quad.result->symbol->name << "\t\t" << quad.arg1->num_const << "\t\t" << quad.label << std::endl; 
                    } else if (quad.arg1->type == CONSTSTRING_EXPR) {
                        std::cout << quad.line << "Error: the symbol" << "\t\t" << quad.arg1->str_const << " is a string" << std::endl;
                    } else if (quad.arg1->type == CONSTBOOL_EXPR) {
                        std::cout << quad.line << "\t" << "IF GREATER EQUAL" << "\t\t" << quad.result->symbol->name << "\t\t" << quad.arg1->bool_const << "\t\t" << quad.label << std::endl;
                    } else {
                        std::cout << quad.line << "\t" << "IF GREATER EQUAL" << "\t\t" << quad.result->symbol->name << "\t\t" << quad.arg1->symbol->name << "\t\t" << quad.label << std::endl;
                    }
                }
                break;
            case 14:
                if (quad.result->type == CONSTNUM_EXPR) {
                    if (quad.arg1->type == CONSTNUM_EXPR) { 
                        std::cout << "\n" << quad.line << "\t" << "IF LESS" << "\t\t" << quad.result->num_const << "\t\t" << quad.arg1->num_const << "\t\t" << quad.label << std::endl; 
                    } else if (quad.arg1->type == CONSTSTRING_EXPR) {
                        std::cout << "\n" << quad.line << "Error: the symbol" << "\t\t" << quad.arg1->str_const << " is a string";
                    } else {
                        std::cout << "\n" << quad.line << "\t" << "IF LESS" << "\t\t" << quad.result->num_const << "\t\t" << quad.arg1->symbol->name << "\t\t" << quad.label << std::endl;
                    }
                } else if (quad.result->type == CONSTSTRING_EXPR) {
                    std::cout << "\n" << quad.line << "Error: the symbol" << "\t\t" << quad.result->str_const << " is a string";
                } else {
                    if (quad.arg1->type == CONSTNUM_EXPR) { 
                        std::cout << "\n" << quad.line << "\t" << "IF LESS" << "\t\t" << quad.result->symbol->name << "\t\t" << quad.arg1->num_const << "\t\t" << quad.label << std::endl; 
                    } else if (quad.arg1->type == CONSTSTRING_EXPR) {
                        std::cout << "\n" << quad.line << "Error: the symbol" << "\t\t" << quad.arg1->str_const << " is a string";
                    } else {
                        std::cout << "\n" << quad.line << "\t" << "IF LESS" << "\t\t" << quad.result->symbol->name << "\t\t" << quad.arg1->symbol->name << "\t\t" << quad.label << std::endl;
                    }
                }
                break;
            case 15:
                if (quad.result->type == CONSTNUM_EXPR) {
                    if (quad.arg1->type == CONSTNUM_EXPR) { 
                        std::cout << quad.line << "\t" << "IF GREATER" << "\t\t" << quad.result->num_const << "\t\t" << quad.arg1->num_const << "\t\t" << quad.label << std::endl; 
                    } else if (quad.arg1->type == CONSTSTRING_EXPR) {
                        std::cout << quad.line << "Error: the symbol" << "\t\t" << quad.arg1->str_const << " is a string" << std::endl;
                    } else {
                        std::cout << quad.line << "\t" << "IF GREATER" << "\t\t" << quad.result->num_const << "\t\t" << quad.arg1->bool_const << "\t\t" << quad.label << std::endl;
                    }
                } else if (quad.result->type == CONSTSTRING_EXPR) {
                    std::cout << quad.line << "Error: the symbol" << "\t\t" << quad.result->str_const << " is a string" << std::endl;
                } else if (quad.result->type == CONSTBOOL_EXPR) {
                    std::cout << quad.line << "\t" << "IF GREATER" << "\t\t" << quad.result->bool_const << "\t\t" << quad.arg1->bool_const << "\t\t" << quad.label << std::endl;
                } else {
                    if (quad.arg1->type == CONSTNUM_EXPR) { 
                        std::cout << quad.line << "\t" << "IF GREATER" << "\t\t" << quad.result->symbol->name << "\t\t" << quad.arg1->num_const << "\t\t" << quad.label << std::endl; 
                    } else if (quad.arg1->type == CONSTSTRING_EXPR) {
                        std::cout << quad.line << "Error: the symbol" << "\t\t" << quad.arg1->str_const << " is a string" << std::endl;
                    } else if (quad.arg1->type == CONSTBOOL_EXPR) {
                        std::cout << quad.line << "\t" << "IF GREATER" << "\t\t" << quad.result->symbol->name << "\t\t" << quad.arg1->bool_const << "\t\t" << quad.label << std::endl;
                    } else {
                        std::cout << quad.line << "\t" << "IF GREATER" << "\t\t" << quad.result->symbol->name << "\t\t" << quad.arg1->symbol->name << "\t\t" << quad.label << std::endl;
                    }
                }
                break;
            case 16:
                std::cout << quad.line << "\t\t" << "CALL" << "\t\t" << quad.result->symbol->name << std::endl;
                break;
            case 17:
                if (quad.result->type == CONSTNUM_EXPR) {
                    std::cout << quad.line << "\t\t" << "PARAM" << "\t\t" << quad.result->num_const << std::endl;
                } else if (quad.result->type == CONSTSTRING_EXPR) {
                    std::cout << quad.line << "\t\t" << "PARAM" << "\t\t" << quad.result->str_const << std::endl;
                    
                } else if (quad.result->type == CONSTBOOL_EXPR) {
                    std::cout << quad.line << "\t\t" << "PARAM" << "\t\t" << quad.result->bool_const << std::endl;
                } else {
                    std::cout << quad.line << "\t\t" << "PARAM" << "\t\t" << quad.result->symbol->name << std::endl;
                }
                break;
            case 18:
                if (quad.result != NULL) {
                    if (quad.result->type == CONSTNUM_EXPR) {
                        std::cout << quad.line << "\t\t" << "RETURN" << "\t\t" << quad.result->num_const << std::endl;
                    } else if (quad.result->type == CONSTBOOL_EXPR) {
                        std::cout << quad.line << "\t\t" << "RETURN" << "\t\t" << quad.result->bool_const << std::endl;
                    } else if (quad.result->type == CONSTSTRING_EXPR) {
                        std::cout << quad.line << "\t\t" << "RETURN" << "\t\t" << quad.result->str_const << std::endl;
                    } else {                    while (4);
                        std::cout << quad.line << "\t\t" << "RETURN" << "\t\t" << quad.result->symbol->name << std::endl;
                    }
                } 
                break;
            case 19:
                std::cout << quad.line << "\t\t" << "GETRETVAL" << "\t\t" << quad.result->symbol->name << std::endl;
                break;
            case 20:
                std::cout << quad.line << "\t\t\t\t" << "FUNCSTART " << "\t\t" << quad.result->symbol->name << std::endl;
                break;
            case 21:
                std::cout << quad.line << "\t\t\t\t" << "FUNCEND   " << "\t\t" << quad.result->symbol->name << std::endl;
                break;
            case 22:
                std::cout << quad.line << "\t\t" << "TABLECREATE  " << "\t\t" << quad.result->symbol->name << std::endl;
                break;
            case 23:
                if (quad.arg2->type == CONSTNUM_EXPR) {
                    if (quad.arg1->type == CONSTNUM_EXPR) {
                        std::cout << quad.line << "\t" << "TABLEGETELEM" << "\t\t" << quad.result->symbol->name << "\t\t" << quad.arg1->num_const << "\t\t" << quad.arg2->num_const << std::endl;
                    } else if (quad.arg1->type == CONSTSTRING_EXPR) {
                        std::cout << quad.line << "\t" << "TABLEGETELEM" << "\t\t" << quad.result->symbol->name << "\t\t" << quad.arg1->str_const << "\t\t" << quad.arg2->num_const << std::endl;
                    } else if (quad.arg1->type == CONSTBOOL_EXPR) {
                        std::cout << quad.line << "\t" << "TABLEGETELEM" << "\t\t" << quad.result->symbol->name << "\t\t" << quad.arg1->bool_const << "\t\t" << quad.arg2->num_const << std::endl;
                    } else {
                        std::cout << quad.line << "\t" << "TABLEGETELEM" << "\t\t" << quad.result->symbol->name << "\t\t" << quad.arg1->symbol->name << "\t\t" << quad.arg2->num_const << std::endl;
                    }
                } else if (quad.arg2->type == CONSTSTRING_EXPR) {
                    if (quad.arg1->type == CONSTNUM_EXPR) {
                        std::cout << quad.line << "\t" << "TABLEGETELEM" << "\t\t" << quad.result->symbol->name << "\t\t" << quad.arg1->num_const << "\t\t" << quad.arg2->str_const << std::endl;
                    } else if (quad.arg1->type == CONSTSTRING_EXPR) {
                        std::cout << quad.line << "\t" << "TABLEGETELEM" << "\t\t" << quad.result->symbol->name << "\t\t" << quad.arg1->str_const << "\t\t" << quad.arg2->str_const << std::endl;
                    } else if (quad.arg1->type == CONSTBOOL_EXPR) {
                        std::cout << quad.line << "\t" << "TABLEGETELEM" << "\t\t" << quad.result->symbol->name << "\t\t" << quad.arg1->bool_const << "\t\t" << quad.arg2->str_const << std::endl;
                    } else {
                        std::cout << quad.line << "\t" << "TABLEGETELEM" << "\t\t" << quad.result->symbol->name << "\t\t" << quad.arg1->symbol->name << "\t\t" << quad.arg2->str_const << std::endl;
                    }
                } else if (quad.arg2->type == CONSTBOOL_EXPR) {
                    if (quad.arg1->type == CONSTNUM_EXPR) {
                        std::cout << quad.line << "\t" << "TABLEGETELEM" << "\t\t" << quad.result->symbol->name << "\t\t" << quad.arg1->num_const << "\t\t" << quad.arg2->bool_const << std::endl;
                    } else if (quad.arg1->type == CONSTSTRING_EXPR) {
                        std::cout << quad.line << "\t" << "TABLEGETELEM" << "\t\t" << quad.result->symbol->name << "\t\t" << quad.arg1->str_const << "\t\t" << quad.arg2->bool_const << std::endl;
                    } else if (quad.arg1->type == CONSTBOOL_EXPR) {
                        std::cout << quad.line << "\t" << "TABLEGETELEM" << "\t\t" << quad.result->symbol->name << "\t\t" << quad.arg1->bool_const << "\t\t" << quad.arg2->bool_const << std::endl;
                    } else {
                        std::cout << quad.line << "\t" << "TABLEGETELEM" << "\t\t" << quad.result->symbol->name << "\t\t" << quad.arg1->symbol->name << "\t\t" << quad.arg2->bool_const << std::endl;
                    }
                } else {
                    if (quad.arg1->type == CONSTNUM_EXPR) {
                        std::cout << quad.line << "\t" << "TABLEGETELEM" << "\t\t" << quad.result->symbol->name << "\t\t" << quad.arg1->num_const << "\t\t" << quad.arg2->symbol->name << std::endl;
                    } else if (quad.arg1->type == CONSTSTRING_EXPR) {
                        std::cout << quad.line << "\t" << "TABLEGETELEM" << "\t\t" << quad.result->symbol->name << "\t\t" << quad.arg1->str_const << "\t\t" << quad.arg2->symbol->name << std::endl;
                    } else if (quad.arg1->type == CONSTBOOL_EXPR) {
                        std::cout << quad.line << "\t" << "TABLEGETELEM" << "\t\t" << quad.result->symbol->name << "\t\t" << quad.arg1->bool_const << "\t\t" << quad.arg2->symbol->name << std::endl;
                    } else {
                        std::cout << quad.line << "\t" << "TABLEGETELEM" << "\t\t" << quad.result->symbol->name << "\t\t" << quad.arg1->symbol->name << "\t\t" << quad.arg2->symbol->name << std::endl;
                    }
                } 
                break;
            case 24:
                if (quad.arg2->type == CONSTNUM_EXPR) {
                    if (quad.arg1->type == CONSTNUM_EXPR) {
                        std::cout << quad.line << "\t" << "TABLESETELEM" << "\t\t" << quad.result->symbol->name << "\t\t" << quad.arg1->num_const << "\t\t" << quad.arg2->num_const << std::endl;
                    } else if (quad.arg1->type == CONSTSTRING_EXPR) {
                        std::cout << quad.line << "\t" << "TABLESETELEM" << "\t\t" << quad.result->symbol->name << "\t\t" << quad.arg1->str_const << "\t\t" << quad.arg2->num_const << std::endl;
                    } else if (quad.arg1->type == CONSTBOOL_EXPR) {
                        std::cout << quad.line << "\t" << "TABLESETELEM" << "\t\t" << quad.result->symbol->name << "\t\t" << quad.arg1->bool_const << "\t\t" << quad.arg2->num_const << std::endl;
                    } else {
                        std::cout << quad.line << "\t" << "TABLESETELEM" << "\t\t" << quad.result->symbol->name << "\t\t" << quad.arg1->symbol->name << "\t\t" << quad.arg2->num_const << std::endl;
                    }
                } else if (quad.arg2->type == CONSTSTRING_EXPR) {
                    if (quad.arg1->type == CONSTNUM_EXPR) {
                        std::cout << quad.line << "\t" << "TABLESETELEM" << "\t\t" << quad.result->symbol->name << "\t\t" << quad.arg1->num_const << "\t\t" << quad.arg2->str_const << std::endl;
                    } else if (quad.arg1->type == CONSTSTRING_EXPR) {
                        std::cout << quad.line << "\t" << "TABLESETELEM" << "\t\t" << quad.result->symbol->name << "\t\t" << quad.arg1->str_const << "\t\t" << quad.arg2->str_const << std::endl;
                    } else if (quad.arg1->type == CONSTBOOL_EXPR) {
                        std::cout << quad.line << "\t" << "TABLESETELEM" << "\t\t" << quad.result->symbol->name << "\t\t" << quad.arg1->bool_const << "\t\t" << quad.arg2->str_const << std::endl;
                    } else {
                        std::cout << quad.line << "\t" << "TABLESETELEM" << "\t\t" << quad.result->symbol->name << "\t\t" << quad.arg1->symbol->name << "\t\t" << quad.arg2->str_const << std::endl;
                    }
                } else if (quad.arg2->type == CONSTBOOL_EXPR) {
                    if (quad.arg1->type == CONSTNUM_EXPR) {
                        std::cout << quad.line << "\t" << "TABLESETELEM" << "\t\t" << quad.result->symbol->name << "\t\t" << quad.arg1->num_const << "\t\t" << quad.arg2->bool_const << std::endl;
                    } else if (quad.arg1->type == CONSTSTRING_EXPR) {
                        std::cout << quad.line << "\t" << "TABLESETELEM" << "\t\t" << quad.result->symbol->name << "\t\t" << quad.arg1->str_const << "\t\t" << quad.arg2->bool_const << std::endl;
                    } else if (quad.arg1->type == CONSTBOOL_EXPR) {
                        std::cout << quad.line << "\t" << "TABLESETELEM" << "\t\t" << quad.result->symbol->name << "\t\t" << quad.arg1->bool_const << "\t\t" << quad.arg2->bool_const << std::endl;
                    } else {
                        std::cout << quad.line << "\t" << "TABLESETELEM" << "\t\t" << quad.result->symbol->name << "\t\t" << quad.arg1->symbol->name << "\t\t" << quad.arg2->bool_const << std::endl;
                    }
                } else {
                    if (quad.arg1->type == CONSTNUM_EXPR) {
                        std::cout << quad.line << "\t" << "TABLESETELEM" << "\t\t" << quad.result->symbol->name << "\t\t" << quad.arg1->num_const << "\t\t" << quad.arg2->symbol->name << std::endl;
                    } else if (quad.arg1->type == CONSTSTRING_EXPR) {
                        std::cout << quad.line << "\t" << "TABLESETELEM" << "\t\t" << quad.result->symbol->name << "\t\t" << quad.arg1->str_const << "\t\t" << quad.arg2->symbol->name << std::endl;
                    } else if (quad.arg1->type == CONSTBOOL_EXPR) {
                        std::cout << quad.line << "\t" << "TABLESETELEM" << "\t\t" << quad.result->symbol->name << "\t\t" << quad.arg1->bool_const << "\t\t" << quad.arg2->symbol->name << std::endl;
                    } else {
                        std::cout << quad.line << "\t" << "TABLESETELEM" << "\t\t" << quad.result->symbol->name << "\t\t" << quad.arg1->symbol->name << "\t\t" << quad.arg2->symbol->name << std::endl;
                    }
                }
                break;
            case 25:
                std::cout << quad.line << "\t\t" << "JUMP  " << "\t\t" << quad.label << std::endl;
                break;
            default:
                break;
        }
        
    }
    return;
}

std::string new_temp_name(int temp_counter) {
    //temp_counter++;
    return ("__t"+std::to_string(temp_counter));
}

int reset_temp_counter(int temp_counter) {
    temp_counter = 0;
    return temp_counter;
}

std::vector<int> function_local_offset_table;
std::vector<int> formal_arg_offset_table;

void push_func_local_offset_table() {
    function_local_offset_table.push_back(function_local_offset);
    return;
}

void pop_func_local_offset_table() {
    function_local_offset_table.pop_back();
    return;
}

unsigned get_func_local_offset_table() {
    return function_local_offset_table.back();
}

void set_func_local_offset_table() {
    function_local_offset_table.back() = function_local_offset;
    return;
}

void push_formal_arg_offset_table() {
    formal_arg_offset_table.push_back(formal_arg_offset);
    return;
}

void pop_formal_arg_offset_table() {
    formal_arg_offset_table.pop_back();
    return;
}

unsigned get_formal_arg_offset_table() {
    return formal_arg_offset_table.back();
}

void set_formal_arg_offset_table() {
    formal_arg_offset_table.back() = formal_arg_offset;
    return;
}

void set_scope_space_counter(int type) {
    scope_space_counter = type;
    return;
}

int get_scope_space_counter() {
    return scope_space_counter;
}

void set_prev_scope_space_counter(int type) {
    prev_scope_space_counter = type;
    return;
}

int get_prev_scope_space_counter() {
    return prev_scope_space_counter;
}

scope_space curr_scope_space() {
    switch (scope_space_counter) {
        case 1: return program_var;
        case 2: return function_local;
        case 3: return formal_arg;
        default: assert(0); 
    } 
}

unsigned curr_scope_offset() {
    switch (curr_scope_space()) {
        case program_var: return program_var_offset;
        case function_local: return get_func_local_offset_table();
        case formal_arg: return formal_arg_offset;
        default: assert(0); 
    }
}

void inc_curr_scope_offset() {
    switch (curr_scope_space()) {
        case program_var: ++program_var_offset; break;
        case function_local: ++function_local_offset; set_func_local_offset_table(); break;
        case formal_arg: ++formal_arg_offset; set_formal_arg_offset_table(); break;
        default: assert(0); 
    }
    return;
}

void enter_scope_space() {
    ++scope_space_counter;
    return;
}

void exit_scope_space() {
    assert(scope_space_counter>1); 
    --scope_space_counter;
    return;
}

void reset_scope_space() {
    scope_space_counter = 1;
    return;
}

void reset_func_local_offset() {
    function_local_offset = 0;
    return;
}

void reset_formal_arg_offset() {
    formal_arg_offset = 0;
    return;
}

bool check_if_is_arithm(expression* expr) {
    if (expr->type == CONSTSTRING_EXPR || expr->type == NIL_EXPR || expr->type == CONSTBOOL_EXPR || 
        expr->type == NEWTABLE_EXPR || expr->type == PROGRAMFUNC_EXPR || expr->type == LIBFUNC_EXPR || expr->type == BOOL_EXPR) {
        return false;
    }
    return true;
}

bool check_if_is_arithm_alt(expression* expr) {
    if (expr->type == NIL_EXPR) {
        return false;
    }
    return true;
}

void set_curr_quad_label(unsigned step) {
    curr_quad_label = step;
}

unsigned next_quad_label() {
    return quad_table.size(); 
}

expression* manage_expr_plus_expr(expression* result, expression* expr1, expression* expr2) {
	if (check_if_is_arithm(expr1) == true && check_if_is_arithm(expr2) == true) {
        SymbolType type = GLOBAL;
        if (get_scope() != 0) {
            type = LOCAL;
        }
        SymbolTableEntry* new_temp = new_temp_var(-1, type);
        //std::cout << new_temp->scope << std::endl;
        if (expr1->type == 8 && expr2->type == 8) {
            result = make_constnum_expression(CONSTNUM_EXPR, expr1->num_const+expr2->num_const);
            //std::cout << "$1->type " << expr1->type << std::endl;
            emit_add(ADD, result, expr1, expr2);
        } else {  
            result = make_arithm_expression(ARITHM_EXPR, get_symbol(new_temp->name, get_scope()));
            //std::cout << "$1->type " << expr1->type << std::endl;
            emit_add(ADD, result, expr1, expr2);
        }
   } else {
       std::cout << "We cannot do arithmetic actions to a no numerable expression" << std::endl;
   }
   return result;
}

expression* manage_expr_minus_expr(expression* result, expression* expr1, expression* expr2) {
	if (check_if_is_arithm(expr1) == true && check_if_is_arithm(expr2) == true) {
        SymbolType type = GLOBAL;
        if (get_scope() != 0) {
            type = LOCAL;
        }
        SymbolTableEntry* new_temp = new_temp_var(-1, type);
        if (expr1->type == 8 && expr2->type == 8) {
            result = make_constnum_expression(CONSTNUM_EXPR, expr1->num_const-expr2->num_const);
            //std::cout << "$1->type " << expr1->type << std::endl;
            emit_sub(SUB, result, expr1, expr2);
        } else {  
            result = make_arithm_expression(ARITHM_EXPR, get_symbol(new_temp->name, get_scope()));
            //std::cout << "$1->type " << expr1->type << std::endl;
            emit_sub(SUB, result, expr1, expr2);
        }
    } else {
       std::cout << "We cannot do arithmetic actions to a no numerable expression" << std::endl;
    }
    return result;
}

expression* manage_expr_mul_expr(expression* result, expression* expr1, expression* expr2) {
	if (check_if_is_arithm(expr1) == true && check_if_is_arithm(expr2) == true) {
        SymbolType type = GLOBAL;
        if (get_scope() != 0) {
            type = LOCAL;
        }
        SymbolTableEntry* new_temp = new_temp_var(-1, type);
        if (expr1->type == 8 && expr2->type == 8) {
            result = make_constnum_expression(CONSTNUM_EXPR, expr1->num_const*expr2->num_const);
            //std::cout << "$1->type " << expr1->type << std::endl;
            emit_mul(MUL, result, expr1, expr2);
        } else {  
            result = make_arithm_expression(ARITHM_EXPR, get_symbol(new_temp->name, get_scope()));
            //std::cout << "$1->type " << expr1->type << std::endl;
            emit_mul(MUL, result, expr1, expr2);
        }
                    
    } else {
        std::cout << "We cannot do arithmetic actions to a no numerable expression" << std::endl;
    }
    return result;
}

expression* manage_expr_div_expr(expression* result, expression* expr1, expression* expr2) {
	if (check_if_is_arithm(expr1) == true && check_if_is_arithm(expr2) == true) {
        SymbolType type = GLOBAL;
        if (get_scope() != 0) {
            type = LOCAL;
        }
        SymbolTableEntry* new_temp = new_temp_var(-1, type);
        if (expr1->type == 8 && expr2->type == 8) {
            result = make_constnum_expression(CONSTNUM_EXPR, expr1->num_const/expr2->num_const);
            //std::cout << "$1->type " << expr1->type << std::endl;
            emit_div(DIV, result, expr1, expr2);
        } else {  
            result = make_arithm_expression(ARITHM_EXPR, get_symbol(new_temp->name, get_scope()));
            //std::cout << "$1->type " << expr1->type << std::endl;
            emit_div(DIV, result, expr1, expr2);
        }
    } else {
       std::cout << "We cannot do arithmetic actions to a no numerable expression" << std::endl;
    }
    return result;
}

expression* manage_expr_mod_expr(expression* result, expression* expr1, expression* expr2) {
	if (check_if_is_arithm(expr1) == true && check_if_is_arithm(expr2) == true) {
        SymbolType type = GLOBAL;
        if (get_scope() != 0) {
            type = LOCAL;
        }
        SymbolTableEntry* new_temp = new_temp_var(-1, type);
        if (expr1->type == 8 && expr2->type == 8) {
            result = make_constnum_expression(CONSTNUM_EXPR, (int)expr1->num_const%(int)expr2->num_const);
            //std::cout << "$1->type " << expr1->type << std::endl;
            emit_mod(MOD, result, expr1, expr2);
        } else {  
            result = make_arithm_expression(ARITHM_EXPR, get_symbol(new_temp->name, get_scope()));
            //std::cout << "$1->type " << expr1->type << std::endl;
            emit_mod(MOD, result, expr1, expr2);
        }
    } else {
       std::cout << "We cannot do arithmetic actions to a no numerable expression" << std::endl;
    }
    return result;
}

expression* manage_expr_gr_expr(expression* result, expression* expr1, expression* expr2) {
	std::cout << "Expr1 type is: " << expr1->type << std::endl;
    if (check_if_is_arithm(expr1) == true && check_if_is_arithm(expr2) == true) {
        SymbolType type = GLOBAL;
        if (get_scope() != 0) {
            type = LOCAL;
        }
        SymbolTableEntry* new_temp = new_temp_var(-1, type);
        //std::cout << new_temp.scope << std::endl;
        result = make_bool_expression(BOOL_EXPR, get_symbol(new_temp->name, get_scope()));
        result->truelist.push_front(next_quad_label());
        result->falselist.push_front(next_quad_label() + 1);
        //emit_if_greater(IF_GR, expr1, expr2, next_quad_label()+3);
        //set_curr_quad_label((next_quad_label()+3));
        emit_if_greater(IF_GR, expr1, expr2, UINT_MAX);
        //emit_assign(ASSIGN, result, make_constbool_expression(CONSTBOOL_EXPR, 0));
        //emit_jump(JUMP, (next_quad_label()+2));
        //set_curr_quad_label((next_quad_label()+2));
        emit_jump(JUMP, UINT_MAX);
        //emit_assign(ASSIGN, result, make_constbool_expression(CONSTBOOL_EXPR, 1));
    } else {
       std::cout << "We cannot do arithmetic actions to a no numerable expression" << std::endl;
       exit(-1);
    }
    

    return result;
}

expression* manage_expr_ls_expr(expression* result, expression* expr1, expression* expr2) {
	if (check_if_is_arithm(expr1) == true && check_if_is_arithm(expr2) == true) {
        SymbolType type = GLOBAL;
        if (get_scope() != 0) {
            type = LOCAL;
        }
        SymbolTableEntry* new_temp = new_temp_var(-1, type);
        //std::cout << new_temp.scope << std::endl;
        result = make_bool_expression(BOOL_EXPR, get_symbol(new_temp->name, get_scope()));
        result->truelist.push_front(next_quad_label());
        result->falselist.push_front(next_quad_label() + 1);
        //emit_if_less(IF_L, expr1, expr2, next_quad_label()+3);
        emit_if_less(IF_L, expr1, expr2, UINT_MAX);
        //set_curr_quad_label((next_quad_label()+3));
        //emit_assign(ASSIGN, result, make_constbool_expression(CONSTBOOL_EXPR, 0));
        emit_jump(JUMP, UINT_MAX);
        //set_curr_quad_label((next_quad_label()+2));
        //emit_assign(ASSIGN, result, make_constbool_expression(CONSTBOOL_EXPR, 1));
    } else {
        std::cout << "We cannot do arithmetic actions to a no numerable expression" << std::endl;
        exit(-1);
    }
    return result;
}

expression* manage_expr_gr_eq_expr(expression* result, expression* expr1, expression* expr2) {
	if (check_if_is_arithm(expr1) == true && check_if_is_arithm(expr2) == true) {
        SymbolType type = GLOBAL;
        if (get_scope() != 0) {
            type = LOCAL;
        }
        SymbolTableEntry* new_temp = new_temp_var(-1, type);
        //std::cout << new_temp.scope << std::endl;
        result = make_bool_expression(BOOL_EXPR, get_symbol(new_temp->name, get_scope()));
        result->truelist.push_front(next_quad_label());
        result->falselist.push_front(next_quad_label() + 1);
        //emit_if_greatereq(IF_GR_EQ, expr1, expr2, next_quad_label()+3);
        emit_if_greatereq(IF_GR_EQ, expr1, expr2, UINT_MAX);
        //set_curr_quad_label((next_quad_label()+3));
        //emit_assign(ASSIGN, result, make_constbool_expression(CONSTBOOL_EXPR, 0));
        emit_jump(JUMP, UINT_MAX);
        //set_curr_quad_label((next_quad_label()+2));
        //emit_assign(ASSIGN, result, make_constbool_expression(CONSTBOOL_EXPR, 1));
    } else {
        std::cout << "We cannot do arithmetic actions to a no numerable expression" << std::endl;
        exit(-1);
    }
    return result;
}

expression* manage_expr_ls_eq_expr(expression* result, expression* expr1, expression* expr2) {
	if (check_if_is_arithm(expr1) == true && check_if_is_arithm(expr2) == true) {
        SymbolType type = GLOBAL;
        if (get_scope() != 0) {
            type = LOCAL;
        }
        SymbolTableEntry* new_temp = new_temp_var(-1, type);
        //std::cout << new_temp.scope << std::endl;
        result = make_bool_expression(BOOL_EXPR, get_symbol(new_temp->name, get_scope()));
        result->truelist.push_front(next_quad_label());
        result->falselist.push_front(next_quad_label() + 1);
        //emit_if_lesseq(IF_L_EQ, expr1, expr2, next_quad_label()+3);
        emit_if_lesseq(IF_L_EQ, expr1, expr2, UINT_MAX);
        //set_curr_quad_label((next_quad_label()+3));
        //emit_assign(ASSIGN, result, make_constbool_expression(CONSTBOOL_EXPR, 0));
        emit_jump(JUMP, UINT_MAX);
        //set_curr_quad_label((next_quad_label()+2));
        //emit_assign(ASSIGN, result, make_constbool_expression(CONSTBOOL_EXPR, 1));
    } else {
        std::cout << "We cannot do arithmetic actions to a no numerable expression" << std::endl;
        exit(-1);
    }
    return result;
}

expression* manage_expr_eq_expr(expression* result, expression* expr1, expression* expr2) {
	if (check_if_is_arithm(expr1) == true && check_if_is_arithm(expr2) == true) {
        SymbolType type = GLOBAL;
        if (get_scope() != 0) {
            type = LOCAL;
        }
        SymbolTableEntry* new_temp = new_temp_var(-1, type);
        //std::cout << new_temp.scope << std::endl;
        result = make_bool_expression(BOOL_EXPR, get_symbol(new_temp->name, get_scope()));
        result->truelist.push_front(next_quad_label());
        result->falselist.push_front(next_quad_label() + 1);
        //emit_if_equal(IF_EQ, expr1, expr2, next_quad_label()+3);
        emit_if_equal(IF_EQ, expr1, expr2, UINT_MAX);
        //set_curr_quad_label((next_quad_label()+3));
        //emit_assign(ASSIGN, result, make_constbool_expression(CONSTBOOL_EXPR, 0));
        emit_jump(JUMP, UINT_MAX);
        //set_curr_quad_label((next_quad_label()+2));
        //emit_assign(ASSIGN, result, make_constbool_expression(CONSTBOOL_EXPR, 1));
    } else {
        std::cout << "We cannot do arithmetic actions to a no numerable expression" << std::endl;
        exit(-1);
    }
    return result;
}

expression* manage_expr_not_eq_expr(expression* result, expression* expr1, expression* expr2) {
	if (check_if_is_arithm(expr1) == true && check_if_is_arithm(expr2) == true) {
        SymbolType type = GLOBAL;
        if (get_scope() != 0) {
            type = LOCAL;
        }
        SymbolTableEntry* new_temp = new_temp_var(-1, type);
        //std::cout << new_temp.scope << std::endl;
        result = make_bool_expression(BOOL_EXPR, get_symbol(new_temp->name, get_scope()));
        result->truelist.push_front(next_quad_label());
        result->falselist.push_front(next_quad_label() + 1);
        //emit_if_not_equal(IF_N_EQ, expr1, expr2, next_quad_label()+3);
        emit_if_not_equal(IF_N_EQ, expr1, expr2, UINT_MAX);
        //set_curr_quad_label((next_quad_label()+3));
        //emit_assign(ASSIGN, result, make_constbool_expression(CONSTBOOL_EXPR, 0));
        emit_jump(JUMP, UINT_MAX);
        //set_curr_quad_label((next_quad_label()+2));
        //emit_assign(ASSIGN, result, make_constbool_expression(CONSTBOOL_EXPR, 1));
    } else {
        std::cout << "We cannot do arithmetic actions to a no numerable expression" << std::endl;
        exit(-1);
    }
    return result;
}

expression* manage_expr_and_expr(expression* result, expression* expr1, unsigned m_quad, expression* expr2) {
    SymbolType type = GLOBAL;
    if (get_scope() != 0) {
        type = LOCAL;
    }
    SymbolTableEntry* new_temp = new_temp_var(-1, type);
    //std::cout << new_temp.scope << std::endl;
    result = make_bool_expression(BOOL_EXPR, get_symbol(new_temp->name, get_scope()));
    
    
    for (auto& it : expr1->truelist) {
        std::cout << "True list: " << it << std::endl;
        backpatch(it, m_quad);
    }
    
    result->truelist = expr2->truelist;
    expr1->falselist.merge(expr2->falselist);
    result->falselist = expr1->falselist;
   
    
    //emit_bool(AND, result, expr1, expr2);
    return result;
}

expression* manage_expr_or_expr(expression* result, expression* expr1, unsigned m_quad, expression* expr2) {
    
    SymbolType type = GLOBAL;
    if (get_scope() != 0) {
        type = LOCAL;
    }
    SymbolTableEntry* new_temp = new_temp_var(-1, type);
    //std::cout << new_temp.scope << std::endl;
    result = make_bool_expression(BOOL_EXPR, get_symbol(new_temp->name, get_scope()));
    //std::cout << "M Quad is " << m_quad << std::endl;
    for (auto& it : expr1->falselist) {
        std::cout << "False list: " << it << std::endl;
        backpatch(it, m_quad);
    }
    expr1->truelist.merge(expr2->truelist);
    
    result->truelist = expr1->truelist;
    /*for (auto& it : expr2->falselist) {
        std::cout << "False list: " << it << std::endl;
        backpatch(it, m_quad+2);
    }*/
    result->falselist = expr2->falselist;
    //emit_bool(OR, result, expr1, expr2);
    return result;
}

expression* manage_uminus_expr(expression* result, expression* expr1) {
    //std::cout << "term -> - expression" << std::endl;
    if (check_if_is_arithm(expr1)) {
        //std::cout << "term -> - expression in check_if_is_arithm" << std::endl;

        SymbolType type = GLOBAL;
        if (get_scope() != 0) {
            type = LOCAL;
        }
        SymbolTableEntry* new_temp = new_temp_var(-1, type);
        std::cout << new_temp->scope << std::endl;
        result = make_arithm_expression(ARITHM_EXPR, get_symbol(new_temp->name, get_scope()));
        //std::cout << "term -> - expression after make_arithm_expression" << std::endl;

        emit_arithm_one(UMINUS, result, expr1);
    }
    return result;
}

expression* manage_not_expr(expression* result, expression* expr1) {
    if (expr1->truelist.empty()) {
        expr1->truelist.push_back(next_quad_label());
        expr1->falselist.push_back(next_quad_label()+1);
        emit_if_equal(IF_EQ, expr1, make_constbool_expression(CONSTBOOL_EXPR, 1), UINT_MAX);
        emit_jump(JUMP, UINT_MAX);
    }
    SymbolType type = GLOBAL;
    if (get_scope() != 0) {
        type = LOCAL;
    }
    SymbolTableEntry* new_temp = new_temp_var(-1, type);
    std::cout << new_temp->scope << std::endl;
    result = make_bool_expression(BOOL_EXPR, get_symbol(new_temp->name, get_scope()));
    result->truelist = expr1->falselist;
    result->falselist = expr1->truelist;
    //emit_bool(NOT, result, expr1, NULL);
    return result;
}

expression* manage_db_plus_lvalue(expression* result, expression* expr1) {
    
    if (handle_expr(expr1->symbol->name, expr1->type) == 0) {
        if (check_if_is_arithm(expr1)) {
            //std::cout << "--L-VALUE" << std::endl;
            if (expr1->type == TABLEITEM_EXPR) {
                result = emit_if_table_item(expr1);
                emit_add(ADD, result, result, make_constnum_expression(CONSTNUM_EXPR, 1));
                emit_table_set_item(TABLESETELEM, expr1, expr1->index, result);
            } else {
                //std::cout << "EMIT ADD 1" << std::endl;
                emit_add(ADD, expr1, expr1, make_constnum_expression(CONSTNUM_EXPR, 1));
                SymbolType type = GLOBAL;
                    if (get_scope() != 0) {
                        type = LOCAL;
                    }
                SymbolTableEntry* new_temp = new_temp_var(-1, type);
                //std::cout << new_temp->scope << std::endl;
                result = make_arithm_expression(ARITHM_EXPR, get_symbol(new_temp->name, get_scope()));
                emit_assign(ASSIGN, result, expr1);
                
            }
        } else {
            std::cout <<"Error r-value: " << expr1->symbol->name << "is not arithmetic value" << std::endl;
            return NULL;
        }
    }

    return result;
}

expression* manage_lvalue_db_plus(expression* result, expression* expr1) {
    
    if (handle_expr(expr1->symbol->name, expr1->type) == 0) {
        if (check_if_is_arithm(expr1)) {
            SymbolType type = GLOBAL;
            if (get_scope() != 0) {
                type = LOCAL;
            }
            SymbolTableEntry* new_temp = new_temp_var(-1, type);
            //std::cout << new_temp->scope << std::endl;
            result = make_lvalue_expression(VAR_EXPR, get_symbol(new_temp->name, get_scope()));
            if (expr1->type == TABLEITEM_EXPR) {
                expression* val = emit_if_table_item(expr1);
                emit_assign(ASSIGN, result, val);
                emit_add(ADD, val, val, make_constnum_expression(CONSTNUM_EXPR, 1));
                emit_table_set_item(TABLESETELEM, expr1, expr1->index, val);
            } else {
                //std::cout << "EMIT ASSIGN 1" << std::endl;
                emit_assign(ASSIGN, result, expr1);
                emit_add(ADD, expr1, expr1, make_constnum_expression(CONSTNUM_EXPR, 1));
            }
        } else {
            std::cout <<"Error r-value: " << expr1->symbol->name << "is not arithmetic value" << std::endl;
            return NULL;
        }
    }
    return result;
}

expression* manage_db_minus_lvalue(expression* result, expression* expr1) {

    if (handle_expr(expr1->symbol->name, expr1->type) == 0) {
        if (check_if_is_arithm(expr1)) {
            //std::cout << "--L-VALUE" << std::endl;
            if (expr1->type == TABLEITEM_EXPR) {
                result = emit_if_table_item(expr1);
                emit_sub(SUB, result, result, make_constnum_expression(CONSTNUM_EXPR, 1));
                emit_table_set_item(TABLESETELEM, expr1, expr1->index, result);
            } else {
                //std::cout << "EMIT ADD 1" << std::endl;
                emit_sub(SUB, expr1, expr1, make_constnum_expression(CONSTNUM_EXPR, 1));
                SymbolType type = GLOBAL;
                if (get_scope() != 0) {
                    type = LOCAL;
                }
                SymbolTableEntry* new_temp = new_temp_var(-1, type);
                //td::cout << new_temp->scope << std::endl;
                result = make_arithm_expression(ARITHM_EXPR, get_symbol(new_temp->name, get_scope()));
                emit_assign(ASSIGN, result, expr1);
                
            }
        } else {
            std::cout <<"Error r-value: " << expr1->symbol->name << "is not arithmetic value" << std::endl;
            return NULL;
        }
    }

    return result;
} 

expression* manage_lvalue_db_minus(expression* result, expression* expr1) {
     
    if (handle_expr(expr1->symbol->name, expr1->type) == 0) {
        if (check_if_is_arithm(expr1)) {
            SymbolType type = GLOBAL;
            if (get_scope() != 0) {
                type = LOCAL;
            }
            SymbolTableEntry* new_temp = new_temp_var(-1, type);
            //std::cout << new_temp->scope << std::endl;
            result = make_lvalue_expression(VAR_EXPR, get_symbol(new_temp->name, get_scope()));
            if (expr1->type == TABLEITEM_EXPR) {
                expression* val = emit_if_table_item(expr1);
                emit_assign(ASSIGN, result, val);
                emit_sub(SUB, val, val, make_constnum_expression(CONSTNUM_EXPR, 1));
                emit_table_set_item(TABLESETELEM, expr1, expr1->index, val);
            } else {
                //std::cout << "EMIT ASSIGN 1" << std::endl;
                emit_assign(ASSIGN, result, expr1);
                emit_sub(SUB, expr1, expr1, make_constnum_expression(CONSTNUM_EXPR, 1));
            }
        } else {
            std::cout <<"Error r-value: " << expr1->symbol->name << "is not arithmetic value" << std::endl;
            return NULL;
        }
    }

    return result;
}

expression* manage_assign_expr(expression* result, expression* expr1, expression* expr2) {
    /*if (expr2->symbol) {
    std::cout<< "LVALUE = EXPR" << expr2->symbol->name << std::endl;
    }*/
    if (expr2->truelist.empty()) {
        if (handle_expr(expr1->symbol->name, expr1->type) == 0) {
        
            if (expr1->type == TABLEITEM_EXPR) {
            
                emit_table_set_item(TABLESETELEM, expr1, expr1->index, expr2);
                result = emit_if_table_item(expr1);
                result->type = ASSIGN_EXPR;
            
            } else {
            
                if (check_if_is_arithm_alt(expr1) == true && check_if_is_arithm_alt(expr2) == true) {
                
                    emit_assign(ASSIGN, expr1, expr2);

                    SymbolType type = GLOBAL;
                    if (get_scope() != 0) {
                        type = LOCAL;
                    }
                    SymbolTableEntry* new_temp = new_temp_var(-1, type);
                    //std::cout << new_temp->name << std::endl;

                    if (get_symbol(new_temp->name, get_scope()) != NULL) {
                        //std::cout << "NEW TEMP NAME: " << new_temp->name << std::endl;
                        result = make_assign_expression(ASSIGN_EXPR, get_symbol(new_temp->name, get_scope()));
                        //std::cout << "ASSIGN RESULT NAME: " << result->symbol->name << std::endl;
                        
                        emit_assign(ASSIGN, result, expr1);
                    } else {
                        //std::cout << new_temp->name << std::endl;
                        result = make_assign_expression(ASSIGN_EXPR, get_symbol_inactive(new_temp->name, get_scope()));
                        
                        emit_assign(ASSIGN, result, expr1);
                    }
                } else {
                    std::cout << "Error r-value is not an arithmetic expression" << expr1->type << " OR " << expr2->type << std::endl;
                }
            }
        }
    } else {
        if (handle_expr(expr1->symbol->name, expr1->type) == 0) {
        
            if (expr1->type == TABLEITEM_EXPR) {
            
                emit_table_set_item(TABLESETELEM, expr1, expr1->index, expr2);
                result = emit_if_table_item(expr1);
                result->type = ASSIGN_EXPR;
            
            } else {
            
                if (check_if_is_arithm_alt(expr1) == true && check_if_is_arithm_alt(expr2) == true) {
                
                    //emit_assign(ASSIGN, expr1, expr2);

                    SymbolType type = GLOBAL;
                    if (get_scope() != 0) {
                        type = LOCAL;
                    }
                    SymbolTableEntry* new_temp = new_temp_var(-1, type);
                    //std::cout << new_temp->name << std::endl;

                    if (get_symbol(new_temp->name, get_scope()) != NULL) {
                        //std::cout << "NEW TEMP NAME: " << new_temp->name << std::endl;
                        result = make_assign_expression(ASSIGN_EXPR, get_symbol(new_temp->name, get_scope()));
                        //std::cout << "ASSIGN RESULT NAME: " << result->symbol->name << std::endl;
                        
                        for (auto& it : expr2->truelist) {
                            std::cout << "TRUE LIST: " << it << std::endl;
                            backpatch(it, next_quad_label());
                        }
                        for (auto& it : expr2->falselist) {
                            std::cout << "FALSE LIST: " << it << std::endl;
                            backpatch(it, next_quad_label()+2);
                        }
                       
                        emit_assign(ASSIGN, result, make_constbool_expression(CONSTBOOL_EXPR, 1));
                        emit_jump(JUMP, (next_quad_label()+2));
                        set_curr_quad_label((next_quad_label()+2));
                        emit_assign(ASSIGN, result, make_constbool_expression(CONSTBOOL_EXPR, 0));
                        emit_assign(ASSIGN, expr1, result);
                    } else {
                        //std::cout << new_temp->name << std::endl;
                        result = make_assign_expression(ASSIGN_EXPR, get_symbol_inactive(new_temp->name, get_scope()));
                    
                        for (auto& it : expr2->truelist) {
                            std::cout << "TRUELIST: " << it << std::endl;
                            backpatch(it, next_quad_label());
                        }
                        for (auto& it : expr2->falselist) {
                            std::cout << "FALSE LIST: " << it << std::endl;
                            backpatch(it, next_quad_label()+2);
                        }
                        //backpatch(expr2->truelist.back(), next_quad_label());
                        emit_assign(ASSIGN, result, make_constbool_expression(CONSTBOOL_EXPR, 1));
                        emit_jump(JUMP, (next_quad_label()+2));
                        set_curr_quad_label((next_quad_label()+2));
                        emit_assign(ASSIGN, result, make_constbool_expression(CONSTBOOL_EXPR, 0));
                        emit_assign(ASSIGN, expr1, result);
                    }
                } else {
                    std::cout << "Error r-value is not an arithmetic expression" << expr1->type << " OR " << expr2->type << std::endl;
                }
            }
        }
    }

    return result;
}

expression* manage_lvalue_id(expression* result, std::string symbol, int line_num) {
    //std::cout << "MANAGE LVALUE ID" << std::endl;
    int symbol_flag = handle_id(symbol, get_scope(), line_num, curr_scope_space(), curr_scope_offset());
    //std::cout << "MANAGE LVALUE ID AFTER HANDLE ID Symbol Flag: " << symbol_flag << std::endl;
    if (symbol_flag == 1) {
        if (get_symbol(symbol, get_scope())->type == LIBFUNC) {
            //std::cout << "MANAGE LVALUE ID LIBFUNC" << std::endl;
            result = make_lvalue_expression(LIBFUNC_EXPR, get_symbol(symbol, get_scope()));

        } else if (get_symbol(symbol, get_scope())->type == USERFUNC) {
            //std::cout << "MANAGE LVALUE ID USERFUNC" << std::endl;
            result = make_lvalue_expression(PROGRAMFUNC_EXPR, get_symbol(symbol, get_scope()));
            //std::cout << "MANAGE LVALUE ID USERFUNC AFTER" << std::endl;
        } else {
            //std::cout << "MANAGE LVALUE ID OTHERWISE" << std::endl;
            /*std::cout << "GET SYMBOL: " << get_symbol(symbol, get_scope())->name << std::endl;
            result = make_lvalue_expression(VAR_EXPR, get_symbol(symbol, get_scope()));*/
            if (get_symbol(symbol, get_scope()) != NULL) {
                //std::cout << "MANAGE LVALUE ID OTHERWISE" << std::endl;
                //std::cout << "GET SYMBOL: " << get_symbol(symbol, get_scope())->name << std::endl;
                //std::cout << "GET SYMBOL SCOPE: " << get_scope() << std::endl;
                result = make_lvalue_expression(VAR_EXPR, get_symbol(symbol, get_scope()));
                //emit_if_equal(IF_EQ, result, make_constbool_expression(CONSTBOOL_EXPR, 1), UINT_MAX);
                //emit_jump(JUMP, UINT_MAX);
                //std::cout << "MANAGE LVALUE ID OTHERWISE AFTER MAKE LVALUE EXPR" << std::endl;

            } else {
                result = make_lvalue_expression(VAR_EXPR, get_symbol_inactive(symbol, get_scope()));
            }
        }
    } else if (symbol_flag == 0) {
        //std::cout << "GET SYMBOL NAME: " << get_symbol(symbol, 1)->name << std::endl;
        for (int i = 0; i <= get_scope(); i++) {
            if (get_symbol(symbol, i) != NULL) {
                //std::cout << "GET SYMBOL TYPE: " << get_symbol(symbol, i)->name << std::endl;
                if (get_symbol(symbol, i)->type == LIBFUNC) {
                    //std::cout << "MANAGE LVALUE ID LIBFUNC" << std::endl;
                    result = make_lvalue_expression(LIBFUNC_EXPR, get_symbol(symbol, i));
                    break;
                } else if (get_symbol(symbol, i)->type == USERFUNC) {
                    //std::cout << "MANAGE LVALUE ID USERFUNC" << std::endl;
                    result = make_lvalue_expression(PROGRAMFUNC_EXPR, get_symbol(symbol, i));
                    //std::cout << "MANAGE LVALUE ID USERFUNC AFTER" << std::endl;
                    break;
                } else {
                    if (get_symbol(symbol, i) != NULL) {
                        //std::cout << "MANAGE LVALUE ID OTHERWISE" << std::endl;
                        //std::cout << "GET SYMBOL: " << get_symbol(symbol, i)->name << std::endl;
                        result = make_lvalue_expression(VAR_EXPR, get_symbol(symbol, i));
                        //emit_if_equal(IF_EQ, result, make_constbool_expression(CONSTBOOL_EXPR, 1), UINT_MAX);
                        //emit_jump(JUMP, UINT_MAX);
                        //std::cout << "MANAGE LVALUE ID OTHERWISE AFTER MAKE LVALUE EXPR" << std::endl;
                        break;
                    } else {
                        result = make_lvalue_expression(VAR_EXPR, get_symbol_inactive(symbol, i));
                        break;
                    }
                }
            }
        }
    } else {
        return NULL;
    }
    return result;
}

expression* manage_lvalue_local_id(expression* result, std::string symbol, int line_num) {
    //std::cout << "Current Scope Space: " << curr_scope_space() << ", Current Scope Offset: " << curr_scope_offset() << std::endl;
    handle_local_id(symbol, get_scope(), line_num, curr_scope_space(), curr_scope_offset());
    if (get_symbol(symbol, get_scope())->type == LIBFUNC) {
        result = make_lvalue_expression(LIBFUNC_EXPR, get_symbol(symbol, get_scope()));
    } else if (get_symbol(symbol, get_scope())->type == USERFUNC) {
        result = make_lvalue_expression(PROGRAMFUNC_EXPR, get_symbol(symbol, get_scope()));
    } else {
        if (get_symbol(symbol, get_scope() != NULL)) {
            result = make_lvalue_expression(VAR_EXPR, get_symbol(symbol, get_scope()));
            //emit_if_equal(IF_EQ, result, make_constbool_expression(CONSTBOOL_EXPR, 1), UINT_MAX);
            //emit_jump(JUMP, UINT_MAX);
        } else {
            result = make_lvalue_expression(VAR_EXPR, get_symbol_inactive(symbol, get_scope()));
        }
    }
    return result;
}

expression* manage_lvalue_global_id(expression* result, std::string symbol, int line_num) {
    handle_global_access_id(symbol);
    result = make_lvalue_expression(VAR_EXPR, get_symbol(symbol,0));
    //emit_if_equal(IF_EQ, result, make_constbool_expression(CONSTBOOL_EXPR, 1), UINT_MAX);
    //emit_jump(JUMP, UINT_MAX);
    return result;                               
}

expression* manage_lvalue_dot_id(expression* result, expression* expr1, std::string symbol) {
    result = member_item(expr1, symbol);
    return result;
}

expression* manage_lvalue_lbr_expr_rbr(expression* result, expression* expr1, expression* expr2) {
    expr1 = emit_if_table_item(expr1);
    if (expr2->type == CONSTNUM_EXPR) {
        result = make_table_item_expression(TABLEITEM_EXPR, expr1->symbol, make_constnum_expression(CONSTNUM_EXPR, expr2->num_const));
    } else if (expr2->type == CONSTSTRING_EXPR) {
        result = make_table_item_expression(TABLEITEM_EXPR, expr1->symbol, make_conststring_expression(CONSTSTRING_EXPR, expr2->str_const));
    }
    return result;
}

expression* manage_lvalue_callsuffix(expression* result, expression* lvalue, std::string id_name, expression* callsuffix, int method) {
    //std::cout << "MANAGE LVALUE CALLSUFFIX ID NAME: " << lvalue->symbol->name << std::endl;
    expression* callsuffix_2;
    lvalue = emit_if_table_item(lvalue);
    std::vector<expression*> callsuffix_stack;
    if (method) {
    
        /*if (lvalue->type == LIBFUNC_EXPR || lvalue->type == PROGRAMFUNC_EXPR || lvalue->type == VAR_EXPR) {
            
        } else {*/
            while (callsuffix) {
                //std::cout << "Callsuffix Elem: " << callsuffix->symbol->name << std::endl;
                callsuffix_stack.push_back(callsuffix);
                callsuffix = callsuffix->next;
            }
            callsuffix_stack.push_back(lvalue);
            /*for (const auto& elem : callsuffix_stack) {
                callsuffix_stack_rev.push_back(elem);
                //std::cout << "Elem: " <<  elem->symbol->name << std::endl;
            }*/
            lvalue = emit_if_table_item(member_item(lvalue, id_name));
            //callsuffix_2 = callsuffix_stack.front();
            //std::cout << "CALLSUFFIX ELEM: " << callsuffix_stack.front()->next->next->symbol->name << std::endl;

            
        }
    //}
    result = make_call_in(lvalue, callsuffix_stack);
    return result;
}

SymbolTableEntry* manage_funcprefix(SymbolTableEntry* symbol, std::string name, int line_num) {
    //std::cout << "Manage_funcprefix_w_name" << std::endl;
    handle_funcdef_w_name(name, get_scope(), line_num, curr_scope_space(), curr_scope_offset());
    symbol = get_symbol(name, get_scope());
    symbol->i_address = next_quad_label();
    emit_jump(JUMP, UINT_MAX);
    emit_funcdef(FUNCSTART, make_func_expression(PROGRAMFUNC_EXPR, get_symbol(symbol->name, get_scope())));
    set_prev_scope_space_counter(get_scope_space_counter());
    set_scope_space_counter(3);
    push_formal_arg_offset_table();
    
    return symbol;
}

SymbolTableEntry* manage_funcprefix_anonym(SymbolTableEntry* symbol, int line_num) {
    std::string name = handle_funcdef_anonym_name(get_scope(), line_num, curr_scope_space(), curr_scope_offset());
    symbol = get_symbol(name, get_scope());
    symbol->i_address = next_quad_label();
    emit_jump(JUMP, UINT_MAX);
    emit_funcdef(FUNCSTART, make_func_expression(PROGRAMFUNC_EXPR, get_symbol(symbol->name, get_scope())));
    set_prev_scope_space_counter(get_scope_space_counter());
    set_scope_space_counter(3);
    push_formal_arg_offset_table();

    return symbol;

}

expression* manage_funcdef(SymbolTableEntry* symbol, unsigned funcbody) {
    //std::cout << "MANAGE FUNCDEF" << std::endl;
    symbol->total_locals = funcbody;
    expression* res = make_func_expression(PROGRAMFUNC_EXPR, symbol);
    //std::cout << "AFTER MAKE FUNC EXPRESSION" << std::endl;
    emit_funcdef(FUNCEND,res);
    //std::cout << "AFTER EMIT FUNCDEF" << std::endl;
    handle_funcdef_block_end(get_scope());
    //std::cout << "AFTER HANDLE FUNCDEF BLOCK END" << std::endl;
    return res;
}

expression* manage_lbr_elist_rbr(expression* objectdef, expression* elist) {
    //std::cout << "OBJECTDEF" << std::endl;
    SymbolType type = GLOBAL;
    if (get_scope() != 0) {
        type = LOCAL;
    }
    SymbolTableEntry* new_temp = new_temp_var(-1, type);
    expression* table = make_new_table_expression(NEWTABLE_EXPR, get_symbol(new_temp->name, get_scope()));
    emit_table_create(TABLECREATE, table);
    std::list<expression*> e_list;
    
    while (elist) {
        e_list.push_front(elist);
        elist = elist->next;
    }
    int i = 0;
    for (auto& e : e_list) {
        //std::cout << "E_LIST " << e->bool_const << std::endl;
        emit_table_set_item(TABLESETELEM, table, make_constnum_expression(CONSTNUM_EXPR, i++), e);
        
    }
    objectdef = table;

    return objectdef;
}

expression* manage_lbr_indexed_rbr(expression* objectdef, indexed_elem* indexed) {
    //printf("objectdef -> [indexed]\n");
    SymbolType type = GLOBAL;
    if (get_scope() != 0) {
        type = LOCAL;
    }
    SymbolTableEntry* new_temp = new_temp_var(-1, type);
    //std::cout << new_temp->name << std::endl;
    expression* table = make_new_table_expression(NEWTABLE_EXPR, get_symbol(new_temp->name, get_scope()));
    emit_table_create(TABLECREATE, table);
    //set_indexed_map(table);
    std::list<indexed_elem*> indexed_list;
    while (indexed) {
        indexed_list.push_front(indexed);
        indexed = indexed->next;
    }
    for (const auto& indexed : indexed_list) {
        //std::cout << "Indexed " << indexed->index->symbol->name << std::endl;
        emit_table_set_item(TABLESETELEM, table, indexed->index, indexed->value);
    }
    objectdef = table;

    return objectdef;
}

unsigned manage_ifprefix(unsigned ifprefix, expression* expr) {
    if (expr->type == CONSTBOOL_EXPR && expr->bool_const == 1) {
        // Condition is always true, no need for conditional jump
        return next_quad_label(); // No quads are emitted
    } else if (expr->type == CONSTBOOL_EXPR && expr->bool_const == 0) {
        ifprefix = next_quad_label();
        emit_jump(JUMP, UINT_MAX); // Jump to the end of the `if` block
        return ifprefix; // Return the label of the `JUMP` quad
    } else {
        emit_if_equal(IF_EQ, expr, make_constbool_expression(CONSTBOOL_EXPR, 1), next_quad_label() + 2);
        ifprefix = next_quad_label();
        emit_jump(JUMP, UINT_MAX); // Emit a JUMP quad with a placeholder label
        return ifprefix;
    }
}

unsigned manage_elseprefix(unsigned elseprefix) {
    elseprefix = next_quad_label();
    emit_jump(JUMP, 0);
    return elseprefix;
}

std::vector<break_stmt> breaklist;
std::vector<continue_stmt> contlist;

void push_breaklist(unsigned pos, unsigned label) {
    break_stmt new_break_stmt;
    new_break_stmt.pos = pos;
    new_break_stmt.label = label;

    breaklist.push_back(new_break_stmt);
}

unsigned breaklist_size() {
    return breaklist.back().pos;
}

void push_contlist(unsigned pos, unsigned label) {
    continue_stmt new_cont_stmt;
    new_cont_stmt.pos = pos;
    new_cont_stmt.label = label;

    contlist.push_back(new_cont_stmt);
}

unsigned contlist_size() {
    return contlist.back().pos;
}

void patchlabel(unsigned quad_num, unsigned label) {
    //std::cout << "Patching quad_num: " << quad_num << ", label: " << label << std::endl;
    if (quad_num >= quad_table.size()) {
        std::cerr << "Error: quad_num " << quad_num << " is out of bounds (quad_table size: " << quad_table.size() << ")" << std::endl;
        abort();
    }
    if (quad_table.at(quad_num).label && quad_table.at(quad_num).label != UINT_MAX && quad_table.at(quad_num).label != label) {
        std::cerr << "Error: quad_num " << quad_num << " already has a conflicting label (" << quad_table.at(quad_num).label << ")" << std::endl;
        abort();
    }
    quad_table.at(quad_num).label = label;
    
}

void patchlist_breaklist(unsigned label) {
    if (breaklist.empty()) {
        return;
    }
    for (auto& break_stmt : breaklist) {
        quad_table.at(break_stmt.pos).label = label;
    }
}

void patchlist_contlist(unsigned label) {
    if (contlist.empty()) {
        return;
    }
    for (auto& cont_stmt : contlist) {
        quad_table.at(cont_stmt.pos).label = label;
    }
}

void push_state_stack(std::string state) {
    state_stack.push_back(state);
}
void pop_state_stack() {
    state_stack.pop_back();    
}
std::string state_stack_top() {
    return state_stack.back();
}

int state_stack_size() {
    return state_stack.size();
}

void make_indexed_map_list() {
    indexed_map_list.push_back(indexed_map);
}

std::map<expression*, expression*> get_indexed_map() {
    //std::cout<< "Indexed Map Size: " << indexed_map_list.size() << std::endl;
    return indexed_map_list.back();
}

indexed_elem* make_indexed_elem(expression* index, expression* value) {
    indexed_elem* new_indexed_elem = new indexed_elem;

    new_indexed_elem->index = index;
    new_indexed_elem->value = value;
    return new_indexed_elem;
}

void backpatch(unsigned quad_num, unsigned m_quad) {
    //std::cout << "Patching quad_num: " << quad_num << ", label: " << m_quad << std::endl;
    if (quad_num >= quad_table.size()) {
        std::cerr << "Error: quad_num " << quad_num << " is out of bounds (quad_table size: " << quad_table.size() << ")" << std::endl;
        abort();
    }
    if (quad_table.at(quad_num).label && quad_table.at(quad_num).label != UINT_MAX && quad_table.at(quad_num).label != m_quad) {
        std::cerr << "Error: quad_num " << quad_num << " already has a conflicting label (" << quad_table.at(quad_num).label << ")" << std::endl;
        abort();
    }
    quad_table.at(quad_num).label = m_quad;
}