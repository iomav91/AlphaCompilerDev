#include "symbol_table.h"
#include <cstdlib>

#define  SIZE  1000

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
    int               line;
};

struct expression {
    enum expr_value     type;
    SymbolTableEntry    symbol;
    expression*         index;
    double              num_const; 
    std::string         str_const;
    int                 bool_const;
    //expression*         next_expr;         
};

struct quad {
    enum iopcode        op;
    std::string         result;
    std::string         arg1;
    std::string         arg2;
    unsigned            label;
    unsigned            line;
};

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
}

void push_vector_inactive(std::map<std::string, SymbolTableEntry> m) {
    inactive_symbol_table.push_back(m);
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

/*extern int scope_counter;

int get_scope() {
    return scope_counter;
}*/

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

quad make_quad(iopcode op, std::string result, std::string arg1, 
               std::string arg2, unsigned label, unsigned line) {
    quad new_quad;

    new_quad.op = op;
    new_quad.result = result;
    new_quad.arg1 = arg1;
    new_quad.arg2 = arg2;
    new_quad.label = label;
    new_quad.line = line;

    return new_quad;
}

expression* make_var_num_expression(expr_value type, SymbolTableEntry symbol, double num_const) {
    expression* new_expression = (expression*)malloc(sizeof(expression));

    new_expression->type = type;
    new_expression->symbol = symbol;
    new_expression->num_const = num_const;

    return new_expression;
}

void emit(iopcode op, std::string result, std::string arg1, 
        std::string arg2, unsigned label, unsigned line) {
    std::cout << "HERE" << std::endl;
    quad_table.push_back(make_quad(op, result, arg1, arg2, label, line));
}

void quad_table_print() {
    std::cout << "INSTR. LINE" << "\t" << "OPERATION" << "\t" << "RESULT" << "\t\t" << "Arg 1" << "\t\t" << "Arg 2" << std::endl;
    for (auto& quad : quad_table) {
        switch (quad.op) {
            case 0:
                std::cout << quad.line << "\t\t" << "ASSIGN" << "\t\t" << quad.result << "\t\t" << quad.arg1 << "\t\t" << quad.arg2 << std::endl;
                break;
            case 1:
                std::cout << quad.line << "\t\t" << "ADD" << "\t\t" << quad.result << "\t\t" << quad.arg1 << "\t\t" << quad.arg2 << std::endl;
                break;
            /*case 2:
                std::cout << quad.line << "\t\t" << "SUB" << "\t\t" << quad.result->symbol.name << "\t\t" << quad.arg1->symbol.name << "\t\t" << quad.arg2->symbol.name << std::endl;
                break;
            case 3:
                std::cout << quad.line << "\t\t" << "MUL" << "\t\t" << quad.result->symbol.name << "\t\t" << quad.arg1->symbol.name << "\t\t" << quad.arg2->symbol.name << std::endl;
                break;
            case 4:
                std::cout << quad.line << "\t\t" << "DIV" << "\t\t" << quad.result->symbol.name << "\t\t" << quad.arg1->symbol.name << "\t\t" << quad.arg2->symbol.name << std::endl;
                break;
            case 5:
                std::cout << quad.line << "\t\t" << "MOD" << "\t\t" << quad.result->symbol.name << "\t\t" << quad.arg1->symbol.name << "\t\t" << quad.arg2->symbol.name << std::endl;
                break;
            case 6:
                std::cout << quad.line << "\t\t" << "UMINUS" << "\t\t" << quad.result->symbol.name << "\t\t" << quad.arg1->symbol.name << "\t\t" << quad.arg2->symbol.name << std::endl;
                break;
            case 7:
                std::cout << quad.line << "\t\t" << "AND" << "\t\t" << quad.result->symbol.name << "\t\t" << quad.arg1->symbol.name << "\t\t" << quad.arg2->symbol.name << std::endl;
                break;
            case 8:
                std::cout << quad.line << "\t\t" << "OR" << "\t\t" << quad.result->symbol.name << "\t\t" << quad.arg1->symbol.name << "\t\t" << quad.arg2->symbol.name << std::endl;
                break;
            case 9:
                std::cout << quad.line << "\t\t" << "NOT" << "\t\t" << quad.result->symbol.name << "\t\t" << quad.arg1->symbol.name << "\t\t" << quad.arg2->symbol.name << std::endl;
                break;
            case 10:
                std::cout << quad.line << "\t\t" << "IF EQUAL" << "\t\t" << quad.result->symbol.name << "\t\t" << quad.arg1->symbol.name << "\t\t" << quad.arg2->symbol.name << std::endl;
                break;
            case 11:
                std::cout << quad.line << "\t\t" << "IF NOT EQUAL" << "\t\t" << quad.result->symbol.name << "\t\t" << quad.arg1->symbol.name << "\t\t" << quad.arg2->symbol.name << std::endl;
                break;
            case 12:
                std::cout << quad.line << "\t\t" << "IF LESS EQUAL" << "\t\t" << quad.result->symbol.name << "\t\t" << quad.arg1->symbol.name << "\t\t" << quad.arg2->symbol.name << std::endl;
                break;
            case 13:
                std::cout << quad.line << "\t\t" << "IF GREATER EQUAL" << "\t\t" << quad.result->symbol.name << "\t\t" << quad.arg1->symbol.name << "\t\t" << quad.arg2->symbol.name << std::endl;
                break;
            case 14:
                std::cout << quad.line << "\t\t" << "IF LESS" << "\t\t" << quad.result->symbol.name << "\t\t" << quad.arg1->symbol.name << "\t\t" << quad.arg2->symbol.name << std::endl;
                break;
            case 15:
                std::cout << quad.line << "\t\t" << "IF GREATER" << "\t\t" << quad.result->symbol.name << "\t\t" << quad.arg1->symbol.name << "\t\t" << quad.arg2->symbol.name << std::endl;
                break;
            case 16:
                std::cout << quad.line << "\t\t" << "CALL" << "\t\t" << quad.result->symbol.name << "\t\t" << quad.arg1->symbol.name << "\t\t" << quad.arg2->symbol.name << std::endl;
                break;
            case 17:
                std::cout << quad.line << "\t\t" << "PARAM" << "\t\t" << quad.result->symbol.name << "\t\t" << quad.arg1->symbol.name << "\t\t" << quad.arg2->symbol.name << std::endl;
                break;
            case 18:
                std::cout << quad.line << "\t\t" << "RETURN" << "\t\t" << quad.result->symbol.name << "\t\t" << quad.arg1->symbol.name << "\t\t" << quad.arg2->symbol.name << std::endl;
                break;
            case 19:
                std::cout << quad.line << "\t\t" << "GETRETVAL" << "\t\t" << quad.result->symbol.name << "\t\t" << quad.arg1->symbol.name << "\t\t" << quad.arg2->symbol.name << std::endl;
                break;
            case 20:
                std::cout << quad.line << "\t\t" << "FUNCSTART" << "\t\t" << quad.result->symbol.name << "\t\t" << quad.arg1->symbol.name << "\t\t" << quad.arg2->symbol.name << std::endl;
                break;
            case 21:
                std::cout << quad.line << "\t\t" << "FUNCEND" << "\t\t" << quad.result->symbol.name << "\t\t" << quad.arg1->symbol.name << "\t\t" << quad.arg2->symbol.name << std::endl;
                break;
            case 22:
                std::cout << quad.line << "\t\t" << "TABLECREATE" << "\t\t" << quad.result->symbol.name << "\t\t" << quad.arg1->symbol.name << "\t\t" << quad.arg2->symbol.name << std::endl;
                break;
            case 23:
                std::cout << quad.line << "\t\t" << "TABLEGETELEM" << "\t\t" << quad.result->symbol.name << "\t\t" << quad.arg1->symbol.name << "\t\t" << quad.arg2->symbol.name << std::endl;
                break;
            case 24:
                std::cout << quad.line << "\t\t" << "TABLESETELEM" << "\t\t" << quad.result->symbol.name << "\t\t" << quad.arg1->symbol.name << "\t\t" << quad.arg2->symbol.name << std::endl;
                break;*/
            default:
                break;
        }
        
    }
}

std::string new_temp_name() {
    //temp_counter++;
    return ("__t"+std::to_string(temp_counter));
}

void reset_temp_counter() {
    temp_counter = 0;
}

SymbolTableEntry new_temp_var(int line, SymbolType type) {
    std::string name = new_temp_name();
    temp_counter++;
    if (lookup_at_scope(name, 0)) {
        return symbol_table_alt.at(0)["name"];
    }
    SymbolTableEntry symbol = make_entry(name, 0, line, program_var, curr_scope_offset(), type);
    return symbol;
    
}

enum scope_space curr_scope_space() {
    if (scope_space_counter == 1) {
        return program_var;
    } else if (scope_space_counter % 2 == 0) {
        return formal_arg;
    } else
        return function_local; 
}

unsigned curr_scope_offset() {
    switch (curr_scope_space()) {
        case program_var: return program_var_offset;
        case function_local: return function_local_offset;
        case formal_arg: return formal_arg_offset;
        default: assert(0); 
    }
}

void inc_curr_scope_offset() {
    switch (curr_scope_space()) {
        case program_var: ++program_var_offset; break;
        case function_local: ++function_local_offset; break;
        case formal_arg: ++formal_arg_offset; break;
        default: assert(0); 
    }
}

void enter_scope_space() {
    ++scope_space_counter;
}

void exit_scope_space() {
    assert(scope_space_counter>1); 
    --scope_space_counter;
}

int main() {
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

    print_symtable();*/

    //SymbolTableEntry entry1 = make_entry("x", 0, 0, GLOBAL);

    init_vector();

    std::cout << symbol_table_alt.size() << std::endl;

    print_symtable();

    insert("x", 0, -1, program_var, curr_scope_offset(), GLOBAL);

    std::cout << "NEW SYMBOL TABLE DISPLAY" << std::endl;

    print_symtable();

    std::cout << "-------------------------------------------------------------------" << std::endl;

    emit(ASSIGN, "a", "x", "z", UINT_MAX, 0);
    emit(ADD, "b", "a", "z", UINT_MAX, 1);
    //emit(SUB, make_var_num_expression(VAR_EXPR, make_entry("c", 0, 1, GLOBAL), 0.0), make_var_num_expression(VAR_EXPR, make_entry("e", 0, 1, GLOBAL), 1.50), make_var_num_expression(VAR_EXPR, make_entry("d", 0, 1, GLOBAL), 1.50), UINT_MAX, 2);
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
}