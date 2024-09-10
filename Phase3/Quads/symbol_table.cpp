#include "symbol_table.h"
#include <cstdlib>

#define  SIZE  1000

int temp_counter = 0;
unsigned line_op_counter = 0;
unsigned prev_scope_space_counter = 1;
unsigned program_var_offset = 0;
unsigned function_local_offset = 0;
unsigned formal_arg_offset = 0;
unsigned scope_space_counter = 1;
unsigned curr_quad_label = 0;

/*struct Key {
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

struct quad {
    enum iopcode        op;
    expression*         result;
    expression*         arg1;
    expression*         arg2;
    unsigned            label;
    unsigned            line;
};*/

std::vector<std::map<std::string, SymbolTableEntry>> symbol_table_alt;
std::vector<std::map<std::string, SymbolTableEntry>> inactive_symbol_table;
std::vector<quad> quad_table;

Key make_key(const std::string& symbol_name, const int& symbol_scope) {
    Key key;

    key.name = symbol_name;
    key.scope = symbol_scope;

    return key;
}

SymbolTableEntry make_entry(const std::string& symbol_name, const
int& symbol_scope, const int& symbol_line, scope_space space, unsigned offset, SymbolType symbol_type) {
    SymbolTableEntry entry;

    entry.isActive = true;
    entry.type = symbol_type;
    entry.name = symbol_name;
    entry.scope = symbol_scope;
    entry.line = symbol_line;
    entry.space = space;
    entry.offset = offset;
    return entry;
}

int is_scope_change(int size, int scope) {
    if (scope >= size) {
        return 1;
    }
    return 0;
}

void init_vector() {
    std::map<std::string, SymbolTableEntry> m;
    m = insert_lib_function(m);
    symbol_table_alt.push_back(m);
    return;
}

void push_vector() {
    std::map<std::string, SymbolTableEntry> m;
    symbol_table_alt.push_back(m);
    return;
}

void push_vector_inactive(std::map<std::string, SymbolTableEntry> m) {
    inactive_symbol_table.push_back(m);
    return;
}

std::string get_space_to_word(scope_space space) {
    switch (space) {
        case 0: return "PROGRAM_VAR";
        case 1: return "FUNCTION_LOCAL";
        case 2: return "FORMAL_ARG";
        default: assert(0); 
    }
}

void print_symtable() {
    if (symbol_table_alt.size() == 0) {
        return;
    }
    for (auto map : symbol_table_alt) {
        for (const auto& symbol : map) {
            std::cout << symbol.first << "\t" << " (Type: [0 -> GLOBAL | 1 -> LOCAL | 2 -> FORMAL | 3 -> USERFUNC | 4 -> LIBFUNC]) " << symbol.second.type << "\t"
                  << " (Line) " << symbol.second.line << "\t" << " (Scope) " << symbol.second.scope << " (Space) " << get_space_to_word(symbol.second.space) << "\t" << " (Offset) " << symbol.second.offset 
                  << "\t" << "(State: [Active -> 1 | Inactive -> 0]) " << symbol.second.isActive << std::endl; 
        } 
    }
    return;
}

void print_symtable_inactive() {
    if (inactive_symbol_table.size() == 0) {
        return;
    }
    for (auto map : inactive_symbol_table) {
        for (const auto& symbol : map) {
            std::cout << symbol.first << "\t" << " (Type: [0 -> GLOBAL | 1 -> LOCAL | 2 -> FORMAL | 3 -> USERFUNC | 4 -> LIBFUNC]) " << symbol.second.type << "\t"
                  << " (Line) " << symbol.second.line << "\t" << " (Scope) " << symbol.second.scope << " (Space) " <<get_space_to_word(symbol.second.space) << "\t" << " (Offset) " << symbol.second.offset 
                  << "\t" << "(State: [Active -> 1 | Inactive -> 0]) " << symbol.second.isActive << std::endl; 
        } 
    }
    return;
}

void insert(std::string name, int scope, int line, scope_space space, unsigned offset, SymbolType type) {
    if (is_scope_change(symbol_table_alt.size(), scope) == 0) {
        //std::cout << "LOCAL" << std::endl;
        //std::cout << "handle_func_w_1arg 2.1.1" << std::endl;
        symbol_table_alt.back()[name] = make_entry(name, scope, line, space, offset, type);
        inc_curr_scope_offset();
        //symbol_table_alt.back()[name].isActive = true;
        
    } else {
        //std::cout << "THERE !!!!" << std::endl;
        push_vector(); // isos na prepei na kaneis push sta action tou parser
        
        symbol_table_alt.back()[name] = make_entry(name, scope, line, space, offset, type);
        inc_curr_scope_offset();
        
    }
    return;
}

SymbolTableEntry* get_symbol(std::string name, int scope) {
    for (auto& symbol : symbol_table_alt.at(scope)) {
        if (symbol.first == name && symbol.second.scope == scope && symbol.second.isActive == true) {
            //std::cout << "Symbol is here" << std::endl;
            //make_entry(symbol.second.name, symbol.second.scope, symbol.second.line, symbol.second.space, symbol.second.offset, symbol.second.type);
            return &symbol.second;
        }
    }
}

bool lookup_at_scope(std::string name, int scope) {
    //std::cout << "Scope is at lookup_at_scope " << scope << std::endl;
    if (symbol_table_alt.size() != 0) {
        //std::cout << "Scope is at lookup_at_scope " << scope << std::endl;
        /*for (const auto& symbol : symbol_table_alt.at(scope)) {
            std::cout << "Scope is at lookup_at_scope " << scope << symbol.first << std::endl;
            //std::cout << symbol.first << "\t"<< symbol.second.isActive << "\t" << symbol.second.line << std::endl;
            if (symbol.first == name && symbol.second.isActive == true) {
                std::cout << "Scope is at lookup_at_scope " << scope << symbol.first << std::endl;
                return true;
            }
        }*/

        for (auto map : symbol_table_alt) {
            for (const auto& symbol : map) {
                if (symbol.first == name && symbol.second.scope == scope && symbol.second.isActive == true) {
                    return true;
                }
            }    
        }
    }
    return false;
}

bool lookup(std::string name) {
    for (auto map : symbol_table_alt) {
        for (const auto& symbol : map) {
            if (symbol.first == name && symbol.second.isActive == true) {
                return true;
            }
        }   
    }
    return false;
}

/*SymbolTableEntry lookup_and_ret_sym(std::string name, int scope) {
    for (auto map : symbol_table_alt) {
        for (auto& symbol : map) {
            if (symbol.first == name && symbol.second.scope == scope && symbol.second.isActive == true) {
                return symbol;
            }
        }    
    }
    return NULL;
}*/

int lookup_and_return_scope(std::string name) {
    for (auto map : symbol_table_alt) {
        for (const auto& symbol : map) {
            if (symbol.first == name && symbol.second.isActive == true) {
                return symbol.second.scope;
            }
        }   
    }
    return -1;
}

int lookup_and_return_scope_second(std::string name, int scope) {
    for (auto map : symbol_table_alt) {
        for (const auto& symbol : map) {
            if (symbol.first == name && symbol.second.scope == scope && symbol.second.isActive == true) {
                return symbol.second.scope;
            }
        }   
    }
    return -1;
}

int is_a_func_name(std::string name) {
    for (auto map : symbol_table_alt) {
        for (const auto& symbol : map) {
            if (symbol.first == name && symbol.second.isActive == true) {
                if (symbol.second.type == USERFUNC) {
                    return 1;
                }
            }
        }   
    }
    return 0;
}

int is_symbol_active(std::string name, int scope) {
    for (auto map : symbol_table_alt) {
        for (const auto& symbol : map) {
            if (symbol.first == name && symbol.second.scope == scope && symbol.second.isActive == false) {
                return 1;
            }
        }   
    }
    return 0;
}

void hide() {
    //std::cout << "Size of the symbol table is outside" << symbol_table_alt.size() << std::endl;
    /*if (symbol_table_alt.size() >= 1 && symbol_table_alt.back().size() != 0) {
        std::cout << "Size of the symbol table is " << symbol_table_alt.size() << std::endl;
        /*for (auto& symbol : symbol_table_alt.back()) {
            symbol.second.isActive = false;
        }*/

        /*for (int i = 1; i < symbol_table_alt.size(); i++) {
            for (auto& symbol : symbol_table_alt.at(i)) {
                
                symbol.second.isActive = false;
                
            }
        }
        for (auto& symbol : symbol_table_alt.back()) {
            symbol.second.isActive = false;
        }
    }*/
    for (auto& symbol : symbol_table_alt.back()) {
        symbol.second.isActive = false;
    }
    push_vector_inactive(symbol_table_alt.back());
    symbol_table_alt.pop_back();
    return;
}

int get_vector_size() {
    return symbol_table_alt.size();
}

/*std::vector<std::map<std::string, SymbolTableEntry>> get_symtable() {
    return symbol_table_alt;
}*/


/*SymbolTableEntry make_func_entry(const std::string& symbol_name, const
int& symbol_scope, const int& symbol_line, SymbolType symbol_type) {
    SymbolTableEntry entry;

    entry.isActive = true;
    entry.type = symbol_type;
    entry.name = symbol_name;
    entry.scope = symbol_scope;
    entry.line = symbol_line;

    return entry;
}*/

//int hash_function(const std::string& key) {
  //  return key. % SIZE;
//}

extern int scope_counter;

int get_scope() {
    return scope_counter;
}

/*void init_stack() {
    std::map<std::string, SymbolTableEntry> m;
    m = insert_lib_function(m);
    symbol_table.push(m);
}

void push_stack() {
    std::map<std::string, SymbolTableEntry> m;
    symbol_table.push(m);
}

void edit_stack(std::string symbol, int scope, int line, SymbolType
symbol_type) {
    if (is_scope_change(symbol_table.size(), scope) == 0) {
        symbol_table.top()[symbol] = make_entry(symbol, scope, line, symbol_type);
    } else {
        std::cout << "THERE" << std::endl;
        push_stack(); // isos na prepei na kaneis push sta action tou parser
        
        symbol_table.top()[symbol] = make_entry(symbol, scope, line, symbol_type);
    }
}*/

std::map<std::string, SymbolTableEntry> insert_lib_function(std::map<std::string, SymbolTableEntry> m) {
    m["print"] = make_entry("print", 0, -1, program_var, curr_scope_offset(), LIBFUNC);
    inc_curr_scope_offset();
    m["input"] = make_entry("input", 0, -1, program_var, curr_scope_offset(), LIBFUNC);
    inc_curr_scope_offset();
    m["objectmemberkeys"] = make_entry("objectmemberkeys", 0, -1, program_var, curr_scope_offset(), LIBFUNC);
    inc_curr_scope_offset();
    m["objecttotalmembers"] = make_entry("objecttotalmembers", 0, -1, program_var, curr_scope_offset(), LIBFUNC);
    inc_curr_scope_offset();
    m["objectcopy"] = make_entry("objectcopy",0, -1, program_var, curr_scope_offset(), LIBFUNC);
    inc_curr_scope_offset();
    m["totalarguments"] = make_entry("totalarguments", 0, -1, program_var, curr_scope_offset(), LIBFUNC);
    inc_curr_scope_offset();
    m["argument"] = make_entry("argument", 0, -1, program_var, curr_scope_offset(), LIBFUNC);
    inc_curr_scope_offset();
    m["typeof"] = make_entry("typeof", 0, -1, program_var, curr_scope_offset(), LIBFUNC);
    inc_curr_scope_offset();
    m["strtonum"] = make_entry("strtonum", 0, -1, program_var, curr_scope_offset(), LIBFUNC);
    inc_curr_scope_offset();
    m["sqrt"] = make_entry("sqrt", 0, -1, program_var, curr_scope_offset(), LIBFUNC);
    inc_curr_scope_offset();
    m["cos"] = make_entry("cos", 0, -1, program_var, curr_scope_offset(), LIBFUNC);
    inc_curr_scope_offset();
    m["sin"] = make_entry("sin", 0, -1, program_var, curr_scope_offset(), LIBFUNC);
    inc_curr_scope_offset();

    return m;
}

/*void print_symtable() {
    for (const auto& map : symbol_table.top()) {
        std::cout << map.first << " " << map.second.scope << " "
                  << map.second.line << " " << map.second.type << " "
<< map.second.isActive << std::endl;
    }
}*/

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
    //std::cout << "HERE" << std::endl;
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

std::string new_temp_name() {
    //temp_counter++;
    return ("__t"+std::to_string(temp_counter));
}

void reset_temp_counter() {
    temp_counter = 0;
    return;
}

SymbolTableEntry new_temp_var(int line, SymbolType type) {
    std::string name = new_temp_name();
    temp_counter++;
    if (lookup_at_scope(name, 0)) {
        return symbol_table_alt.at(0)["name"];
    }
    SymbolTableEntry symbol;
    if (type == GLOBAL) {
        symbol = make_entry(name, 0, line, program_var, curr_scope_offset(), type);
        insert(symbol.name, symbol.scope, symbol.line, symbol.space, symbol.offset, symbol.type);
    } else if (type == LOCAL) {
        symbol = make_entry(name, get_scope(), line, function_local, curr_scope_offset(), type);
        insert(symbol.name, symbol.scope, symbol.line, symbol.space, symbol.offset, symbol.type);
    } else {
        symbol = make_entry(name, get_scope(), line, function_local, curr_scope_offset(), type);
        insert(symbol.name, symbol.scope, symbol.line, symbol.space, symbol.offset, symbol.type);
    }
    return symbol;
    
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

/*int main() {
    /*insert_lib_function();
    print_symtable();
    symbol_table["print"] = make_func_entry("print", 1, -1, USERFUNC);
    print_symtable();
    std::cout << symbol_table.at("print").scope << std::endl;
    SymbolTableEntry entry1;
    entry1.isActive = true;
    entry1.type = GLOBAL;
    entry1.name = "x";
    entry1.scope = 0;
    entry1.line = 1;

    SymbolTableEntry entry2;
    entry2.isActive = true;
    entry2.type = GLOBAL;
    entry2.name = "a";
    entry2.scope = 0;
    entry2.line = 1;
    symbol_table[hash_function()] = entry1;
    symbol_table[hash_function()] = entry2;
    for (int i = 0; i < SIZE; i++) {
        std::cout << i << " " << symbol_table[i].name << std::endl;
    }

    std::cout << symbol_table_alt.size() << std::endl;

    init_vector();

    std::cout << symbol_table_alt.size() << std::endl;

    print_symtable();

    insert("x", 0, -1, GLOBAL);

    print_symtable();

    insert("x", 1, -1, LOCAL);

    std::cout << "NEW SYMBOL TABLE DISPLAY" << std::endl;

    print_symtable();

    if (lookup_at_scope("x", 1)) {
        std::cout << "Symbol is in the symbol table" << std::endl;
        hide();
    }

    std::cout << "NEW SYMBOL TABLE DISPLAY" << std::endl;

    print_symtable();

    //SymbolTableEntry entry1 = make_entry("x", 0, 0, GLOBAL);

    init_vector();

    std::cout << symbol_table_alt.size() << std::endl;

    print_symtable();

    insert("x", 0, -1, program_var, curr_scope_offset(), GLOBAL);

    std::cout << "NEW SYMBOL TABLE DISPLAY" << std::endl;

    print_symtable();

    std::cout << "-------------------------------------------------------------------" << std::endl;

    //emit(ASSIGN, "a", "x", "z", UINT_MAX, 0);
    //emit(ADD, "b", "a", "z", UINT_MAX, 1);
    emit(ASSIGN, make_var_num_expression(VAR_EXPR, make_entry("c", 0, 1, program_var, 1, GLOBAL), 0.0), make_var_num_expression(VAR_EXPR, make_entry("e", 0, 1, program_var, 2, GLOBAL), 1.50), make_var_num_expression(VAR_EXPR, make_entry("d", 0, 1, program_var, 3, GLOBAL), 1.50), UINT_MAX, 2);
    quad_table_print();

    //std::string new_tmp_name = new_temp_name();

    //std::cout << "New Temp Name: " << new_tmp_name << std::endl;

    std::cout << "-------------------------------------------------------------------" << std::endl;
    
    SymbolTableEntry symbol = new_temp_var(1, GLOBAL);
    insert(symbol.name, symbol.scope, symbol.line, symbol.space, curr_scope_offset(), symbol.type);
    print_symtable();

    std::cout << "-------------------------------------------------------------------" << std::endl;
    
    SymbolTableEntry symbol1 = new_temp_var(1, GLOBAL);
    insert(symbol1.name, symbol1.scope, symbol1.line, symbol1.space, curr_scope_offset(), symbol1.type);
    print_symtable();


    return 0;
}*/