#include "handlers.h"

extern int is_in_function_mode;
extern int is_in_block_mode;
extern int is_a_member_access;
extern int is_func_call;

//std::list<std::string> argsList;

std::vector<int> blocks_prec;

int func_anonym_counter = 0;

void push_blocks_prec(int n) {
    blocks_prec.push_back(n);
}

/*void pop_blocks_prec() {
    blocks_prec.pop_back();
}*/

int handle_assign_expr(std::string name) {
    //std::cout << "Type is " << type << std::endl;
    if (lookup(name)) {
        /*if ((is_a_func_name(name) || is_a_lib_func(name)) && lookup_and_return_scope(name) == scope && is_a_member_access == 0) {
            std::cout << "Error symbol " << name << " is a function it cannot be assigned" << std::endl;
            return;
        }*/
        if (is_a_func_name(name) || is_a_lib_func(name)) {
            std::cout << "Error symbol " << name << " is a function it cannot be assigned" << std::endl;
            return 1;
        }
    }
    return 0;
}

void handle_db_plus_expr(std::string name) {
    if (lookup(name)) {
        /*if ((is_a_func_name(name) || is_a_lib_func(name)) && lookup_and_return_scope(name) == scope && is_a_member_access == 0) {
            std::cout << "Error symbol " << name << " is a function it cannot be assigned" << std::endl;
            return;
        }*/
        if (is_a_func_name(name) || is_a_lib_func(name)) {
            std::cout << "Error symbol " << name << " is a function it cannot be " << std::endl;
            return;
        }
    }
    return;
}

void handle_plus_db_expr(std::string name) {
    if (lookup(name)) {
        /*if ((is_a_func_name(name) || is_a_lib_func(name)) && lookup_and_return_scope(name) == scope && is_a_member_access == 0) {
            std::cout << "Error symbol " << name << " is a function it cannot be assigned" << std::endl;
            return;
        }*/
        if (is_a_func_name(name) || is_a_lib_func(name)) {
            std::cout << "Error symbol " << name << " is a function it cannot be assigned" << std::endl;
            return;
        }
    }
    return;
}

void handle_db_minus_expr(std::string name) {
    if (lookup(name)) {
        /*if ((is_a_func_name(name) || is_a_lib_func(name)) && lookup_and_return_scope(name) == scope && is_a_member_access == 0) {
            std::cout << "Error symbol " << name << " is a function it cannot be assigned" << std::endl;
            return;
        }*/
        if (is_a_func_name(name) || is_a_lib_func(name)) {
            std::cout << "Error symbol " << name << " is a function it cannot be assigned" << std::endl;
            return;
        }
    }
    return;
}

void handle_minus_db_expr(std::string name) {
    if (lookup(name)) {
        /*if ((is_a_func_name(name) || is_a_lib_func(name)) && lookup_and_return_scope(name) == scope && is_a_member_access == 0) {
            std::cout << "Error symbol " << name << " is a function it cannot be assigned" << std::endl;
            return;
        }*/
        if (is_a_func_name(name) || is_a_lib_func(name)) {
            std::cout << "Error symbol " << name << " is a function it cannot be assigned" << std::endl;
            return;
        }
    }
    return;
}

int has_access(std::string name) {
    for (int i = get_scope(); i>=0; i--) {
        std::cout << "i = " << i << std::endl;
        
        if (lookup_at_scope(name,i)) {
            if (lookup_and_return_scope(name) != get_scope() && lookup_and_return_scope(name) != 0) {
                if (!is_a_func_name(name) && !is_a_lib_func(name)) {
                    if (is_in_function_mode <= get_scope() && get_scope() != 1) {
                        return 0;
                    }
                }
            } 
        }
    }
    return 1;
}

void handle_id(std::string name, int scope, scope_space space, unsigned offset, int line) {
    if (!lookup(name)) {
        //std::cout << "false Scope " << scope << std::endl;
        
        if (scope == 0) {
            insert(name, scope, line, space, offset, GLOBAL);
            
        } else {
            insert(name, scope, line, space, offset, LOCAL);
        }
    } else {
        
        std::cout << "HANDLE ID is already in symbol table" << std::endl;

        if (has_access(name)) {

        } else {
            std::cout << "Error: " << name << " :: " << name << " at line " << line << " (local) not accessible" << std::endl;
            return;
        }
    }
    return;
}

void handle_local_id(std::string name,  int scope, scope_space space, unsigned offset, int line) {

    if (!lookup_at_scope(name, get_scope())) {    
        if (is_a_lib_func(name)) {
            std::cout << "Error: Shadowing of library function " << name << " is not allowed" << std::endl;
            return;
        }
        if (scope != 0) {
            insert(name,scope,line, space, offset,LOCAL);
        } else {
            insert(name, scope, line, space, offset, GLOBAL);
        }

    } else {
        
        std::cout << "Symbol " << name << " is a reference" << std::endl;
        return;
        
    }
    return; 
}

void handle_global_access_id(std::string name) {
    if (!lookup_at_scope(name, 0)) {
        std::cout << "Error: Symbol " << name << " cannot be accessed in the global scope" << std::endl;
        return;
    } else {
        std::cout << "Ok Symbol is referred at global symbol " << name << std::endl;
        return;
    }
    //return;
}

void handle_funcdef_w_name(std::string name, int scope, scope_space space, unsigned offset, int line) {
    std::cout << "Scope in handle_funcdef_w_name " << scope << std::endl;
    if (!lookup_at_scope(name, scope)) {
        std::cout << "handle funcdef with name" << name << std::endl;
        if (is_a_lib_func(name) == 1) {
            std::cout << "Error: Shadowing of library function " << name << " is not allowed" << std::endl;
            return;
        }
        insert(name, scope, line, space, offset, USERFUNC);
    } else {
        if (is_a_lib_func(name) == 1) {
            std::cout << "Error: Shadowing of library function " << name << " is not allowed" << std::endl;
            return;
        }
        std::cout << "Error: Symbol " << name << " is in the symbol table" << std::endl;
        return;
    }
    return;
}

void handle_funcdef_anonym_name(int scope, scope_space space, unsigned offset, int line) {
    std::string name = func_name_generator();
    ++func_anonym_counter;
    /*if (!lookup_at_scope(name, get_scope())) {
        insert(name, scope, line, USERFUNC);
    } else {
        std::cout << "Error: Symbol " << name << " is in the symbol table" << std::endl;
        return;
    }*/

    if (!lookup_at_scope(name, scope)) {
        //std::cout << "THERE !!!" << std::endl;
        if (is_a_lib_func(name) == 1) {
            std::cout << "Error: Shadowing of library function " << name << " is not allowed" << std::endl;
            return;
        }
        insert(name, scope, line, space, offset, USERFUNC);
    } else {
        if (is_a_lib_func(name) == 1) {
            std::cout << "Error: Shadowing of library function " << name << " is not allowed" << std::endl;
            return;
        }
        std::cout << "Error: Symbol " << name << " is in the symbol table" << std::endl;
        return;
    }
    return;
}

void handle_func_w_1arg(std::string name, int scope, scope_space space, unsigned offset, int line) {
    //std::cout << "Scope is: " << get_scope() << std::endl;
    if (get_vector_size() > scope) {
        std::cout << "handle_func_w_1arg 2" << std::endl;

        if (!lookup_at_scope(name, get_scope())) {
            if (!is_a_lib_func(name)) {
                insert(name, scope, line, space, offset, FORMAL);
                return;
            } else {
                std::cout << "Error: Shadowing of library function " << name << " is not allowed" << std::endl;
                return;
            }
        } else {
            if (is_a_lib_func(name)) {
                std::cout << "Error: Shadowing of library function " << name << " is not allowed" << std::endl;
                return;
            }
            std::cout << "Error: Symbol " << name << " is in the symbol table" << std::endl;
            return;
        }

        /*if (!lookup_at_scope(name, get_scope()) && is_a_lib_func(name) == 0) {
            //std::cout << "handle_func_w_1arg 2.1" << std::endl;
            insert(name, scope, line, FORMAL);
            return;
        } else {
            std::cout << "handle_func_w_1arg 3" << std::endl;
            if (is_a_lib_func(name) == 1) {
                std::cout << "Error: Shadowing of library function " << name << " is not allowed" << std::endl;
                return;
            }
            std::cout << "Error: Symbol " << name << " is in the symbol table" << std::endl;
            return;
        }*/
    } else {
        std::cout << "handle_func_w_1arg 1" << std::endl;
        if (is_a_lib_func(name) == 1) {
            std::cout << "Error: Shadowing of library function " << name << " is not allowed" << std::endl;
            return;
        }
        push_vector();
        insert(name, scope, line, space, offset, FORMAL);
    }
    return;
}

void handle_expression(std::string name1, std::string name2, int scope, int line) {
    if (!lookup_at_scope(name1, get_scope())) {
        std::cout << "Error: Symbol " << name1 << " is in the symbol table" << std::endl;
    }

    if (!lookup_at_scope(name2, get_scope())) {
        std::cout << "Error: Symbol " << name2 << " is in the symbol table" << std::endl;
    }

    return;
}

void handle_funcdef_block_end(int scope) {
    if (scope != 0) {
        hide();
    }
}

void handle_block_end(int scope) {
    if (scope != 0) {
        hide();
    }
}

int is_a_lib_func(std::string name) {

    if (name == "print" || name == "input" || name == "objectmemberkeys" || name == "objecttotalmembers" || name == "objectcopy" || name == "totalarguments" 
       || name == "argument" || name == "typeof" || name == "strtonum" || name == "sqrt" || name == "cos" || name == "sin") {
        return 1;
    }
    return 0;
}

std::string func_name_generator() {
   return ("__func" + std::to_string(func_anonym_counter));
}


