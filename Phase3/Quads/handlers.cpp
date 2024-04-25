#include "handlers.h"

extern int is_in_function_mode;
extern int is_in_block_mode;

//std::list<std::string> argsList;

std::vector<int> blocks_prec;

int func_anonym_counter = 0;

void push_blocks_prec(int n) {
    blocks_prec.push_back(n);
}

void handle_assign_expr(std::string name, int scope) {
    //std::cout << "Type is " << type << std::endl;
    if (lookup(name)) {
        if (is_a_func_name(name) && lookup_and_return_scope(name) == scope) {
            std::cout << "Error symbol " << name << " is a function it cannot be assigned" << std::endl;
            return;
        } 
    }
    return;
}

void handle_id(std::string name, int scope, int line) {
    if (!lookup(name)) {
    	//std::cout << "false Scope " << scope << std::endl;
        
        if (scope == 0) {
            insert(name, scope, line, GLOBAL);
            
        } else {
            insert(name, scope, line, LOCAL);
        }
    } else {
        //std::cout << "LOOK HERE " << "" << std::endl;
        //std::cout << "Scope of symbol in the table with " << name << " is " << lookup_and_return_scope(name) << " and Current Scope is " << scope << std::endl;
    	/*if (lookup_and_return_scope(name) < scope && lookup_and_return_scope(name) != 0 && is_in_function_mode == 1 && (is_in_function_mode >= is_in_block_mode)) {
            std::cout << "LOOK HERE 1" << std::endl;
    		std::cout << "You cannot access symbol " << name << " is scope " << lookup_and_return_scope(name) << std::endl;
            return;
    	} 

        if (lookup_and_return_scope(name) == scope && lookup_and_return_scope(name) != 0 && is_a_func_name(name) == 1) {
            std::cout << "LOOK HERE 2" << std::endl;
            std::cout << "Error: function with name " << name << " used as an l-value" << std::endl;   
            return;
        }*/

        for (int i = get_scope()-1; i >= 1; i--) {
            //std::cout << "Current Scope is " << i << std::endl;
            //std::cout << "Blocks Prec " << blocks_prec.back() << std::endl;   
            if (lookup_and_return_scope_second(name, i) < scope && lookup_and_return_scope_second(name, i) != -1 && blocks_prec.back() == 2) {
                //std::cout << "LOOK HERE 3 " <<  scope << lookup_and_return_scope_second(name, i) << std::endl;
                std::cout << "Error: " << name << " :: " << name << " at line " << line << " (local) not accessible" << std::endl;
                return;
            }

            /*if (lookup_and_return_scope_second(name, i) < scope && lookup_and_return_scope_second(name, i) != -1) {
                std::cout << "HANDLE_ID !!!!" << lookup_and_return_scope_second(name, i) << std::endl;
                std::cout << "Error: " << name << " :: " << name << " (local) not accessible" << std::endl;
                return;   
            }

            /*if (lookup_and_return_scope_second(name, i) < scope && lookup_and_return_scope_second(name, i) != -1 && is_in_function_mode == 1) {
                std::cout << "HANDLE_ID !!!!!" << lookup_and_return_scope_second(name, i) << std::endl;
                std::cout << "Error: " << name << " :: " << name << " (local) not accessible" << std::endl;
                return;   
            }*/

        }
    }
    return;
}

void handle_local_id(std::string name, int scope, int line) {

    /*if (!lookup_at_scope(name, get_scope())) {
    	std::cout << "false" << std::endl;
        insert(name, scope, line, LOCAL);
    }*/
    //std::cout << "Vector Size: " << get_vector_size() << std::endl;
    //if (get_vector_size() > scope) {
        //std::cout << "Vector Size: " << get_vector_size() << std::endl;
        if (!lookup_at_scope(name, get_scope())) {
            //std::cout << "HERE AFTER get_vector_size > scope" << std::endl;
            /*if (is_a_func_name(name) == 1 && scope != 0) {
                std::cout << "Error: " << name << ": (local) " << name << " is not accessible" << std::endl;
                return;
            }*/

            if (is_a_lib_func(name) == 1 && scope != 0) {
                std::cout << "Error: Shadowing of library function " << name << " is not allowed" << std::endl;
                return;
            }
            insert(name, scope, line, LOCAL);
        } else {
            /*if (lookup_and_return_scope(name) < scope && lookup_and_return_scope(name) != 0 && is_in_function_mode == 1) {
                std::cout << "You cannot access symbol" << name << " is scope " << lookup_and_return_scope(name) << std::endl;
                return;
            }
            /*if (is_a_func_name(name) == 1 && scope != 0) {
                std::cout << "Error: " << name << ": (local) " << name << " is not accessible" << std::endl;
                return;
            }*/

            /*if (is_a_lib_func(name) == 1 && scope != 0) {
                std::cout << "Error: Shadowing of library function " << name << " is not allowed" << std::endl;
                return;
            }*/
            std::cout << "Symbol " << name << " is a reference" << std::endl;
            return;
        }
    /*} else {
        std::cout << "Else Vector Size: " << get_vector_size() << std::endl;
        if (is_a_func_name(name) == 1) {
            std::cout << "Error: " << name << ": (local) " << name << " is not accessible" << std::endl;
            return;
        }

        if (is_a_lib_func(name) == 1) {
            std::cout << "Error: Shadowing of library function " << name << " is not allowed" << std::endl;
            return;
        }
        //push_vector();
        insert(name, scope, line, LOCAL);
        
    }*/
    return;	
}

void handle_global_access_id(std::string name) {
    if (!lookup_at_scope(name, 0)) {
        std::cout << "Error: Symbol " << name << " cannot be accessed in the global scope" << std::endl;
    } else {
        std::cout << "Ok Symbol is referred at global symbol " << name << std::endl;
    }
    return;
}

void handle_funcdef_w_name(std::string name, int scope, int line) {
    //std::cout << "Scope in handle_funcdef_w_name " << scope << std::endl;
    if (!lookup_at_scope(name, scope)) {
        //std::cout << "THERE !!!" << std::endl;
        if (is_a_lib_func(name) == 1) {
            std::cout << "Error: Shadowing of library function " << name << " is not allowed" << std::endl;
            return;
        }
        insert(name, scope, line, USERFUNC);
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

void handle_funcdef_anonym_name(int scope, int line) {
    std::string name = func_name_generator();
    ++func_anonym_counter;
    if (!lookup_at_scope(name, get_scope())) {
        insert(name, scope, line, USERFUNC);
    } else {
        std::cout << "Error: Symbol " << name << " is in the symbol table" << std::endl;
        return;
    }
    return;
}

void handle_func_w_1arg(std::string name, int scope, int line) {
    //std::cout << "Scope is: " << get_scope() << std::endl;
    if (get_vector_size() > scope) {
        //std::cout << "handle_func_w_1arg 2" << std::endl;
        if (!lookup_at_scope(name, get_scope()) && is_a_lib_func(name) == 0) {
            //std::cout << "handle_func_w_1arg 2.1" << std::endl;
            insert(name, scope, line, FORMAL);
            return;
        } else {
            //std::cout << "handle_func_w_1arg 3" << std::endl;
            std::cout << "Error: Symbol " << name << " is in the symbol table" << std::endl;
            return;
        }
    } else {
        //std::cout << "handle_func_w_1arg 1" << std::endl;
        push_vector();
        insert(name, scope, line, FORMAL);
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
   return ("__t"+std::to_string(func_anonym_counter));
}



