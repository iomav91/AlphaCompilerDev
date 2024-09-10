#include "quads_univ.h"
#include "symbol_table.h"

/*struct SymbolTableEntry {
    bool isActive;
    enum SymbolType type;
    std::string name;
    int scope;
    int line;
};*/

// Phase 3

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
    expression*         result;
    expression*         arg1;
    expression*         arg2;
    unsigned            label;
    unsigned            line;
};

quad make_quad(enum iopcode op, expression* result, expression* arg1, 
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

expression* make_var_num_expression(expr_value type, SymbolTableEntry symbol, double num_const) {
    expression* new_expression = (expression*)malloc(sizeof(expression));

    new_expression->type = type;
    new_expression->symbol = symbol;
    new_expression->num_const = num_const;

    return new_expression;
}

std::vector<struct quad> quad_table;

void emit(enum iopcode op, expression* result, expression* arg1, 
        expression* arg2, unsigned label, unsigned line) {

    quad new_quad = make_quad(op, result, arg1, arg2, label, line);  
    quad_table.push_back(new_quad);
}

void quad_table_print() {
    std::cout << "INSTR. LINE" << "\t" << "OPERATION" << "\t" << "RESULT" << "\t\t" << "Arg 1" << "\t\t" << "Arg 2" << std::endl;
    for (auto quad : quad_table) {
        switch (quad.op) {
            case 0:
                std::cout << quad.line << "\t\t" << "ASSIGN" << "\t\t" << quad.result->symbol.name << "\t\t" << quad.arg1->symbol.name << "\t\t" << quad.arg2->symbol.name << std::endl;
                break;
            case 1:
                std::cout << quad.line << "\t\t" << "ADD" << "\t\t" << quad.result->symbol.name << "\t\t" << quad.arg1->symbol.name << "\t\t" << quad.arg2->symbol.name << std::endl;
                break;
            case 2:
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
                break;
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

SymbolTableEntry new_temp_var(int line, enum SymbolType type) {
    std::string name = new_temp_name();
    temp_counter++;
    if (!lookup_at_scope(name, 0)) {
        SymbolTableEntry symbol = make_entry(name, 0, line, type);
        insert
        return symbol;
    }
    return get_symtable().at(0)[name];
}

int main() {
    init_vector();
    //SymbolTableEntry entry1 = make_entry("x", 0, 0, GLOBAL);
    emit(ASSIGN, make_var_num_expression(VAR_EXPR, make_entry("x", 0, 0, GLOBAL), 0.0), make_var_num_expression(VAR_EXPR, make_entry("y", 0, 0, GLOBAL), 1.50), make_var_num_expression(VAR_EXPR, make_entry("z", 0, 0, GLOBAL), 1.50), UINT_MAX, 0);
    emit(ADD, make_var_num_expression(VAR_EXPR, make_entry("z", 0, 1, GLOBAL), 0.0), make_var_num_expression(VAR_EXPR, make_entry("a", 0, 1, GLOBAL), 1.50), make_var_num_expression(VAR_EXPR, make_entry("b", 0, 1, GLOBAL), 1.50), UINT_MAX, 1);
    emit(SUB, make_var_num_expression(VAR_EXPR, make_entry("c", 0, 1, GLOBAL), 0.0), make_var_num_expression(VAR_EXPR, make_entry("e", 0, 1, GLOBAL), 1.50), make_var_num_expression(VAR_EXPR, make_entry("d", 0, 1, GLOBAL), 1.50), UINT_MAX, 2);
    quad_table_print();

    //std::string new_tmp_name = new_temp_name();

    //std::cout << "New Temp Name: " << new_tmp_name << std::endl;

    SymbolTableEntry symbol = new_temp_var(1, GLOBAL);
    //print_symtable();
    return 0;
}