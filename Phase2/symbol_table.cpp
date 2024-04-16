#include "symbol_table.h"
#include <cstdlib>

#define  SIZE  1000

struct Key {
    std::string name;
    int scope;
};

struct SymbolTableEntry {
    bool isActive;
    enum SymbolType type;
    std::string name;
    int scope;
    int line;
};

std::vector<std::map<std::string, SymbolTableEntry>> symbol_table_alt;
std::vector<std::map<std::string, SymbolTableEntry>> inactive_symbol_table;

Key make_key(const std::string& symbol_name, const int& symbol_scope) {
    Key key;

    key.name = symbol_name;
    key.scope = symbol_scope;

    return key;
}

SymbolTableEntry make_entry(const std::string& symbol_name, const
int& symbol_scope, const int& symbol_line, SymbolType symbol_type) {
    SymbolTableEntry entry;

    entry.isActive = true;
    entry.type = symbol_type;
    entry.name = symbol_name;
    entry.scope = symbol_scope;
    entry.line = symbol_line;

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
}

void push_vector() {
    std::map<std::string, SymbolTableEntry> m;
    symbol_table_alt.push_back(m);
}

void push_vector_inactive(std::map<std::string, SymbolTableEntry> m) {
    inactive_symbol_table.push_back(m);
}

void print_symtable() {
    for (auto map : symbol_table_alt) {
        for (const auto& symbol : map) {
            std::cout << symbol.first << "\t" << " (Type: [0 -> GLOBAL | 1 -> LOCAL | 2 -> FORMAL | 3 -> USERFUNC | 4 -> LIBFUNC]) " << symbol.second.type << "\t"
                  << " (Line) " << symbol.second.line << "\t" << " (Scope) " << symbol.second.scope << "\t"
<< "(State: [Active -> 1 | Inactive -> 0]) " << symbol.second.isActive << std::endl; 
        } 
    }
}

void print_symtable_inactive() {
    for (auto map : inactive_symbol_table) {
        for (const auto& symbol : map) {
            std::cout << symbol.first << "\t" << " (Type: [0 -> GLOBAL | 1 -> LOCAL | 2 -> FORMAL | 3 -> USERFUNC | 4 -> LIBFUNC]) " << symbol.second.type << "\t"
                  << " (Line) " << symbol.second.line << "\t" << " (Scope) " << symbol.second.scope << "\t"
<< "(State: [Active -> 1 | Inactive -> 0]) " << symbol.second.isActive << std::endl; 
        } 
    }
}

void insert(std::string name, int scope, int line, SymbolType type) {
    if (is_scope_change(symbol_table_alt.size(), scope) == 0) {
        //std::cout << "LOCAL" << std::endl;
        //std::cout << "handle_func_w_1arg 2.1.1" << std::endl;
        symbol_table_alt.back()[name] = make_entry(name, scope, line, type);
        //symbol_table_alt.back()[name].isActive = true;
        
    } else {
        //std::cout << "THERE !!!!" << std::endl;
        push_vector(); // isos na prepei na kaneis push sta action tou parser
        
        symbol_table_alt.back()[name] = make_entry(name, scope, line, type);
        
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
    m["print"] = make_entry("print", 0, -1, LIBFUNC);
    m["input"] = make_entry("input", 0, -1, LIBFUNC);
    m["objectmemberkeys"] = make_entry("objectmemberkeys", 0, -1, LIBFUNC);
    m["objecttotalmembers"] = make_entry("objecttotalmembers", 0, -1, LIBFUNC);
    m["objectcopy"] = make_entry("objectcopy",0, -1, LIBFUNC);
    m["totalarguments"] = make_entry("totalarguments", 0, -1, LIBFUNC);
    m["argument"] = make_entry("argument", 0, -1, LIBFUNC);
    m["typeof"] = make_entry("typeof", 0, -1, LIBFUNC);
    m["strtonum"] = make_entry("strtonum", 0, -1, LIBFUNC);
    m["sqrt"] = make_entry("sqrt", 0, -1, LIBFUNC);
    m["cos"] = make_entry("cos", 0, -1, LIBFUNC);
    m["sin"] = make_entry("sin", 0, -1, LIBFUNC);

    return m;
}

/*void print_symtable() {
    for (const auto& map : symbol_table.top()) {
        std::cout << map.first << " " << map.second.scope << " "
                  << map.second.line << " " << map.second.type << " "
<< map.second.isActive << std::endl;
    }
}*/

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

    return 0;
}*/