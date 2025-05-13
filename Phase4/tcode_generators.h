#ifndef _TCODE_GENERATORS_H
#define _TCODE_GENERATORS_H

#include <iostream>
#include <string>
#include "icode_managers.h"

enum vmopcode {
    ASSIGN_T,        ADD_T,            SUB_T,
    MUL_T,           DIV_T,            MOD_T,
    JEQ,             JNE,              JLE,        
    JGE,             JLT,              JGT,          
    CALLFUNC,        PUSHARG,          ENTERFUNC,    
    EXITFUNC,        TABLECREATE_T,    TABLEGETELEM_T, 
    TABLESETELEM_T,  JUMP_T,           NOP
};

enum vmarg_t {
    GLOBAL_T,
    LOCAL_T,
    FORMAL_T,
    BOOL_T,
    STRING_T,
    NUMBER_T,
    USERFUNC_T,
    LIBFUNC_T,
    RETVAL_T,
    NIL_T,
    LABEL_T
};

struct vmarg {
    vmarg_t   type;
    unsigned value;
};


struct instruction {
    enum vmopcode opcode;
    vmarg* arg1;
    vmarg* arg2;
    vmarg* result;
    unsigned srcLine;
};

unsigned retrieve_quad_index();
unsigned current_processed_quad();

unsigned next_instruction_label();
vmarg* make_retvaloperand(vmarg* arg);
vmarg* make_numberoperand(double num);
vmarg* make_booloperand(int bool_const);

vmarg* make_operand(expression*);
instruction make_instruction(vmopcode, vmarg*, vmarg*, vmarg*, unsigned);

void generate_tcode();
void generate_ASSIGN(quad*);
void generate_ADD(quad*);
void generate_SUB(quad*);
void generate_MUL(quad*);
void generate_DIV(quad*);
void generate_MOD(quad*);
void generate_UMINUS(quad*);
void generate_AND(quad*);
void generate_OR(quad*);
void generate_NOT(quad*);
void generate_IF_EQ(quad*);
void generate_IF_N_EQ(quad*);
void generate_IF_L_EQ(quad*);
void generate_IF_GR_EQ(quad*);
void generate_IF_L(quad*);
void generate_IF_GR(quad*);
void generate_CALL(quad*);
void generate_PARAM(quad*);
void generate_RET(quad*);
void generate_GETRETVAL(quad*);
void generate_FUNCSTART(quad*);
void generate_FUNCEND(quad*);
void generate_TABLECREATE(quad*);
void generate_TABLEGETELEM(quad*);
void generate_TABLESETELEM(quad*);
void generate_JUMP(quad*);

void emit(instruction*);

void print_instruction(instruction*);
void print_instruction_table();
void clear_instruction_table();
void reset_instruction_table();

#endif