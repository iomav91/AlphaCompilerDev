
#include "symbol_table.h"
#include "icode_managers.h"
#include <cstdlib>

#define  SIZE  1000

extern int temp_counter = 0;

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

std::vector<std::map<std::string, SymbolTableEntry*>> symbol_table_alt;
std::vector<std::map<std::string, SymbolTableEntry*>> inactive_symbol_table;


Key make_key(const std::string& symbol_name, const int& symbol_scope) {
    Key key;

    key.name = symbol_name;
    key.scope = symbol_scope;

    return key;
}

SymbolTableEntry* make_entry(const std::string& symbol_name, const
int& symbol_scope, const int& symbol_line, scope_space space, unsigned offset, SymbolType symbol_type) {
    SymbolTableEntry* entry = new SymbolTableEntry;

    entry->isActive = true;
    entry->type = symbol_type;
    entry->name = symbol_name;
    entry->scope = symbol_scope;
    entry->line = symbol_line;
    entry->space = space;
    entry->offset = offset;
    return entry;
}

int is_scope_change(int size, int scope) {
    if (scope >= size) {
        return 1;
    }
    return 0;
}

void init_vector() {
    std::map<std::string, SymbolTableEntry*> m;
    m = insert_lib_function(m);
    symbol_table_alt.push_back(m);
    return;
}

void push_vector() {
    std::map<std::string, SymbolTableEntry*> m;
    symbol_table_alt.push_back(m);
    return;
}

void push_vector_inactive(std::map<std::string, SymbolTableEntry*> m) {
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
            std::cout << symbol.first << "\t" << " (Type: [0 -> GLOBAL | 1 -> LOCAL | 2 -> FORMAL | 3 -> USERFUNC | 4 -> LIBFUNC]) " << symbol.second->type << "\t"
                  << " (Line) " << symbol.second->line << "\t" << " (Scope) " << symbol.second->scope << " (Space) " << get_space_to_word(symbol.second->space) << "\t" << " (Offset) " << symbol.second->offset 
                  << "\t" << "(State: [Active -> 1 | Inactive -> 0]) " << symbol.second->isActive << std::endl; 
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
            std::cout << symbol.first << "\t" << " (Type: [0 -> GLOBAL | 1 -> LOCAL | 2 -> FORMAL | 3 -> USERFUNC | 4 -> LIBFUNC]) " << symbol.second->type << "\t"
                  << " (Line) " << symbol.second->line << "\t" << " (Scope) " << symbol.second->scope << " (Space) " <<get_space_to_word(symbol.second->space) << "\t" << " (Offset) " << symbol.second->offset 
                  << "\t" << "(State: [Active -> 1 | Inactive -> 0]) " << symbol.second->isActive << std::endl; 
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
        symbol_table_alt.back()[name]->isActive = true;
        
    } else {
        //std::cout << "handle_func_w_1arg 2.1.2" << std::endl;
        push_vector(); // isos na prepei na kaneis push sta action tou parser
        
        symbol_table_alt.back()[name] = make_entry(name, scope, line, space, offset, type);
        inc_curr_scope_offset();
        
    }
    return;
}

SymbolTableEntry* get_symbol(std::string name, int scope) {
    //std::cout << "Symbol name is: " << name << ", " << "Symbol scope is: " << scope << std::endl; 
    for (auto& symbol : symbol_table_alt.at(scope)) {
        //std::cout << "Symbol name is: " << symbol.first << ", " << "Symbol scope is: " << symbol.second.scope << std::endl;
        if (symbol.first == name && symbol.second->scope == scope && symbol.second->isActive == true) {
            //std::cout << "Symbol is here" << std::endl;
            //make_entry(symbol.second.name, symbol.second.scope, symbol.second.line, symbol.second.space, symbol.second.offset, symbol.second.type);
            return symbol.second;
        }
    }
    return NULL;
}

SymbolTableEntry* get_symbol_inactive(std::string name, int scope) {
    //std::cout << "Symbol name is: " << name << ", " << "Symbol scope is: " << scope << std::endl; 
    for (auto& symbol : inactive_symbol_table.at(scope)) {
        //std::cout << "Symbol name is: " << symbol.first << ", " << "Symbol scope is: " << symbol.second.scope << std::endl;
        if (symbol.first == name && symbol.second->scope == scope) {
            //std::cout << "Symbol is here" << std::endl;
            //make_entry(symbol.second.name, symbol.second.scope, symbol.second.line, symbol.second.space, symbol.second.offset, symbol.second.type);
            return symbol.second;
        }
    }
    return NULL;
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
                if (symbol.first == name && symbol.second->scope == scope && symbol.second->isActive == true) {
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
            if (symbol.first == name && symbol.second->isActive == true) {
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
            if (symbol.first == name && symbol.second->isActive == true) {
                return symbol.second->scope;
            }
        }   
    }
    return -1;
}

int lookup_and_return_scope_second(std::string name, int scope) {
    for (auto map : symbol_table_alt) {
        for (const auto& symbol : map) {
            if (symbol.first == name && symbol.second->scope == scope && symbol.second->isActive == true) {
                return symbol.second->scope;
            }
        }   
    }
    return -1;
}

int is_a_func_name(std::string name) {
    for (auto map : symbol_table_alt) {
        for (const auto& symbol : map) {
            if (symbol.first == name && symbol.second->isActive == true) {
                if (symbol.second->type == USERFUNC) {
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
            if (symbol.first == name && symbol.second->scope == scope && symbol.second->isActive == false) {
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
        symbol.second->isActive = false;
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

std::map<std::string, SymbolTableEntry*> insert_lib_function(std::map<std::string, SymbolTableEntry*> m) {
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

SymbolTableEntry* new_temp_var(int line, SymbolType type) {
    std::string name = new_temp_name(temp_counter);
    temp_counter++;
    if (lookup_at_scope(name, 0)) {
        return symbol_table_alt.at(0)["name"];
    }
    SymbolTableEntry* symbol;
    if (type == GLOBAL) {
        symbol = make_entry(name, 0, line, program_var, curr_scope_offset(), type);
        insert(symbol->name, symbol->scope, symbol->line, symbol->space, symbol->offset, symbol->type);
    } else if (type == LOCAL) {
        symbol = make_entry(name, get_scope(), line, function_local, curr_scope_offset(), type);
        insert(symbol->name, symbol->scope, symbol->line, symbol->space, symbol->offset, symbol->type);
    } else {
        if (get_scope() == 0) {
            symbol = make_entry(name, get_scope(), line, program_var, curr_scope_offset(), type);
        } else {
            symbol = make_entry(name, get_scope(), line, function_local, curr_scope_offset(), type);
        }
        //symbol = make_entry(name, get_scope(), line, function_local, curr_scope_offset(), type);
        insert(symbol->name, symbol->scope, symbol->line, symbol->space, symbol->offset, symbol->type);
    }
    return symbol;
    
}

