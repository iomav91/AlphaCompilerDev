#include "tcode_generators.h"
#include "icode_managers.h"
#include <iostream>
#include <string>
#include <vector>
#include "symbol_table.h"

extern std::vector<quad> quad_table;
std::vector<instruction> instruction_table;

typedef void (*generator_func_t)(quad*);

unsigned consts_conststrings(std::string str) {
    unsigned index = 0;
    for (const auto& ch : str) {
        index = (index << 8) + ch;
    }
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
    return new vmarg{NUMBER_T, (unsigned)num};
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
            return new vmarg{NUMBER_T, (unsigned)expr->num_const};
        case CONSTBOOL_EXPR:
            return new vmarg{BOOL_T, (unsigned)expr->bool_const};
        case CONSTSTRING_EXPR:
            return new vmarg{STRING_T, consts_conststrings(expr->str_const)};
        case NIL_EXPR:
            return new vmarg{NIL_T, 0};
        case LIBFUNC_EXPR: {
            return new vmarg{LIBFUNC_T, expr->symbol->i_address};
        }
        case PROGRAMFUNC_EXPR: {
            return new vmarg{USERFUNC_T, expr->symbol->i_address};
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
    // This is a placeholder; actual implementation would depend on the context
    // of the UMINUS operation in the quad.
    // For example, you might want to generate code that negates a number.
    // This could involve creating a new instruction that multiplies the number
    // by -1 or something similar.
    // For now, we'll just print a message.
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

void generate_FUNCSTART(quad*) {
    // Implementation for generating code for FUNCSTART opcode
    std::cout << "Generating code for FUNCSTART" << std::endl;
}

void generate_FUNCEND(quad*) {
    // Implementation for generating code for FUNCEND opcode
    std::cout << "Generating code for FUNCEND" << std::endl;
}

void generate_TABLECREATE(quad* quad) {
    // Implementation for generating code for TABLECREATE opcode
    std::cout << "Generating code for TABLECREATE" << std::endl;

    instruction* tablecreate_instruction = new instruction;
    tablecreate_instruction->opcode = TABLECREATE_T;
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
            std::cout << instr.srcLine << "\t\t" << "ASSIGN" << "\t\t" << instr.arg1->value << std::endl;
        } else {
            std::cout << instr.srcLine << "\t\t" << "ASSIGN" << "\t\t" << instr.result->value << "\t\t" << instr.arg1->value << std::endl;
        }
    } else if (instr.opcode == ADD_T) {
        std::cout << instr.srcLine << "\t\t" << "ADD" << "\t\t" << instr.result->value << "\t\t" << instr.arg1->value << "\t\t" << instr.arg2->value << std::endl;
    } else if (instr.opcode == SUB_T) {
        std::cout << instr.srcLine << "\t\t" << "SUB" << "\t\t" << instr.result->value << "\t\t" << instr.arg1->value << "\t\t" << instr.arg2->value << std::endl;
    } else if (instr.opcode == MUL_T) {
        std::cout << instr.srcLine << "\t\t" << "MUL" << "\t\t" << instr.result->value << "\t\t" << instr.arg1->value << "\t\t" << instr.arg2->value << std::endl;
    } else if (instr.opcode == DIV_T) {
        std::cout << instr.srcLine << "\t\t" << "DIV" << "\t\t" << instr.result->value << "\t\t" << instr.arg1->value << "\t\t" << instr.arg2->value << std::endl;
    } else if (instr.opcode == MOD_T) {
        std::cout << instr.srcLine << "\t\t" << "MOD" << "\t\t" << instr.result->value << "\t\t" << instr.arg1->value << "\t\t" << instr.arg2->value << std::endl;
    } else if (instr.opcode == JEQ) {
       std::cout << instr.srcLine  << "\t\t" << "JEQ" << "\t\t" << instr.arg1->value << "\t\t" << instr.arg2->value << "\t\t" << instr.result->value << std::endl;
    } else if (instr.opcode == JNE) {
        std::cout<<instr.srcLine<<"\t\t"<< "JNE"<< "\t\t"<<instr.result->value<<"\t\t"<<instr.arg1->value<<"\t\t"<<instr.arg2->value<<std::endl;
    } else if (instr.opcode == JLE) {
        std::cout<<instr.srcLine<<"\t\t"<< "JLE"<< "\t\t"<<instr.result->value<<"\t\t"<<instr.arg1->value<<"\t\t"<<instr.arg2->value<<std::endl;
    } else if (instr.opcode == JGE) {
        std::cout<<instr.srcLine<<"\t\t"<< "JGE"<< "\t\t"<<instr.result->value<<"\t\t"<<instr.arg1->value<<"\t\t"<<instr.arg2->value<<std::endl;
    } else if (instr.opcode == JLT) {
        std::cout<<instr.srcLine<<"\t\t"<< "JLT"<< "\t\t"<<instr.result->value<<"\t\t"<<instr.arg1->value<<"\t\t"<<instr.arg2->value<<std::endl;
    } else if (instr.opcode == JGT) {
        std::cout<<instr.srcLine<<"\t\t"<< "JGT"<< "\t\t"<<instr.result->value<<"\t\t"<<instr.arg1->value<<"\t\t"<<instr.arg2->value<<std::endl;
    } else if (instr.opcode == CALLFUNC) {
        std::cout<<instr.srcLine<<"\t\t"<< "CALLFUNC"<< "\t\t"<<instr.result->value<<"\t\t"<<std::endl;
    } else if (instr.opcode == PUSHARG) {
        std::cout<<instr.srcLine<<"\t\t"<< "PUSHARG"<< "\t\t"<<instr.result->value<<"\t\t"<<std::endl;
    } else if (instr.opcode == ENTERFUNC) {
        std::cout<<instr.srcLine<<"\t\t"<< "ENTERFUNC" << "\t\t" << instr.result->value << std::endl;
    } else if (instr.opcode == EXITFUNC) {
        std::cout<<instr.srcLine<<"\t\t"<< "EXITFUNC" << "\t\t" << instr.result->value << std::endl;
    } else if (instr.opcode == TABLECREATE_T) {
        std::cout<<instr.srcLine<<"\t\t"<< "TABLECREATE" << "\t\t" << instr.result->value << std::endl;
    } else if (instr.opcode == TABLEGETELEM_T) {
        std::cout<<instr.srcLine<<"\t\t"<< "TABLEGETELEM" << "\t\t" << instr.result->value << "\t\t" << instr.arg1->value << "\t\t" << instr.arg2->value << std::endl;
    } else if (instr.opcode == TABLESETELEM_T) {
        std::cout<<instr.srcLine<<"\t\t"<< "TABLESETELEM" << "\t\t" << instr.result->value << "\t\t" << instr.arg1->value << "\t\t" << instr.arg2->value << std::endl;
    } else if (instr.opcode == JUMP_T) {
        std::cout<<instr.srcLine<<"\t\t"<< "JUMP" << "\t\t" << instr.result->value << std::endl;
    } else if (instr.opcode == NOP) {
        std::cout<<instr.srcLine<<"\t\t"<< "NOP" << std::endl;
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