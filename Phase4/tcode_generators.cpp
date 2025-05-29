#include "tcode_generators.h"
#include "icode_managers.h"
#include <iostream>
#include <string>
#include <vector>
#include "symbol_table.h"
#include <fstream>
#include <cstdint>

extern std::vector<quad> quad_table;
std::vector<instruction> instruction_table;

typedef void (*generator_func_t)(quad*);

std::vector<std::string> conststrings_vector;

unsigned consts_conststrings(std::string str) {
    unsigned index = 0;
    for (const auto& str : conststrings_vector) {
        if (str == str) {
            return index;
        }
        ++index;
    }
    conststrings_vector.push_back(str);
    return index;
}

std::vector<double> constsnumbers_vector;

unsigned consts_constnumbers(unsigned num) {
    std::cout << "constsnumbers NUM: " << num << std::endl;
    unsigned index = 0;
    for (const auto& val : constsnumbers_vector) {
        if (val == num) {
            return index;
        }
        ++index;
    }
    constsnumbers_vector.push_back(num);
    return index;
}

std::vector<std::string> libfuncs_vector;

unsigned libfuncs_newused(std::string str) {
    unsigned index = 0;
    for (const auto& libfunc : libfuncs_vector) {
        if (libfunc == str) {
            return index;
        }
        ++index;
    }
    libfuncs_vector.push_back(str);
    return index;
}

std::vector<expression*> userfuncs_vector;

unsigned userfuncs_newfunc(expression* expr) {
    unsigned index = 0;
    expression* userfunc = new expression;
    for (const auto& userfunc : userfuncs_vector) {
        if (userfunc->symbol->name == expr->symbol->name) {
            return index;
        }
        //userfunc->symbol = expr->symbol;
        //userfunc->symbol->i_address = expr->symbol->i_address;
        //userfunc->symbol->total_locals = expr->symbol->total_locals;
        //userfunc->symbol->func_name = expr->symbol->func_name;
        //userfunc->symbol->isActive = expr->symbol->isActive;
        //userfunc->symbol->space = expr->symbol->space;
        //userfunc->symbol->offset = expr->symbol->offset;
        ++index;
    }
    userfuncs_vector.push_back(expr);
    return index;
}

unsigned retrieve_quad_index() {
    unsigned quad_index = 0;
    while ((quad_index = current_processed_quad()) < quad_table.size()) {
        quad* current_quad = &quad_table[quad_index];
        // Process the current quad
        std::cout << "Processing quad at index: " << quad_index << std::endl;
    }
    return quad_index;
}

unsigned current_processed_quad() {
    static unsigned current_quad_index = 0;

    if (current_quad_index >= quad_table.size()) {
        //std::cerr << "Error: Attempted to access a quad beyond the quad table size." << std::endl;
        return current_quad_index; // Return the current index even if it exceeds the size
    }

    return current_quad_index++;
}

generator_func_t generators[] = {
    generate_ASSIGN,
    generate_ADD,
    generate_SUB,
    generate_MUL,
    generate_DIV,
    generate_MOD,
    generate_UMINUS,
    generate_AND,
    generate_OR,
    generate_NOT,
    generate_IF_EQ,
    generate_IF_N_EQ,
    generate_IF_L_EQ,
    generate_IF_GR_EQ,
    generate_IF_L,
    generate_IF_GR,
    generate_CALL,
    generate_PARAM,
    generate_RET,
    generate_GETRETVAL,
    generate_FUNCSTART,
    generate_FUNCEND,
    generate_TABLECREATE,
    generate_TABLEGETELEM,
    generate_TABLESETELEM,
    generate_JUMP
};

void emit(instruction* instr) {
    instruction_table.push_back(*instr);
}

unsigned next_instruction_label() {
    return instruction_table.size(); 
}

instruction make_instruction(vmopcode op, vmarg* res, vmarg* arg1, vmarg* arg2, unsigned srcLine) {
    instruction new_instruction;

    new_instruction.opcode = op;
    new_instruction.result = res;
    new_instruction.arg1 = arg1;
    new_instruction.arg2 = arg2;
    new_instruction.srcLine = srcLine;

    return new_instruction;
}

vmarg* make_retvaloperand() {
    return new vmarg{RETVAL_T, 0};
}

vmarg* make_numberoperand(double num) {
    std::cout << "make_numberoperand NUM: " << num << std::endl;
    return new vmarg{NUMBER_T, consts_constnumbers((unsigned)num)};
}

vmarg* make_booloperand(int bool_const) {
    return new vmarg{BOOL_T, (unsigned)bool_const};
}

vmarg* make_operand(expression* expr) {
    if (expr->type == -1) {
        std::cout << "EXPR " << expr->type <<std::endl;
        return NULL;
    }
    switch (expr->type) {
        
        case VAR_EXPR:
        case TABLEITEM_EXPR:
        case ARITHM_EXPR:
        case BOOL_EXPR:
        case NEWTABLE_EXPR:
        case ASSIGN_EXPR:
            switch (expr->symbol->space) {
                case program_var:
                    return new vmarg{GLOBAL_T, expr->symbol->offset};
                case function_local:
                    return new vmarg{LOCAL_T, expr->symbol->offset};
                case formal_arg:
                    return new vmarg{FORMAL_T, expr->symbol->offset};
                default:
                    std::cerr << "Invalid space type for variable expression" << std::endl;
                    return NULL;
            }
            
        case CONSTNUM_EXPR:
            return new vmarg{NUMBER_T, consts_constnumbers(expr->num_const)};
        case CONSTBOOL_EXPR:
            return new vmarg{BOOL_T, (unsigned)expr->bool_const};
        case CONSTSTRING_EXPR:
            return new vmarg{STRING_T, consts_conststrings(expr->str_const)};
        case NIL_EXPR:
            return new vmarg{NIL_T, 0};
        case LIBFUNC_EXPR: {
            return new vmarg{LIBFUNC_T, libfuncs_newused(expr->symbol->name)};
        }
        case PROGRAMFUNC_EXPR: {
            std::cout << "PROGRAMFUNC_EXPR: " << expr->symbol->name << std::endl;
            return new vmarg{USERFUNC_T, userfuncs_newfunc(expr)};
        }
    }
}

// Function prototypes for each opcode generator
void generate_ASSIGN(quad* quad) {
    // Implementation for generating code for ASSIGN opcode
    std::cout << "Generating code for ASSIGN" << std::endl;
    instruction* assign_instruction = new instruction;
    assign_instruction->opcode = ASSIGN_T;
    assign_instruction->result = make_operand(quad->result);
    assign_instruction->arg1 = make_operand(quad->arg1);
    assign_instruction->arg2 = NULL;
    assign_instruction->srcLine = quad->line;
    quad->taddress = next_instruction_label();
    emit(assign_instruction);
}

void generate_NOP(quad* quad) {
    // Implementation for generating code for NOP opcode
    std::cout << "Generating code for NOP" << std::endl;
    instruction* nop_instruction = new instruction;
    nop_instruction->opcode = NOP;
    nop_instruction->result = NULL;
    nop_instruction->arg1 = NULL;
    nop_instruction->arg2 = NULL;
    nop_instruction->srcLine = quad->line;
    emit(nop_instruction);
}

void generate_ADD(quad* quad) {
    // Implementation for generating code for ADD opcode
    std::cout << "Generating code for ADD" << std::endl;
    instruction* add_instruction = new instruction;
    add_instruction->opcode = ADD_T;
    add_instruction->result = make_operand(quad->result);
    add_instruction->arg1 = make_operand(quad->arg1);
    add_instruction->arg2 = make_operand(quad->arg2);
    add_instruction->srcLine = quad->line;
    quad->taddress = next_instruction_label();
    emit(add_instruction);
}

void generate_SUB(quad* quad) {
    // Implementation for generating code for SUB opcode
    std::cout << "Generating code for SUB" << std::endl;
    instruction* sub_instruction = new instruction;
    sub_instruction->opcode = SUB_T;
    sub_instruction->result = make_operand(quad->result);
    sub_instruction->arg1 = make_operand(quad->arg1);
    sub_instruction->arg2 = make_operand(quad->arg2);
    sub_instruction->srcLine = quad->line;
    quad->taddress = next_instruction_label();
    emit(sub_instruction); 
}

void generate_MUL(quad* quad) {
    // Implementation for generating code for MUL opcode
    std::cout << "Generating code for MUL" << std::endl;
    instruction* mul_instruction = new instruction;
    mul_instruction->opcode = MUL_T;
    mul_instruction->result = make_operand(quad->result);
    mul_instruction->arg1 = make_operand(quad->arg1);
    mul_instruction->arg2 = make_operand(quad->arg2);
    mul_instruction->srcLine = quad->line;
    quad->taddress = next_instruction_label();
    emit(mul_instruction);
}

void generate_DIV(quad* quad) {
    // Implementation for generating code for DIV opcode
    std::cout << "Generating code for DIV" << std::endl;
    instruction* div_instruction = new instruction;
    div_instruction->opcode = DIV_T;
    div_instruction->result = make_operand(quad->result);
    div_instruction->arg1 = make_operand(quad->arg1);
    div_instruction->arg2 = make_operand(quad->arg2);
    div_instruction->srcLine = quad->line;
    quad->taddress = next_instruction_label();
    emit(div_instruction);
}

void generate_MOD(quad* quad) {
    // Implementation for generating code for MOD opcode
    std::cout << "Generating code for MOD" << std::endl;
    instruction* mod_instruction = new instruction;
    mod_instruction->opcode = MOD_T;
    mod_instruction->result = make_operand(quad->result);
    mod_instruction->arg1 = make_operand(quad->arg1);
    mod_instruction->arg2 = make_operand(quad->arg2);
    mod_instruction->srcLine = quad->line;
    quad->taddress = next_instruction_label();
    emit(mod_instruction);
}

void generate_UMINUS(quad* quad) {
    // Implementation for generating code for UMINUS opcode
    std::cout << "Generating code for UMINUS" << std::endl;

    instruction* uminus_instruction = new instruction;
    uminus_instruction->opcode = MUL_T;
    uminus_instruction->result = make_operand(quad->result);
    uminus_instruction->arg1 = make_operand(quad->arg1);
    uminus_instruction->arg2 = make_numberoperand(-1);    
    uminus_instruction->srcLine = quad->line;
    quad->taddress = next_instruction_label();
    emit(uminus_instruction);
}

void generate_AND(quad* quad) {
    // Implementation for generating code for AND opcode
    std::cout << "Generating code for AND" << std::endl;
    // This is a placeholder; actual implementation would depend on the context
    // of the AND operation in the quad.
    // For example, you might want to generate code that performs a logical AND
    // between two boolean values.
    // This could involve creating a new instruction that checks if both values
    // are true and sets the result accordingly.
    // For now, we'll just print a message.       
}

void generate_OR(quad* quad) {
    // Implementation for generating code for OR opcode
    //std::cout << "Generating code for OR" << std::endl;
    //quad->taddress = next_instruction_label();
    //instruction* or_instruction = new instruction;
    //or_instruction->opcode = JEQ;
    //or_instruction->result = make_operand(quad->arg1);
    //or_instruction->arg1 = make_booloperand(1);
    //or_instruction->arg2 = NULL;
    //or_instruction->srcLine = quad->line;
    //or_instruction->result->type = LABEL_T;
    //or_instruction->result->value = quad->label+4;
    //emit(or_instruction);

    //instruction* or_instruction2 = new instruction;
    //or_instruction2->opcode = JEQ;
    //or_instruction2->result = make_operand(quad->arg2);
    //or_instruction2->arg1 = make_booloperand(1);
    //or_instruction2->arg2 = NULL;
    //or_instruction2->srcLine = quad->line;
    //or_instruction2->result->type = LABEL_T;
    //or_instruction2->result->value = quad->label+3;
    //emit(or_instruction2);

    //instruction* assign_instruction = new instruction;
    //assign_instruction->opcode = ASSIGN_T;
    //assign_instruction->result = make_operand(quad->result);
    //assign_instruction->arg1 = make_booloperand(0);
    //assign_instruction->arg2 = NULL;
    //assign_instruction->srcLine = quad->line;
    //emit(assign_instruction);

    //instruction* jump_instruction = new instruction;
    //jump_instruction->opcode = JUMP_T;
    //jump_instruction->result = new vmarg{LABEL_T, 0}; // Placeholder for label
    //jump_instruction->arg1 = NULL;
    //jump_instruction->arg2 = NULL;
    //jump_instruction->srcLine = quad->line; // Set to 0 or appropriate line number
    //jump_instruction->result->value = quad->label+2; // Set the label to the next instruction
    //emit(jump_instruction);

    //instruction* assign_instruction2 = new instruction;
    //assign_instruction2->opcode = ASSIGN_T;
    //assign_instruction2->result = make_operand(quad->result);
    //assign_instruction2->arg1 = make_booloperand(1);
    //assign_instruction2->arg2 = NULL;
    //assign_instruction2->srcLine = quad->line;
    //emit(assign_instruction2);
}

void generate_NOT(quad* quad) {
    // Implementation for generating code for NOT opcode
    //std::cout << "Generating code for NOT" << std::endl;
    //quad->taddress = next_instruction_label();
    //instruction* not_instruction = new instruction;
    //not_instruction->opcode = JEQ;
    //not_instruction->result = make_operand(quad->result);
    //not_instruction->arg1 = make_booloperand(0);
    //not_instruction->arg2 = NULL;
    //not_instruction->srcLine = quad->line;
    //not_instruction->result->type = LABEL_T;
    //not_instruction->result->value = quad->label+3;
    //emit(not_instruction);

    //instruction* assign_instruction = new instruction;
    //assign_instruction->opcode = ASSIGN_T;
    //assign_instruction->result = make_operand(quad->result);
    //assign_instruction->arg1 = make_booloperand(0);
    //assign_instruction->arg2 = NULL;
    //assign_instruction->srcLine = quad->line;
    //emit(assign_instruction);

    //instruction* jump_instruction = new instruction;
    //jump_instruction->opcode = JUMP_T;
    //jump_instruction->result = new vmarg{LABEL_T, 0}; // Placeholder for label
    //jump_instruction->arg1 = NULL;
    //jump_instruction->arg2 = NULL;
    //jump_instruction->srcLine = quad->line; // Set to 0 or appropriate line number
    //jump_instruction->result->value = quad->label+2; // Set the label to the next instruction
    //emit(jump_instruction);

    //instruction* assign_instruction2 = new instruction;
    //assign_instruction2->opcode = ASSIGN_T;
    //assign_instruction2->result = make_operand(quad->result);
    //assign_instruction2->arg1 = make_booloperand(1);
    //assign_instruction2->arg2 = NULL;
    //assign_instruction2->srcLine = quad->line;
    //emit(assign_instruction2);
}

void generate_relational(vmopcode opcode, quad* quad) {
    instruction* if_eq_instruction = new instruction;
    if_eq_instruction->opcode = opcode;
    if_eq_instruction->arg1 = make_operand(quad->result);
    if_eq_instruction->arg2 = make_operand(quad->arg1);
    if_eq_instruction->srcLine = quad->line;
    if_eq_instruction->result = new vmarg{LABEL_T, 0}; // Placeholder for label

    if (quad->label < retrieve_quad_index()) {
        std::cout << "HERE if" << std::endl;
        if_eq_instruction->result->value = quad->label;
    } else {
        std::cout << "HERE else" << std::endl;
        //if_eq_instruction->result->value = quad_table[quad->label].taddress;
    }
    quad->taddress = next_instruction_label();
    emit(if_eq_instruction);
}

void generate_IF_EQ(quad* quad) {
    // Implementation for generating code for IF_EQ opcode
    std::cout << "Generating code for IF_EQ" << std::endl;
    //std::cout << "quad->arg2->symbol->name:" << quad->arg2->symbol->name << std::endl;
    generate_relational(JEQ, quad);
    return;
}

void generate_IF_N_EQ(quad* quad) {
    // Implementation for generating code for IF_N_EQ opcode
    std::cout << "Generating code for IF_N_EQ" << std::endl;
    
    generate_relational(JNE, quad);
    return;
}

void generate_IF_L_EQ(quad* quad) {
    // Implementation for generating code for IF_L_EQ opcode
    std::cout << "Generating code for IF_L_EQ" << std::endl;
    
    generate_relational(JLE, quad);
    return;
}

void generate_IF_GR_EQ(quad* quad) {
    // Implementation for generating code for IF_GR_EQ opcode
    std::cout << "Generating code for IF_GR_EQ" << std::endl;
    
    generate_relational(JGE, quad);
    return;
}

void generate_IF_L(quad* quad) {
    // Implementation for generating code for IF_L opcode
    std::cout << "Generating code for IF_L" << std::endl;
    
    generate_relational(JLT, quad);
    return;
}

void generate_IF_GR(quad* quad) {
    // Implementation for generating code for IF_GR opcode
    std::cout << "Generating code for IF_GR" << std::endl;
    
    generate_relational(JGT, quad);
    return;
}

void generate_CALL(quad* quad) {
    // Implementation for generating code for CALL opcode
    std::cout << "Generating code for CALL" << std::endl;
    quad->taddress = next_instruction_label();
    instruction* call_instruction = new instruction;
    call_instruction->opcode = CALLFUNC;
    call_instruction->result = make_operand(quad->result);
    call_instruction->arg1 = NULL;
    call_instruction->arg2 = NULL;
    call_instruction->srcLine = quad->line;
    emit(call_instruction);    
}

void generate_PARAM(quad* quad) {
    // Implementation for generating code for PARAM opcode
    std::cout << "Generating code for PARAM" << std::endl;
    quad->taddress = next_instruction_label();
    instruction* param_instruction = new instruction;
    param_instruction->opcode = PUSHARG;
    param_instruction->result = make_operand(quad->result);
    param_instruction->arg1 = NULL;
    param_instruction->arg2 = NULL;
    param_instruction->srcLine = quad->line;
    emit(param_instruction);
}

void generate_RET(quad* quad) {
    // Implementation for generating code for RET opcode
    std::cout << "Generating code for RET" << std::endl;
    quad->taddress = next_instruction_label();
    instruction* ret_instruction = new instruction;
    ret_instruction->opcode = ASSIGN_T;
    ret_instruction->result = make_retvaloperand();
    ret_instruction->arg1 = make_operand(quad->result);
    ret_instruction->arg2 = NULL;
    ret_instruction->srcLine = quad->line;
    emit(ret_instruction);
}

void generate_GETRETVAL(quad* quad) {
    // Implementation for generating code for GETRETVAL opcode
    //std::cout << "Generating code for GETRETVAL" << std::endl;
    //std::cout << "Quad result type:" << quad->result->type << std::endl;
    quad->taddress = next_instruction_label();
    instruction* getret_instruction = new instruction;
    getret_instruction->opcode = ASSIGN_T;
    getret_instruction->result = make_retvaloperand();
    getret_instruction->arg1 = make_operand(quad->result);
    getret_instruction->arg2 = NULL;
    getret_instruction->srcLine = quad->line;
    emit(getret_instruction);
}

void generate_FUNCSTART(quad* quad) {
    // Implementation for generating code for FUNCSTART opcode
    std::cout << "Generating code for FUNCSTART" << std::endl;

    quad->taddress = next_instruction_label();
    instruction* funcstart_instruction = new instruction;
    funcstart_instruction->opcode = ENTERFUNC;
    funcstart_instruction->result = make_operand(quad->result);
    funcstart_instruction->arg1 = NULL;
    funcstart_instruction->arg2 = NULL;
    funcstart_instruction->srcLine = quad->line;
    emit(funcstart_instruction);
}

void generate_FUNCEND(quad* quad) {
    // Implementation for generating code for FUNCEND opcode
    std::cout << "Generating code for FUNCEND" << std::endl;

    quad->taddress = next_instruction_label();
    instruction* funcend_instruction = new instruction;
    funcend_instruction->opcode = EXITFUNC;
    funcend_instruction->result = make_operand(quad->result);
    funcend_instruction->arg1 = NULL;
    funcend_instruction->arg2 = NULL;
    funcend_instruction->srcLine = quad->line;
    emit(funcend_instruction);
}

void generate_TABLECREATE(quad* quad) {
    // Implementation for generating code for TABLECREATE opcode
    std::cout << "Generating code for TABLECREATE" << std::endl;

    instruction* tablecreate_instruction = new instruction;
    tablecreate_instruction->opcode = NEWTABLE;
    tablecreate_instruction->result = make_operand(quad->result);
    tablecreate_instruction->arg1 = NULL;
    tablecreate_instruction->arg2 = NULL;
    tablecreate_instruction->srcLine = quad->line;
    quad->taddress = next_instruction_label();
    emit(tablecreate_instruction);
}

void generate_TABLEGETELEM(quad* quad) {
    // Implementation for generating code for TABLEGETELEM opcode
    std::cout << "Generating code for TABLEGETELEM" << std::endl;

    instruction* tablegetelem_instruction = new instruction;
    tablegetelem_instruction->opcode = TABLEGETELEM_T;
    tablegetelem_instruction->result = make_operand(quad->result);
    tablegetelem_instruction->arg1 = make_operand(quad->arg1);
    tablegetelem_instruction->arg2 = make_operand(quad->arg2);
    tablegetelem_instruction->srcLine = quad->line;
    quad->taddress = next_instruction_label();
    emit(tablegetelem_instruction);
}

void generate_TABLESETELEM(quad* quad) {
    // Implementation for generating code for TABLESETELEM opcode
    std::cout << "Generating code for TABLESETELEM" << std::endl;

    instruction* tablesetelem_instruction = new instruction;
    tablesetelem_instruction->opcode = TABLESETELEM_T;
    tablesetelem_instruction->result = make_operand(quad->result);
    tablesetelem_instruction->arg1 = make_operand(quad->arg1);
    tablesetelem_instruction->arg2 = make_operand(quad->arg2);
    tablesetelem_instruction->srcLine = quad->line;
    quad->taddress = next_instruction_label();
    emit(tablesetelem_instruction);
}

void generate_JUMP(quad* quad) {
    // Implementation for generating code for JUMP opcode
    std::cout << "Generating code for JUMP" << std::endl;
    instruction* jump_instruction = new instruction;
    
    jump_instruction->opcode = JUMP_T;
    jump_instruction->result = new vmarg{LABEL_T, 0}; // Placeholder for label
    jump_instruction->arg1 = NULL;
    jump_instruction->arg2 = NULL;
    jump_instruction->srcLine = quad->line; // Set to 0 or appropriate line number
    jump_instruction->result->value = quad->label; // Set the label to the next instruction
    quad->taddress = next_instruction_label();
    emit(jump_instruction);
}

void generate_tcode() {
    for (auto& quad : quad_table) {
        if (quad.op < 0 || quad.op >= sizeof(generators) / sizeof(generators[0])) {
            std::cerr << "Invalid opcode: " << quad.op << std::endl;
            continue;
        }
        generators[quad.op](&quad);
    }
}

void print_instruction(const instruction& instr) {
    if (instr.opcode == ASSIGN_T) {
        if (instr.result->type == RETVAL_T) {
            std::cout << instr.srcLine << "\t\t" << "ASSIGN" << "\t\t" << instr.arg1->type << "\t\t" << instr.arg1->value << std::endl;
        } else {
            std::cout << instr.srcLine << "\t\t" << "ASSIGN" << "\t\t" << instr.result->type << "\t\t" << instr.result->value << "\t\t" << instr.arg1->type << "\t\t" << instr.arg1->value << std::endl;
        }
    } else if (instr.opcode == ADD_T) {
        std::cout << instr.srcLine << "\t\t" << "ADD" << "\t\t" << instr.result->type << "\t\t" << instr.result->value << "\t\t" << instr.arg1->type << "\t\t" << instr.arg1->value << "\t\t" << instr.arg2->type << "\t\t" << instr.arg2->value << std::endl;
    } else if (instr.opcode == SUB_T) {
        std::cout << instr.srcLine << "\t\t" << "SUB" << "\t\t" << instr.result->type << "\t\t" << instr.result->value << "\t\t" << instr.arg1->type << "\t\t" << instr.arg1->value << "\t\t" << instr.arg2->type << "\t\t" << instr.arg2->value << std::endl;
    } else if (instr.opcode == MUL_T) {
        std::cout << instr.srcLine << "\t\t" << "MUL" << "\t\t" << instr.result->type << "\t\t" << instr.result->value << "\t\t" << instr.arg1->type << "\t\t" << instr.arg1->value << "\t\t" << instr.arg2->type << "\t\t" << instr.arg2->value << std::endl;
    } else if (instr.opcode == DIV_T) {
        std::cout << instr.srcLine << "\t\t" << "DIV" << "\t\t" << instr.result->type << "\t\t" << instr.result->value << "\t\t" << instr.arg1->type << "\t\t" << instr.arg1->value << "\t\t" << instr.arg2->type << "\t\t" << instr.arg2->value << std::endl;
    } else if (instr.opcode == MOD_T) {
        std::cout << instr.srcLine << "\t\t" << "MOD" << "\t\t" << instr.result->type << "\t\t" << instr.result->value << "\t\t" << instr.arg1->type << "\t\t" << instr.arg1->value << "\t\t" << instr.arg2->type << "\t\t" << instr.arg2->value << std::endl;
    } else if (instr.opcode == JEQ) {
        std::cout << instr.srcLine << "\t\t" << "JEQ" << "\t\t" << instr.arg1->type << "\t\t" << instr.arg1->value << "\t\t" << instr.arg2->type << "\t\t" << instr.arg2->value << "\t\t" << instr.result->type << "\t\t" << instr.result->value << std::endl;
    } else if (instr.opcode == JNE) {
        std::cout << instr.srcLine << "\t\t" << "JNE" << "\t\t" << instr.arg1->type << "\t\t" << instr.arg1->value << "\t\t" << instr.arg2->type << "\t\t" << instr.arg2->value << "\t\t" << instr.result->type << "\t\t" << instr.result->value << std::endl;
    } else if (instr.opcode == JLE) {
        std::cout << instr.srcLine << "\t\t" << "JLE" << "\t\t" << instr.arg1->type << "\t\t" << instr.arg1->value << "\t\t" << instr.arg2->type << "\t\t" << instr.arg2->value << "\t\t" << instr.result->type << "\t\t" << instr.result->value << std::endl;
    } else if (instr.opcode == JGE) {
        std::cout << instr.srcLine << "\t\t" << "JGE" << "\t\t" << instr.arg1->type << "\t\t" << instr.arg1->value << "\t\t" << instr.arg2->type << "\t\t" << instr.arg2->value << "\t\t" << instr.result->type << "\t\t" << instr.result->value << std::endl;
    } else if (instr.opcode == JLT) {
        std::cout << instr.srcLine << "\t\t" << "JLT" << "\t\t" << instr.arg1->type << "\t\t" << instr.arg1->value << "\t\t" << instr.arg2->type << "\t\t" << instr.arg2->value << "\t\t" << instr.result->type << "\t\t" << instr.result->value << std::endl;
    } else if (instr.opcode == JGT) {
        std::cout << instr.srcLine << "\t\t" << "JGT" << "\t\t" << instr.arg1->type << "\t\t" << instr.arg1->value << "\t\t" << instr.arg2->type << "\t\t" << instr.arg2->value << "\t\t" << instr.result->type << "\t\t" << instr.result->value << std::endl;
    } else if (instr.opcode == CALLFUNC) {
        std::cout << instr.srcLine << "\t\t" << "CALLFUNC" << "\t\t" << instr.result->type << "\t\t" << instr.result->value << std::endl;
    } else if (instr.opcode == PUSHARG) {
        std::cout << instr.srcLine << "\t\t" << "PUSHARG"<< "\t\t" << instr.result->type << "\t\t" << instr.result->value << std::endl;
    } else if (instr.opcode == ENTERFUNC) {
        std::cout << instr.srcLine << "\t\t" << "ENTERFUNC" << "\t\t" << instr.result->value << std::endl;
    } else if (instr.opcode == EXITFUNC) {
        std::cout << instr.srcLine << "\t\t" << "EXITFUNC" << "\t\t" << instr.result->value << std::endl;
    } else if (instr.opcode == NEWTABLE) {
        std::cout << instr.srcLine << "\t\t" << "NEWTABLE" << "\t\t" << instr.result->type << "\t\t" << instr.result->value << std::endl;
    } else if (instr.opcode == TABLEGETELEM_T) {
        std::cout << instr.srcLine << "\t\t" << "TABLEGETELEM" << "\t\t" << instr.result->type << "\t\t" << instr.result->value << "\t\t" << instr.arg1->type << "\t\t" << instr.arg1->value << "\t\t" << instr.arg2->type << "\t\t" << instr.arg2->value << std::endl;
    } else if (instr.opcode == TABLESETELEM_T) {
        std::cout << instr.srcLine << "\t\t" << "TABLESETELEM" << "\t\t" << instr.result->type << "\t\t" << instr.result->value << "\t\t" << instr.arg1->type << "\t\t" << instr.arg1->value << "\t\t" << instr.arg2->type << "\t\t" << instr.arg2->value << std::endl;
    } else if (instr.opcode == JUMP_T) {
        std::cout << instr.srcLine << "\t\t" << "JUMP" << "\t\t" << instr.result->type << "\t\t" << instr.result->value << std::endl;
    } else if (instr.opcode == NOP) {
        std::cout << instr.srcLine << "\t\t" << "NOP" << std::endl;
    } else {
        std::cerr << "Unknown opcode: " << instr.opcode << std::endl;
    }       
}

void clear_instruction_table() {
    instruction_table.clear();
}
void reset_instruction_table() {
    instruction_table.clear();
}
void print_instruction_table() {
    std::cout << "Instruction Table:" << std::endl;
    for (const auto& instr : instruction_table) {
        print_instruction(instr);
    }
}

void avm_binary_file(const std::string& filename = "tcode.abc") {


    std::ofstream out(filename, std::ios::binary);
    if (!out.is_open()) {
        std::cerr << "Failed to open binary file for writing: " << filename << std::endl;
        return;
    }

    // Write the number of instructions first
    uint32_t instr_count = instruction_table.size();
    out.write(reinterpret_cast<const char*>(&instr_count), sizeof(instr_count));
    unsigned magic_number = 0x14069134; // Example magic number
    out.write(reinterpret_cast<const char*>(&magic_number), sizeof(magic_number));
    unsigned total_strings = conststrings_vector.size();
    unsigned total_numbers = constsnumbers_vector.size();
    unsigned total_userfuncs = userfuncs_vector.size();
    unsigned total_libfuncs = libfuncs_vector.size();
    out.write(reinterpret_cast<const char*>(&total_strings), sizeof(total_strings));
    for (const auto& str : conststrings_vector) {
        uint32_t str_length = str.length();
        out.write(reinterpret_cast<const char*>(&str_length), sizeof(str_length));
        out.write(str.c_str(), str_length);
        unsigned size = str.length();
        std::cout << "String: " << str << " (length: " << size << ")" << std::endl;
        out.write(reinterpret_cast<const char*>(&size), sizeof(size));
    }
    out.write(reinterpret_cast<const char*>(&total_numbers), sizeof(total_numbers));
    for (const auto& num : constsnumbers_vector) {
        out.write(reinterpret_cast<const char*>(&num), sizeof(num));
    }
    out.write(reinterpret_cast<const char*>(&total_userfuncs), sizeof(total_userfuncs));
    for (const auto& userfunc : userfuncs_vector) {
        unsigned address = userfunc->symbol->i_address;
        unsigned local_size = userfunc->symbol->total_locals;
        const std::string& name = userfunc->symbol->name;
        uint32_t name_length = name.length();
        out.write(reinterpret_cast<const char*>(&address), sizeof(address));
        out.write(reinterpret_cast<const char*>(&local_size), sizeof(local_size));
        out.write(reinterpret_cast<const char*>(&name_length), sizeof(name_length));
        out.write(name.c_str(), name_length);
    }
    out.write(reinterpret_cast<const char*>(&total_libfuncs), sizeof(total_libfuncs));
    for (const auto& libfunc : libfuncs_vector) {
        uint32_t name_length = libfunc.length();
        out.write(reinterpret_cast<const char*>(&name_length), sizeof(name_length));
        out.write(libfunc.c_str(), name_length);
    }

    for (const auto& instr : instruction_table) {
        
        // Write opcode and srcLine
        out.write(reinterpret_cast<const char*>(&instr.opcode), sizeof(instr.opcode));
        out.write(reinterpret_cast<const char*>(&instr.srcLine), sizeof(instr.srcLine));

        // Helper lambda to write a vmarg pointer (can be NULL)
        auto write_vmarg = [&out](const vmarg* arg) {
            uint8_t is_null = (arg == nullptr) ? 1 : 0;
            out.write(reinterpret_cast<const char*>(&is_null), sizeof(is_null));
            if (!is_null) {
                out.write(reinterpret_cast<const char*>(&arg->type), sizeof(arg->type));
                out.write(reinterpret_cast<const char*>(&arg->value), sizeof(arg->value));
            }
        };

        write_vmarg(instr.result);
        write_vmarg(instr.arg1);
        write_vmarg(instr.arg2);
    }

    out.close();
    std::cout << "Tcode instructions written to binary file: " << filename << std::endl;
}

void avm_binary_dump(const std::string& filename = "tcode.abc") {
    std::ifstream in(filename, std::ios::binary);
    if (!in.is_open()) {
        std::cerr << "Failed to open binary file for reading: " << filename << std::endl;
        return;
    }

    uint32_t instr_count;
    in.read(reinterpret_cast<char*>(&instr_count), sizeof(instr_count));
    std::cout << "Instruction count: " << instr_count << std::endl;

    unsigned magic_number;
    in.read(reinterpret_cast<char*>(&magic_number), sizeof(magic_number));
    std::cout << "Magic number: " << std::hex << magic_number << std::dec << std::endl;
    if (magic_number != 0x14069134) {
        std::cerr << "Invalid magic number in binary file." << std::endl;
        in.close();
        return;
    }

    // Read and print constant strings
    unsigned total_strings;
    in.read(reinterpret_cast<char*>(&total_strings), sizeof(total_strings));
    std::cout << "Total strings: " << total_strings << std::endl;
    for (unsigned i = 0; i < total_strings; ++i) {
        uint32_t str_length;
        in.read(reinterpret_cast<char*>(&str_length), sizeof(str_length));
        std::string str(str_length, '\0');
        in.read(&str[0], str_length);
        std::cout << "  String[" << i << "]: " << str << std::endl;
        in.read(reinterpret_cast<char*>(&str_length), sizeof(str_length));
        std::cout << "  String length: " << str_length << std::endl;
        if (str_length != str.length()) {
            std::cerr << "String length mismatch for string[" << i << "]. Expected: " << str_length << ", Actual: " << str.length() << std::endl;
        }
        std::cout << "  String size: " << str.length() << std::endl;
    }

    // Read and print constant numbers
    unsigned total_numbers;
    in.read(reinterpret_cast<char*>(&total_numbers), sizeof(total_numbers));
    std::cout << "Total numbers: " << total_numbers << std::endl;
    for (unsigned i = 0; i < total_numbers; ++i) {
        double num;
        in.read(reinterpret_cast<char*>(&num), sizeof(num));
        std::cout << "  Number[" << i << "]: " << num << std::endl;
    }

    // Read and print user functions
    unsigned total_userfuncs;
    in.read(reinterpret_cast<char*>(&total_userfuncs), sizeof(total_userfuncs));
    std::cout << "Total userfuncs: " << total_userfuncs << std::endl;
    for (unsigned i = 0; i < total_userfuncs; ++i) {
        unsigned address, local_size;
        in.read(reinterpret_cast<char*>(&address), sizeof(address));
        in.read(reinterpret_cast<char*>(&local_size), sizeof(local_size));
        uint32_t name_length;
        in.read(reinterpret_cast<char*>(&name_length), sizeof(name_length));
        std::string name(name_length, '\0');
        in.read(&name[0], name_length);
        std::cout << "  UserFunc[" << i << "]: address=" << address
                  << ", local_size=" << local_size
                  << ", name=" << name << std::endl;
    }

    // Read and print library functions
    unsigned total_libfuncs;
    in.read(reinterpret_cast<char*>(&total_libfuncs), sizeof(total_libfuncs));
    std::cout << "Total libfuncs: " << total_libfuncs << std::endl;
    for (unsigned i = 0; i < total_libfuncs; ++i) {
        uint32_t name_length;
        in.read(reinterpret_cast<char*>(&name_length), sizeof(name_length));
        std::string name(name_length, '\0');
        in.read(&name[0], name_length);
        std::cout << "  LibFunc[" << i << "]: " << name << std::endl;
    }

    // Read and print instructions
    for (uint32_t i = 0; i < instr_count; ++i) {
        vmopcode opcode;
        unsigned srcLine;
        in.read(reinterpret_cast<char*>(&opcode), sizeof(opcode));
        in.read(reinterpret_cast<char*>(&srcLine), sizeof(srcLine));

        auto read_vmarg = [&in]() -> vmarg {
            uint8_t is_null;
            in.read(reinterpret_cast<char*>(&is_null), sizeof(is_null));
            vmarg arg;
            if (!is_null) {
                in.read(reinterpret_cast<char*>(&arg.type), sizeof(arg.type));
                in.read(reinterpret_cast<char*>(&arg.value), sizeof(arg.value));
            } else {
                arg.type = (vmarg_t)0;
                arg.value = 0;
            }
            return arg;
        };

        vmarg result = read_vmarg();
        vmarg arg1 = read_vmarg();
        vmarg arg2 = read_vmarg();

        std::cout << "Instruction " << i << ": opcode=" << opcode
                  << ", srcLine=" << srcLine
                  << ", result(type=" << result.type << ", value=" << result.value << ")"
                  << ", arg1(type=" << arg1.type << ", value=" << arg1.value << ")"
                  << ", arg2(type=" << arg2.type << ", value=" << arg2.value << ")"
                  << std::endl;
    }
    in.close();
}