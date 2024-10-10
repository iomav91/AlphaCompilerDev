#include "icode_managers.h"

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

void set_elist_expression(expression* table) {
    int i = 0;
    for (auto& expr : expression_list) {
        std::cout << "Expression in Brackets is of type " << expr->type << std::endl;
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
        std::cout << "Index is " << index.first->str_const << std::endl;
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

void get_last_expression() {
    //std::cout << "Expression List Size is " << expression_list.size() << std::endl; 
    if (expression_list.size() == 0) {
        return;
    }

    for (int i = expression_list.size()-1; i >= 0; i--) {
        //std::cout << expression_list.at(i)->symbol->name << std::endl;
        if (expression_list.at(i) != NULL) {
            reversed_expression_list.push_back(expression_list.at(i));
            expression_list.pop_back();
        }
    }
    return;
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

expression* make_call(expression* lvalue) {
    //std::cout << "MAKE CALL" << lvalue->symbol->name << std::endl;
    expression* func = emit_if_table_item(lvalue);
    for (auto& expr : reversed_expression_list) {
        emit_param(PARAM, expr);
    }
    reversed_expression_list.clear();
    expression_list.clear();
    //std::cout << "Reversed expression list size " << expression_list.size() << std::endl;
    emit_call(CALL, func);
    SymbolTableEntry new_temp = new_temp_var(lvalue->symbol->line, lvalue->symbol->type);
    std::cout << new_temp.name << std::endl;
    expression* result = make_lvalue_expression(VAR_EXPR, get_symbol(new_temp.name, get_scope()));
    std::cout << "Result is " << result->symbol->name << std::endl;
    emit_get_ret_val(GETRETVAL, result);
    return result;
}

void emit_arithm_one(iopcode op, expression* result, expression* arg1) {
    quad_table.push_back(make_quad(op, result, arg1, NULL, UINT_MAX, line_op_counter));
    line_op_counter++;
    return;
}

void emit_bool(iopcode op, expression* result, expression* arg1) {
    quad_table.push_back(make_quad(op, result, arg1, NULL, UINT_MAX, line_op_counter));
    line_op_counter++;
    return;
}

void emit_assign(iopcode op, expression* result, expression* arg1) {
    //std::cout << "ASSIGN EXPR 2" << std::endl;
    //std::cout << "IN EMIT_ASSIGN RESULT is " << result->symbol->name << std::endl;
    //std::cout << "IN EMIT_ASSIGN ARG1 is " << arg1->symbol->name << std::endl;
    //std::cout << "RESULT is " << quad_table.back().result->symbol->name << std::endl;
    quad_table.push_back(make_quad(op, result, arg1, NULL, UINT_MAX, line_op_counter));
    line_op_counter++;
    return;
}

void emit_add(iopcode op, expression* result, expression* arg1, expression* arg2) {
    std::cout << "ARG 1 " << arg1->type << std::endl;
    std::cout << "ARG 2 " << arg2->type << std::endl;
    quad_table.push_back(make_quad(op, result, arg1, arg2, UINT_MAX, line_op_counter));
    line_op_counter++;
    return;
}

void emit_sub(iopcode op, expression* result, expression* arg1, expression* arg2) {
    std::cout << "ARG 1 " << arg1->type << "  " << arg1->symbol->name << std::endl;
    std::cout << "ARG 2 " << arg2->type << std::endl;
    quad_table.push_back(make_quad(op, result, arg1, arg2, UINT_MAX, line_op_counter));
    line_op_counter++;
    return;
}

void emit_mul(iopcode op, expression* result, expression* arg1, expression* arg2) {
    std::cout << "ARG 1 " << arg1->type << "  " << arg1->symbol->name << std::endl;
    std::cout << "ARG 2 " << arg2->type << std::endl;
    quad_table.push_back(make_quad(op, result, arg1, arg2, UINT_MAX, line_op_counter));
    line_op_counter++;
    return;
}

void emit_div(iopcode op, expression* result, expression* arg1, expression* arg2) {
    std::cout << "ARG 1 " << arg1->type << "  " << arg1->symbol->name << std::endl;
    std::cout << "ARG 2 " << arg2->type << std::endl;
    quad_table.push_back(make_quad(op, result, arg1, arg2, UINT_MAX, line_op_counter));
    line_op_counter++;
    return;
}

void emit_mod(iopcode op, expression* result, expression* arg1, expression* arg2) {
    std::cout << "ARG 1 " << arg1->type << "  " << arg1->symbol->name << std::endl;
    std::cout << "ARG 2 " << arg2->type << std::endl;
    quad_table.push_back(make_quad(op, result, arg1, arg2, UINT_MAX, line_op_counter));
    line_op_counter++;
    return;
}

void emit_if_greater(iopcode op, expression* result, expression* arg1, unsigned label) {
    std::cout << "Quad Label is " << label << std::endl;
    quad_table.push_back(make_quad(op, result, arg1, NULL, label, line_op_counter));
    line_op_counter++;
    return;
}

void emit_if_less(iopcode op, expression* result, expression* arg1, unsigned label) {
    std::cout << "Quad Label is " << label << std::endl;
    quad_table.push_back(make_quad(op, result, arg1, NULL, label, line_op_counter));
    line_op_counter++;
    return;
}

void emit_if_greatereq(iopcode op, expression* result, expression* arg1, unsigned label) {
    std::cout << "Quad Label is " << label << std::endl;
    quad_table.push_back(make_quad(op, result, arg1, NULL, label, line_op_counter));
    line_op_counter++;
    return;
}

void emit_if_lesseq(iopcode op, expression* result, expression* arg1, unsigned label) {
    std::cout << "Quad Label is " << label << std::endl;
    quad_table.push_back(make_quad(op, result, arg1, NULL, label, line_op_counter));
    line_op_counter++;
    return;
}

void emit_if_equal(iopcode op, expression* result, expression* arg1, unsigned label) {
    std::cout << "Quad Label is " << label << std::endl;
    quad_table.push_back(make_quad(op, result, arg1, NULL, label, line_op_counter));
    line_op_counter++;
    return;
}

void emit_if_not_equal(iopcode op, expression* result, expression* arg1, unsigned label) {
    std::cout << "Quad Label is " << label << std::endl;
    quad_table.push_back(make_quad(op, result, arg1, NULL, label, line_op_counter));
    line_op_counter++;
    return;
}

void emit_jump(iopcode op, unsigned label) {
    std::cout << "Quad Label is " << op << std::endl;
    quad_table.push_back(make_quad(op, NULL, NULL, NULL, label, line_op_counter));
    line_op_counter++;
    return;
}

void emit_table_get_item(iopcode op, expression* result, expression* arg1, expression* arg2) {
    //std::cout << "HERE 2" << std::endl;
    //std::cout << "ARG 1 " << arg1->symbol->name <<  std::endl;
    //std::cout << "ARG 2 " << arg2->symbol->name <<  std::endl;
    quad_table.push_back(make_quad(op, result, arg1, arg2, UINT_MAX, line_op_counter));
    line_op_counter++;
    return;
}

void emit_table_set_item(iopcode op, expression* result, expression* arg1, expression* arg2) {
    std::cout << arg1->type << std::endl;
    quad_table.push_back(make_quad(op, result, arg1, arg2, UINT_MAX, line_op_counter));
    line_op_counter++;
    return;
}

expression* emit_if_table_item(expression* lvalue) {
    //std::cout << lvalue->type << std::endl;
    if (lvalue->type != TABLEITEM_EXPR) {
        return lvalue;
    } else {
        //std::cout << "HERE 1" << std::endl;
        SymbolTableEntry new_temp = new_temp_var(lvalue->symbol->line, lvalue->symbol->type);
        //std::cout << new_temp.name << std::endl;
        expression* result = make_lvalue_expression(VAR_EXPR, get_symbol(new_temp.name, get_scope()));
        //std::cout << "L-VALUE->INDEX symbol " << lvalue->symbol->name << std::endl;
        //std::cout << "L-VALUE->INDEX type " << lvalue->index->bool_const << std::endl;
        emit_table_get_item(TABLEGETELEM, result, lvalue, lvalue->index);
        return result;
    }
}

void emit_funcdef(iopcode op, expression* result) {
    std::cout << "HERE" << std::endl;
    quad_table.push_back(make_quad(op, result, NULL, NULL, UINT_MAX, line_op_counter));
    line_op_counter++;
    return;
}

void emit_param(iopcode op, expression* result) {
    quad_table.push_back(make_quad(op, result, NULL, NULL, UINT_MAX, line_op_counter));
    line_op_counter++;
    return;
}

void emit_call(iopcode op, expression* result) {
    quad_table.push_back(make_quad(op, result, NULL, NULL, UINT_MAX, line_op_counter));
    line_op_counter++;
    return;
}

void emit_get_ret_val(iopcode op, expression* result) {
    quad_table.push_back(make_quad(op, result, NULL, NULL, UINT_MAX, line_op_counter));
    line_op_counter++;
    return;
}

void emit_table_create(iopcode op, expression* result) {
    quad_table.push_back(make_quad(op, result, NULL, NULL, UINT_MAX, line_op_counter));
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
    expression* new_expression = new expression;

    new_expression->type = type;
    new_expression->symbol = symbol;

    //expression_list.push_back(new_expression);

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
    std::cout << "Name " << name << std::endl;
    expression* table_item = make_table_item_expression(TABLEITEM_EXPR, lvalue->symbol, make_conststring_expression(CONSTSTRING_EXPR, name));
    std::cout << "Table Item Index is " << table_item->index->str_const << std::endl;
    return table_item;
}

expression* make_func_expression(expr_value type, SymbolTableEntry* symbol) {
    expression* new_expression = new expression;

    new_expression->type = type;
    new_expression->symbol = symbol;
    
    expression_list.push_back(new_expression);

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
                //std::cout << "ASSIGN EXPR 3" << std::endl;
                if (quad.result->type != ASSIGN_EXPR) {
                    //std::cout << "ASSIGN EXPR 3 is of type " << quad.result->symbol->name << std::endl;
                    if (quad.arg1->type == VAR_EXPR) {
                        //std::cout << "ASSIGN EXPR 4" << std::endl;
                        std::cout << quad.line << "\t\t" << "ASSIGN" << "\t\t" << quad.result->symbol->name << "\t\t\t" << quad.arg1->symbol->name << std::endl;
                        break;
                    } else
                    if (quad.arg1->type == TABLEITEM_EXPR) {
                        std::cout << quad.line << "\t\t" << "ASSIGN" << "\t\t" << quad.result->symbol->name << "\t\t\t" << quad.arg1->symbol->name << quad.arg1->index->str_const << std::endl;
                        break;
                    } else
                    if (quad.arg1->type == CONSTNUM_EXPR) {
                        //std::cout << "ASSIGN EXPR 3" << std::endl;
                        std::cout << quad.line << "\t\t" << "ASSIGN" << "\t\t" << quad.result->symbol->name << "\t\t\t" << quad.arg1->num_const << std::endl;
                        break;
                    } else if (quad.arg1->type == CONSTBOOL_EXPR) {
                        std::cout << quad.line << "\t\t" << "ASSIGN" << "\t\t" << quad.result->symbol->name << "\t\t\t" << quad.arg1->bool_const << std::endl;
                        break;
                    } else {
                        std::cout << quad.line << "\t\t" << "ASSIGN" << "\t\t" << quad.result->symbol->name << "\t\t\t" << quad.arg1->symbol->name << std::endl;
                        break;
                    }
                } else {
                    std::cout << "ASSIGN EXPR 5" << std::endl;
                    if (quad.result->symbol == NULL) {
                        break;
                    } else {
                        std::cout << quad.line << "\t\t" << "ASSIGN" << "\t\t" << quad.result->symbol->name << "\t\t\t" << quad.arg1->symbol->name << std::endl;
                        break;
                    }
                }
                break;
            case 1:
                if (quad.arg1->type == VAR_EXPR && quad.arg2->type == VAR_EXPR) {
                    std::cout << "ADD EXPR 2" << std::endl;
                    std::cout << quad.line << "\t\t" << "ADD" << "\t\t" << quad.result->symbol->name << "\t\t" << quad.arg1->symbol->name << "\t\t" << quad.arg2->symbol->name << std::endl;
                    break;
                } else if (quad.arg1->type == VAR_EXPR && quad.arg2->type == CONSTNUM_EXPR) {
                    std::cout << "ADD EXPR 3" << std::endl;
                    std::cout << quad.line << "\t\t" << "ADD" << "\t\t" << quad.result->symbol->name << "\t\t" << quad.arg1->symbol->name << "\t\t" << quad.arg2->num_const << std::endl;
                    break;
                } else if (quad.arg1->type == ARITHM_EXPR && quad.arg2->type == ARITHM_EXPR) {
                    //std::cout << "ADD EXPR 2" << std::endl;
                    std::cout << quad.line << "\t\t" << "ADD" << "\t\t" << quad.result->symbol->name << "\t\t" << quad.arg1->symbol->name << "\t\t" << quad.arg2->symbol->name << std::endl;
                    break;
                } else {
                    if (quad.arg1->symbol == NULL && quad.arg2->symbol == NULL) {
                        std::cout << quad.line << "\t\t" << "ADD" << "\t\t" << quad.result->symbol->name << "\t\t" << quad.arg1->symbol->name << "\t\t" << quad.arg2->symbol->name << std::endl;
                        break;
                    } else {
                        if (quad.arg2->type == VAR_EXPR) {
                            std::cout << quad.line << "\t\t" << "ADD" << "\t\t" << quad.result->symbol->name << "\t\t" << quad.arg1->symbol->name << "\t\t" << quad.arg2->symbol->name << std::endl;
                            break;
                        }
                    }
                }
                break;
            case 2:
                if (quad.arg1->type == VAR_EXPR && quad.arg2->type == VAR_EXPR) {
                    std::cout << "ADD EXPR 2" << std::endl;
                    std::cout << quad.line << "\t\t" << "SUB" << "\t\t" << quad.result->symbol->name << "\t\t" << quad.arg1->symbol->name << "\t\t" << quad.arg2->symbol->name << std::endl;
                    break;
                } else if (quad.arg1->type == VAR_EXPR && quad.arg2->type == CONSTNUM_EXPR) {
                    std::cout << "ADD EXPR 3" << std::endl;
                    std::cout << quad.line << "\t\t" << "SUB" << "\t\t" << quad.result->symbol->name << "\t\t" << quad.arg1->symbol->name << "\t\t" << quad.arg2->num_const << std::endl;
                    break;
                } else {
                    if (quad.arg1->symbol == NULL && quad.arg2->symbol == NULL) {
                        std::cout << quad.line << "\t\t" << "SUB" << "\t\t" << quad.result->symbol->name << "\t\t" << quad.arg1->symbol->name << "\t\t" << quad.arg2->symbol->name << std::endl;
                        break;
                    } else {
                        if (quad.arg2->type == VAR_EXPR) {
                            std::cout << quad.line << "\t\t" << "SUB" << "\t\t" << quad.result->symbol->name << "\t\t" << quad.arg1->symbol->name << "\t\t" << quad.arg2->symbol->name << std::endl;
                            break;
                        }
                    }
                }
                break;
            case 3:
                if (quad.arg1->type == VAR_EXPR && quad.arg2->type == VAR_EXPR) {
                    std::cout << "ADD EXPR 2" << std::endl;
                    std::cout << quad.line << "\t\t" << "MUL" << "\t\t" << quad.result->symbol->name << "\t\t" << quad.arg1->symbol->name << "\t\t" << quad.arg2->symbol->name << std::endl;
                    break;
                } else if (quad.arg1->type == VAR_EXPR && quad.arg2->type == CONSTNUM_EXPR) {
                    std::cout << "ADD EXPR 3" << std::endl;
                    std::cout << quad.line << "\t\t" << "MUL" << "\t\t" << quad.result->symbol->name << "\t\t" << quad.arg1->symbol->name << "\t\t" << quad.arg2->num_const << std::endl;
                    break;
                } else {
                    if (quad.arg1->symbol == NULL && quad.arg2->symbol == NULL) {
                        std::cout << quad.line << "\t\t" << "MUL" << "\t\t" << quad.result->symbol->name << "\t\t" << quad.arg1->symbol->name << "\t\t" << quad.arg2->symbol->name << std::endl;
                        break;
                    } else {
                        if (quad.arg2->type == VAR_EXPR) {
                            std::cout << quad.line << "\t\t" << "MUL" << "\t\t" << quad.result->symbol->name << "\t\t" << quad.arg1->symbol->name << "\t\t" << quad.arg2->symbol->name << std::endl;
                            break;
                        }
                    }
                }
                break;
            case 4:
                if (quad.arg1->type == VAR_EXPR && quad.arg2->type == VAR_EXPR) {
                    std::cout << "ADD EXPR 2" << std::endl;
                    std::cout << quad.line << "\t\t" << "DIV" << "\t\t" << quad.result->symbol->name << "\t\t" << quad.arg1->symbol->name << "\t\t" << quad.arg2->symbol->name << std::endl;
                    break;
                } else if (quad.arg1->type == VAR_EXPR && quad.arg2->type == CONSTNUM_EXPR) {
                    std::cout << "ADD EXPR 3" << std::endl;
                    std::cout << quad.line << "\t\t" << "DIV" << "\t\t" << quad.result->symbol->name << "\t\t" << quad.arg1->symbol->name << "\t\t" << quad.arg2->num_const << std::endl;
                    break;
                } else {
                    if (quad.arg1->symbol == NULL && quad.arg2->symbol == NULL) {
                        std::cout << quad.line << "\t\t" << "DIV" << "\t\t" << quad.result->symbol->name << "\t\t" << quad.arg1->symbol->name << "\t\t" << quad.arg2->symbol->name << std::endl;
                        break;
                    } else {
                        if (quad.arg2->type == VAR_EXPR) {
                            std::cout << quad.line << "\t\t" << "DIV" << "\t\t" << quad.result->symbol->name << "\t\t" << quad.arg1->symbol->name << "\t\t" << quad.arg2->symbol->name << std::endl;
                            break;
                        }
                    }
                }
                break;
            case 5:
                if (quad.arg1->type == VAR_EXPR && quad.arg2->type == VAR_EXPR) {
                    std::cout << "ADD EXPR 2" << std::endl;
                    std::cout << quad.line << "\t\t" << "MOD" << "\t\t" << quad.result->symbol->name << "\t\t" << quad.arg1->symbol->name << "\t\t" << quad.arg2->symbol->name << std::endl;
                    break;
                } else if (quad.arg1->type == VAR_EXPR && quad.arg2->type == CONSTNUM_EXPR) {
                    std::cout << "ADD EXPR 3" << std::endl;
                    std::cout << quad.line << "\t\t" << "MOD" << "\t\t" << quad.result->symbol->name << "\t\t" << quad.arg1->symbol->name << "\t\t" << quad.arg2->num_const << std::endl;
                    break;
                } else {
                    if (quad.arg1->symbol == NULL && quad.arg2->symbol == NULL) {
                        std::cout << quad.line << "\t\t" << "MOD" << "\t\t" << quad.result->symbol->name << "\t\t" << quad.arg1->symbol->name << "\t\t" << quad.arg2->symbol->name << std::endl;
                        break;
                    } else {
                        if (quad.arg2->type == VAR_EXPR) {
                            std::cout << quad.line << "\t\t" << "MOD" << "\t\t" << quad.result->symbol->name << "\t\t" << quad.arg1->symbol->name << "\t\t" << quad.arg2->symbol->name << std::endl;
                            break;
                        }
                    }
                }
                break;
            case 6:
                std::cout << quad.line << "\t\t" << "UMINUS" << "\t\t" << quad.result->symbol->name << "\t\t" << quad.arg1->symbol->name << std::endl;
                break;
            case 7:
                std::cout << quad.line << "\t\t" << "AND" << "\t\t" << quad.result->symbol->name << "\t\t" << quad.arg1->symbol->name << "\t\t" << quad.arg2->symbol->name << std::endl;
                break;
            case 8:
                std::cout << quad.line << "\t\t" << "OR" << "\t\t" << quad.result->symbol->name << "\t\t" << quad.arg1->symbol->name << "\t\t" << quad.arg2->symbol->name << std::endl;
                break;
            case 9:
                std::cout << quad.line << "\t\t" << "NOT" << "\t\t" << quad.result->symbol->name << "\t\t" << quad.arg1->symbol->name << std::endl;
                break;
            case 10:
                std::cout << quad.line << "\t\t" << "IF EQUAL" << "\t\t" << quad.result->symbol->name << "\t\t" << quad.arg1->symbol->name << "\t\t" << quad.label << std::endl;
                break;
            case 11:
                std::cout << quad.line << "\t\t" << "IF NOT EQUAL" << "\t\t" << quad.result->symbol->name << "\t\t" << quad.arg1->symbol->name << "\t\t" << quad.label << std::endl;
                break;
            case 12:
                std::cout << quad.line << "\t\t" << "IF LESS EQUAL" << "\t\t" << quad.result->symbol->name << "\t\t" << quad.arg1->symbol->name << "\t\t" << quad.label << std::endl;
                break;
            case 13:
                std::cout << quad.line << "\t\t" << "IF GREATER EQUAL" << "\t\t" << quad.result->symbol->name << "\t\t" << quad.arg1->symbol->name << "\t\t" << quad.label << std::endl;
                break;
            case 14:
                std::cout << quad.line << "\t\t" << "IF LESS" << "\t\t" << quad.result->symbol->name << "\t\t" << quad.arg1->symbol->name << "\t\t" << quad.label << std::endl;
                break;
            case 15:
                std::cout << quad.line << "\t\t" << "IF GREATER" << "\t\t" << quad.result->symbol->name << "\t\t" << quad.arg1->symbol->name << "\t\t" << quad.label << std::endl;
                break;
            case 16:
                std::cout << quad.line << "\t\t" << "CALL" << "\t\t" << quad.result->symbol->name << std::endl;
                break;
            case 17:
                if (quad.result->type == CONSTNUM_EXPR) {
                    std::cout << quad.line << "\t\t" << "PARAM" << "\t\t" << quad.result->num_const << std::endl;
                    break;
                } else {
                    std::cout << quad.line << "\t\t" << "PARAM" << "\t\t" << quad.result->symbol->name << std::endl;
                    break;
                }
                break;
            case 18:
                std::cout << quad.line << "\t\t" << "RETURN" << "\t\t" << quad.result->symbol->name << "\t\t" << quad.arg1->symbol->name << "\t\t" << quad.arg2->symbol->name << std::endl;
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
                    std::cout << quad.line << "\t\t\t\t" << "TABLEGETELEM" << "\t" << quad.result->symbol->name << "\t\t" << quad.arg1->symbol->name << "\t\t\t" << quad.arg2->num_const << std::endl;
                    break;
                }
                std::cout << quad.line << "\t\t\t\t" << "TABLEGETELEM" << "\t" << quad.result->symbol->name << "\t\t" << quad.arg1->symbol->name << "\t\t\t" << quad.arg2->str_const << std::endl;
                break;
            case 24:
                if (quad.arg1->type == CONSTNUM_EXPR) {
                    if (quad.arg2->type == VAR_EXPR) {
                        //std::cout << "TABLESETELEM HERE 2.1" << std::endl;
                        std::cout << quad.line << "\t\t" << "TABLESETELEM" << "\t\t" << quad.result->symbol->name << "\t\t" << quad.arg1->num_const << "\t\t" << quad.arg2->symbol->name << std::endl;
                        break;
                    } else if (quad.arg2->type == TABLEITEM_EXPR) {
                        //std::cout << "TABLESETELEM HERE 3" << std::endl;
                        std::cout << quad.line << "\t\t" << "TABLESETELEM" << "\t\t" << quad.result->symbol->name << "\t\t" << quad.arg1->num_const << "\t\t" << quad.arg2->str_const << std::endl;
                        break;
                    } else if (quad.arg2->type == CONSTNUM_EXPR) {
                        //std::cout << "TABLESETELEM HERE 3" << std::endl;
                        std::cout << quad.line << "\t\t" << "TABLESETELEM" << "\t\t" << quad.result->symbol->name << "\t\t" << quad.arg1->num_const << "\t\t" << quad.arg2->num_const << std::endl;
                        break;
                    } else if (quad.arg2->type == CONSTSTRING_EXPR) {
                        //std::cout << "TABLESETELEM HERE 3" << std::endl;
                        std::cout << quad.line << "\t\t" << "TABLESETELEM" << "\t\t" << quad.result->symbol->name << "\t\t" << quad.arg1->num_const << "\t\t" << quad.arg2->str_const << std::endl;
                        break;
                    } else if (quad.arg2->type == LIBFUNC_EXPR || quad.arg2->type == LIBFUNC_EXPR) {
                        //std::cout << "TABLESETELEM HERE 3" << std::endl;
                        std::cout << quad.line << "\t\t" << "TABLESETELEM" << "\t\t" << quad.result->symbol->name << "\t\t" << quad.arg1->num_const << "\t\t" << quad.arg2->symbol->name << std::endl;
                        break;
                    } else {
                        //std::cout << "Quad Arg2 Type is " << quad.arg2->type << std::endl;
                        //std::cout << "TABLESETELEM HERE 2" << std::endl;
                        std::cout << quad.line << "\t\t" << "TABLESETELEM" << "\t\t" << quad.result->symbol->name << "\t\t" << quad.arg1->num_const << "\t\t" << quad.arg2->num_const << std::endl;
                        break;
                    }
                } else if (quad.arg1->type == CONSTSTRING_EXPR) {
                    if (quad.arg2->type == VAR_EXPR) {
                        std::cout << quad.line << "\t\t" << "TABLESETELEM" << "\t\t" << quad.result->symbol->name << "\t\t" << quad.arg1->str_const << "\t\t" << quad.arg2->symbol->name << std::endl;
                        break;
                    } else {
                        //std::cout << "TABLESETELEM HERE 3" << std::endl;
                        std::cout << quad.line << "\t\t" << "TABLESETELEM" << "\t\t" << quad.result->symbol->name << "\t\t" << quad.arg1->str_const << "\t\t" << quad.arg2->num_const << std::endl;
                        break;
                    }
                } else if (quad.arg1->type == CONSTBOOL_EXPR) {
                    std::cout << quad.line << "\t\t" << "TABLESETELEM" << "\t\t" << quad.result->symbol->name << "\t\t" << quad.arg1->bool_const << "\t\t" << quad.arg2->num_const << std::endl;
                    break;
                } else {
                    if (quad.arg2->type == CONSTNUM_EXPR) {
                        std::cout << "TABLESETELEM HERE 1" << std::endl;
                        std::cout << quad.line << "\t\t" << "TABLESETELEM" << "\t\t" << quad.result->symbol->name << "\t\t" << quad.arg1->symbol->name << "\t\t" << quad.arg2->num_const << std::endl;
                        break;
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
    if (expr->type == CONSTBOOL_EXPR || expr->type == CONSTSTRING_EXPR || expr->type == NIL_EXPR || 
        expr->type == NEWTABLE_EXPR || expr->type == PROGRAMFUNC_EXPR || expr->type == LIBFUNC_EXPR || expr->type == BOOL_EXPR) {
        return false;
    }
    return true;
}

bool check_if_is_arithm_alt(expression* expr) {
    if (expr->type == CONSTSTRING_EXPR || expr->type == NIL_EXPR || expr->type == PROGRAMFUNC_EXPR 
        || expr->type == LIBFUNC_EXPR) {
        return false;
    }
    return true;
}

void set_curr_quad_label(unsigned step) {
    curr_quad_label = step;
}

unsigned next_quad_label() {
    std::cout << curr_quad_label << std::endl;
    return curr_quad_label; 
}

expression* manage_expr_plus_expr(expression* result, expression* expr1, expression* expr2) {
	if (check_if_is_arithm(expr1) == true && check_if_is_arithm(expr2) == true) {
        SymbolType type = GLOBAL;
        if (get_scope() != 0) {
            type = LOCAL;
        }
        SymbolTableEntry new_temp = new_temp_var(-1, type);
        //std::cout << new_temp.scope << std::endl;
        result = make_arithm_expression(ARITHM_EXPR, get_symbol(new_temp.name, get_scope()));
        //std::cout << "$1->type " << expr1->type << std::endl;
        emit_add(ADD, result, expr1, expr2);
   }
   return result;
}

expression* manage_expr_minus_expr(expression* result, expression* expr1, expression* expr2) {
	if (check_if_is_arithm(expr1) == true && check_if_is_arithm(expr2) == true) {
        SymbolType type = GLOBAL;
        if (get_scope() != 0) {
            type = LOCAL;
        }
        SymbolTableEntry new_temp = new_temp_var(-1, type);
        //std::cout << new_temp.scope << std::endl;
        result = make_arithm_expression(ARITHM_EXPR, get_symbol(new_temp.name, get_scope()));
        //std::cout << "$1->type " << expr1->type << std::endl;
        emit_sub(SUB, result, expr1, expr2);
    }
    return result;
}

expression* manage_expr_mul_expr(expression* result, expression* expr1, expression* expr2) {
	if (check_if_is_arithm(expr1) == true && check_if_is_arithm(expr2) == true) {
        SymbolType type = GLOBAL;
        if (get_scope() != 0) {
            type = LOCAL;
        }
        SymbolTableEntry new_temp = new_temp_var(-1, type);
        //std::cout << new_temp.scope << std::endl;
        result = make_arithm_expression(ARITHM_EXPR, get_symbol(new_temp.name, get_scope()));
        //std::cout << "$1->type " << expr1->type << std::endl;
        emit_mul(MUL, result, expr1, expr2);
                    
    }
    return result;
}

expression* manage_expr_div_expr(expression* result, expression* expr1, expression* expr2) {
	if (check_if_is_arithm(expr1) == true && check_if_is_arithm(expr2) == true) {
        SymbolType type = GLOBAL;
        if (get_scope() != 0) {
            type = LOCAL;
        }
        SymbolTableEntry new_temp = new_temp_var(-1, type);
        //std::cout << new_temp.scope << std::endl;
        result = make_arithm_expression(ARITHM_EXPR, get_symbol(new_temp.name, get_scope()));
        //std::cout << "$1->type " << expr1->type << std::endl;
        emit_div(DIV, result, expr1, expr2);
    }
    return result;
}

expression* manage_expr_mod_expr(expression* result, expression* expr1, expression* expr2) {
	if (check_if_is_arithm(expr1) == true && check_if_is_arithm(expr2) == true) {
        SymbolType type = GLOBAL;
        if (get_scope() != 0) {
            type = LOCAL;
        }
        SymbolTableEntry new_temp = new_temp_var(-1, type);
        //std::cout << new_temp.scope << std::endl;
        result = make_arithm_expression(ARITHM_EXPR, get_symbol(new_temp.name, get_scope()));
        //std::cout << "$1->type " << expr1->type << std::endl;
        emit_mod(MOD, result, expr1, expr2);
    }
    return result;
}

expression* manage_expr_mod_expr(expression* result, expression* expr1, expression* expr2) {
	if (check_if_is_arithm(expr1) == true && check_if_is_arithm(expr2) == true) {
        SymbolType type = GLOBAL;
        if (get_scope() != 0) {
            type = LOCAL;
        }
        SymbolTableEntry new_temp = new_temp_var(-1, type);
        //std::cout << new_temp.scope << std::endl;
        result = make_arithm_expression(ARITHM_EXPR, get_symbol(new_temp.name, get_scope()));
        //std::cout << "$1->type " << expr1->type << std::endl;
        emit_mod(MOD, result, expr1, expr2);
    }
    return result;
}

expression* manage_expr_gr_expr(expression* result, expression* expr1, expression* expr2) {
	SymbolType type = GLOBAL;
    if (get_scope() != 0) {
        type = LOCAL;
    }
    SymbolTableEntry new_temp = new_temp_var(-1, type);
    //std::cout << new_temp.scope << std::endl;
    result = make_bool_expression(BOOL_EXPR, get_symbol(new_temp.name, get_scope()));
    emit_if_greater(IF_GR, expr1, expr2, next_quad_label()+3);
    set_curr_quad_label((next_quad_label()+3));
    emit_assign(ASSIGN, result, make_constbool_expression(CONSTBOOL_EXPR, 1));
    emit_jump(JUMP, (next_quad_label()+2));
    set_curr_quad_label((next_quad_label()+2));
    emit_assign(ASSIGN, result, make_constbool_expression(CONSTBOOL_EXPR, 0));
}

expression* manage_expr_ls_expr(expression* result, expression* expr1, expression* expr2) {
	SymbolType type = GLOBAL;
    if (get_scope() != 0) {
        type = LOCAL;
    }
    SymbolTableEntry new_temp = new_temp_var(-1, type);
    //std::cout << new_temp.scope << std::endl;
    result = make_bool_expression(BOOL_EXPR, get_symbol(new_temp.name, get_scope()));
    emit_if_less(IF_L, expr1, expr2, next_quad_label()+3);
    set_curr_quad_label((next_quad_label()+3));
    emit_assign(ASSIGN, result, make_constbool_expression(CONSTBOOL_EXPR, 1));
    emit_jump(JUMP, (next_quad_label()+2));
    set_curr_quad_label((next_quad_label()+2));
    emit_assign(ASSIGN, result, make_constbool_expression(CONSTBOOL_EXPR, 0));

    return result;
}

expression* manage_expr_gr_eq_expr(expression* result, expression* expr1, expression* expr2) {
	SymbolType type = GLOBAL;
    if (get_scope() != 0) {
        type = LOCAL;
    }
    SymbolTableEntry new_temp = new_temp_var(-1, type);
    //std::cout << new_temp.scope << std::endl;
    result = make_bool_expression(BOOL_EXPR, get_symbol(new_temp.name, get_scope()));
    emit_if_greatereq(IF_GR_EQ, expr1, expr2, next_quad_label()+3);
    set_curr_quad_label((next_quad_label()+3));
    emit_assign(ASSIGN, result, make_constbool_expression(CONSTBOOL_EXPR, 1));
    emit_jump(JUMP, (next_quad_label()+2));
    set_curr_quad_label((next_quad_label()+2));
    emit_assign(ASSIGN, result, make_constbool_expression(CONSTBOOL_EXPR, 0));

    return result;
}

expression* manage_expr_ls_eq_expr(expression* result, expression* expr1, expression* expr2) {
	SymbolType type = GLOBAL;
    if (get_scope() != 0) {
        type = LOCAL;
    }
    SymbolTableEntry new_temp = new_temp_var(-1, type);
    //std::cout << new_temp.scope << std::endl;
    result = make_bool_expression(BOOL_EXPR, get_symbol(new_temp.name, get_scope()));
    emit_if_lesseq(IF_L_EQ, expr1, expr2, next_quad_label()+3);
    set_curr_quad_label((next_quad_label()+3));
    emit_assign(ASSIGN, result, make_constbool_expression(CONSTBOOL_EXPR, 1));
    emit_jump(JUMP, (next_quad_label()+2));
    set_curr_quad_label((next_quad_label()+2));
    emit_assign(ASSIGN, result, make_constbool_expression(CONSTBOOL_EXPR, 0));

    return result;
}

