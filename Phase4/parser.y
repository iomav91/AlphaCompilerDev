
%{
    #include "symbol_table.h"
    #include "handlers.h"
    #include "icode_managers.h"
    #include "tcode_generators.h"
    #include <iostream>
    #include <fstream>
    #include <string>
    extern int yylex();
    extern void yyerror(const char *);
    extern int yyparse();
    extern int yylineno;
    extern FILE* yyin;
    //extern std::list<std::string> argsList;
    int scope_counter;
    //int scope_space_counter = 1;
    int is_in_function_mode = 0;
    int is_in_block_mode = 0;
    int is_in_loop_mode = 0;
    int is_a_member_access = 0;
    std::string name = "";
%}
%output "parser.c"
%defines "parser.h"
%define parse.error detailed
%define parse.lac full

%start program

%code requires {
    struct Call {
        expression* elist;
        int method;
        char* name;
    };

    struct Forprefix {
        unsigned test;
        unsigned enter;
    };

    struct Stmt {
        unsigned breaklist;
        unsigned contlist;
        unsigned returnlist;
    };
}

%union {
    unsigned unsignedVal;
    int intVal;
    double floatVal;
    char* strVal;
    SymbolTableEntry* symbol;
    expression* expr;
    struct Call call_struct;
    struct Forprefix forprefix_struct;
    struct Stmt stmt_struct;
    indexed_elem* indexed;
}

%token TOK_IF
%token TOK_ELSE
%token TOK_WHILE
%token TOK_FOR
%token TOK_FUNCTION
%token TOK_RETURN
%token TOK_BREAK
%token TOK_CONTINUE
%token TOK_AND
%token TOK_NOT
%token TOK_OR
%token TOK_LOCAL
%token <intVal> TOK_TRUE
%token <intVal> TOK_FALSE
%token <strVal> TOK_NIL
%token TOK_ASSIGN
%token TOK_PLUS
%token TOK_MINUS
%token TOK_MULTIPLY
%token TOK_DIVIDE
%token TOK_MODULO
%token TOK_EQUAL
%token TOK_N_EQUAL
%token TOK_DBL_PLUS
%token TOK_DBL_MINUS
%token TOK_GREATER
%token TOK_LESS
%token TOK_GR_EQUAL
%token TOK_LS_EQUAL
%token TOK_L_CURLY_BR
%token TOK_R_CURLY_BR
%token TOK_L_BR
%token TOK_R_BR
%token TOK_L_PARENTH
%token TOK_R_PARENTH
%token TOK_SEMICOLON
%token TOK_COMMA
%token TOK_COLON
%token TOK_DBL_COLON
%token TOK_DOT
%token TOK_DBL_DOT
%token <strVal> TOK_ID
%token <intVal> TOK_INT
%token <floatVal> TOK_FLOAT
%token <strVal> TOK_STRING

%type <expr> lvalue
%type <expr> member
%type <expr> primary
%type <expr> assignexpr
%type <strVal> idlist
%type <expr> expression
%type <expr> const
%type <expr> call
%type <call_struct> callsuffix
%type <call_struct> methodcall
%type <call_struct> normcall
%type <expr> elist
%type <strVal> funcname
%type <symbol> funcprefix
%type <unsignedVal> funcbody
%type <expr> funcdef
%type <expr> objectdef
%type <expr> term
%type <indexed> indexed
%type <indexed> indexedelem
%type <unsignedVal> ifprefix
%type <unsignedVal> if_statement
%type <unsignedVal> ifelse_statement
%type <unsignedVal> elseprefix
%type <unsignedVal> whilestart
%type <unsignedVal> whilecond
%type <stmt_struct> whileblock
%type <unsignedVal> N 
%type <unsignedVal> M
%type <unsignedVal> M1 
%type <forprefix_struct> forprefix
%type <stmt_struct> forblock
%type <stmt_struct> break_statement
%type <stmt_struct> continue_statement 
%type <stmt_struct> return_statement
%type <stmt_struct> statements
%type <stmt_struct> statement

%right TOK_ASSIGN
%left TOK_OR
%left TOK_AND
%nonassoc TOK_EQUAL TOK_N_EQUAL
%nonassoc TOK_GREATER TOK_LESS TOK_GR_EQUAL TOK_LS_EQUAL
%left TOK_PLUS TOK_MINUS
%left TOK_MULTIPLY TOK_DIVIDE TOK_MODULO
%right TOK_NOT TOK_DBL_PLUS TOK_DBL_MINUS
%nonassoc TOK_UMINUS
%left TOK_DOT TOK_DBL_DOT
%left TOK_L_BR TOK_R_BR
%left TOK_L_PARENTH TOK_R_PARENTH

%precedence TOK_IF
%precedence TOK_ELSE

%%

program: statements statement                                                                          {

}
         | %empty                                                                                      {}
;

statement: expression TOK_SEMICOLON                                                                    {
    if (!$1->truelist.empty() || !$1->falselist.empty()) {
        SymbolType type = GLOBAL;
    if (get_scope() != 0) {
        type = LOCAL;
    }
    SymbolTableEntry* new_temp = new_temp_var(-1, type);
    expression* result = make_assign_expression(ASSIGN_EXPR, get_symbol(new_temp->name, get_scope()));

    
    for (auto& it : $1->truelist) {
        std::cout << "TRUELIST: " << it << std::endl;
        std::cout << "NEXT QUAD LABEL: " << next_quad_label() << std::endl;
        backpatch(it, next_quad_label());
    }

    for (auto& it : $1->falselist) {
        std::cout << "FALSE LIST: " << it << std::endl;
        backpatch(it, next_quad_label() + 2);
    }

    emit_assign(ASSIGN, result, make_constbool_expression(CONSTBOOL_EXPR, 1));
    emit_jump(JUMP, (next_quad_label() + 2));
    set_curr_quad_label((next_quad_label() + 2));
    emit_assign(ASSIGN, result, make_constbool_expression(CONSTBOOL_EXPR, 0));
    }
    
}

          | if_statement                                                                               {}
          | ifelse_statement                                                                           {}
          | while_statement                                                                            {}
          | for_statement                                                                              {}
          | return_statement                                                                           {
            handle_return_st();
            
          }
          | TOK_BREAK TOK_SEMICOLON                                                                           {
            handle_break_st();
            add_breakelem(next_quad_label(), 0);
            emit_jump(JUMP, 0);
          } 
          | TOK_CONTINUE TOK_SEMICOLON                                                                         {
            handle_continue_st();
            add_contelem(next_quad_label(), 0);
            emit_jump(JUMP, 0);
          }
          | block                                                                                      {}
          | funcdef                                                                                    {}
          | TOK_SEMICOLON                                                                              {}
          | error TOK_SEMICOLON {yyerrok; yyclearin;}
;

statements: statements statement                                                                       {
    
} | %empty {}
;

M: %empty {
    $$ = next_quad_label();
    //std::cout << "Next Quad: " << $$ << std::endl;
}
;

assignexpr: lvalue TOK_ASSIGN expression                                                               {
                                                                                                            
                                                                                                            $$ = manage_assign_expr($$, $1, $3);
                                                                                                       }
;



expression: assignexpr                                                                                 {$$ = $1;}
            | expression TOK_PLUS expression                                                           {
                                                                                                            //std::cout<<"EXPR PLUS EXPR symbol: " << $$->symbol->name << std::endl;
                                                                                                            $$ = manage_expr_plus_expr($$, $1, $3);
                                                                                                            //std::cout<<"EXPR PLUS EXPR symbol: " << $$->symbol->name << std::endl;
                    
                                                                                                       }
            | expression TOK_MINUS expression                                                          {
                                                                                                            
                                                                                                            $$ = manage_expr_minus_expr($$, $1, $3);
                                                                                                       
                                                                                                       }
            | expression TOK_MULTIPLY expression                                                       {

                                                                                                            $$ = manage_expr_mul_expr($$, $1, $3);        
                    
                                                                                                       }
            | expression TOK_DIVIDE expression                                                         {    
                                                                                                            $$ = manage_expr_div_expr($$, $1, $3);
                                                                                                       }
            | expression TOK_MODULO expression                                                         {

                                                                                                            $$ = manage_expr_mod_expr($$, $1, $3);

                                                                                                       }
            | expression TOK_GREATER expression {
                    
                                                                                                            $$ = manage_expr_gr_expr($$, $1, $3);

                                                                                                       }
            | expression TOK_LESS expression {
                    
                                                                                                            $$ = manage_expr_ls_expr($$, $1, $3);

                                                                                                       }
            | expression TOK_GR_EQUAL expression                                                       {  
                 
                                                                                                            $$ = manage_expr_gr_eq_expr($$, $1, $3);

                                                                                                       }
            | expression TOK_LS_EQUAL expression {
                    
                                                                                                            $$ = manage_expr_ls_eq_expr($$, $1, $3);
                
                                                                                                       }
            | expression TOK_EQUAL expression                                                          {
                                                                                                            $$ = manage_expr_eq_expr($$, $1, $3);
                                                                                                       }
            | expression TOK_N_EQUAL expression                                                        {
                   
                                                                                                            $$ = manage_expr_not_eq_expr($$, $1, $3);
                                                                                                       
                                                                                                       }
            | expression TOK_AND  {
                        if ($1->truelist.empty() && $1->falselist.empty()) {
                            $1->truelist.push_back(next_quad_label());
                            $1->falselist.push_back(next_quad_label()+1);
                            emit_if_equal(IF_EQ, $1, make_constbool_expression(CONSTBOOL_EXPR, 1), UINT_MAX);
                            emit_jump(JUMP, UINT_MAX);
                        }   
                    } M expression {
                    if ($5->truelist.empty() && $5->falselist.empty()) {
                        std::cout << "$5 symbol name is " << $5->symbol->name << std::endl;
                        $5->truelist.push_back(next_quad_label());
                        $5->falselist.push_back(next_quad_label()+1);
                        emit_if_equal(IF_EQ, $5, make_constbool_expression(CONSTBOOL_EXPR, 1), UINT_MAX);
                        emit_jump(JUMP, UINT_MAX);
                    }
                    $$ = manage_expr_and_expr($$, $1, $4, $5);
                }
            | expression TOK_OR {
                        if ($1->truelist.empty() && $1->falselist.empty()) {
                            $1->truelist.push_back(next_quad_label());
                            $1->falselist.push_back(next_quad_label()+1);
                            emit_if_equal(IF_EQ, $1, make_constbool_expression(CONSTBOOL_EXPR, 1), UINT_MAX);
                            emit_jump(JUMP, UINT_MAX);
                    
                        }
                    } M expression {
                    
                    if ($5->truelist.empty() && $5->falselist.empty()) {
                        std::cout << "$5 symbol name is " << $5->symbol->name << std::endl;
                        $5->truelist.push_back(next_quad_label());
                        $5->falselist.push_back(next_quad_label()+1);
                        emit_if_equal(IF_EQ, $5, make_constbool_expression(CONSTBOOL_EXPR, 1), UINT_MAX);
                        emit_jump(JUMP, UINT_MAX);
                    }
                    $$ = manage_expr_or_expr($$, $1, $4, $5);
                }
            | term {
                //printf("expression -> term\n");
                $$ = $1;
                //std::cout << "$$->type " << $$->type << "\t\t" << $$->symbol->name << std::endl;
            }
;

term: TOK_L_PARENTH expression TOK_R_PARENTH {
            //printf("term ->(expression)\n");
            $$ = $2;
      }
      | TOK_MINUS expression %prec TOK_UMINUS                                                          {
            
                                                                                                            $$ = manage_uminus_expr($$, $2);

                                                                                                       }
      | TOK_NOT expression                                                                             {
                                                                                                            $$ = manage_not_expr($$, $2);
                                                                                                            std::cout << "$2 " << $2->symbol->name << std::endl;

                                                                                                            if (!$2->truelist.empty() || !$2->falselist.empty()) {
                                                                                                                
                                                                                                                SymbolType type = GLOBAL;
                                                                                                                if (get_scope() != 0) {
                                                                                                                    type = LOCAL;
                                                                                                                }
                                                                                                                SymbolTableEntry* new_temp = new_temp_var(-1, type);
                                                                                                                expression* result = make_assign_expression(ASSIGN_EXPR, get_symbol(new_temp->name, get_scope()));

                                                                                                                for (auto& it : $2->truelist) {
                                                                                                                    std::cout << "TRUELIST: " << it << std::endl;
                                                                                                                    std::cout << "NEXT QUAD LABEL: " << next_quad_label() << std::endl;
                                                                                                                    backpatch(it, next_quad_label()+2);
                                                                                                                }
                                                                                                                
                                                                                                                for (auto& it : $2->falselist) {
                                                                                                                    std::cout << "FALSE LIST: " << it << std::endl;
                                                                                                                    backpatch(it, next_quad_label());
                                                                                                                }

                                                                                                                emit_assign(ASSIGN, result, make_constbool_expression(CONSTBOOL_EXPR, 1));
                                                                                                                emit_jump(JUMP, (next_quad_label() + 2));
                                                                                                                set_curr_quad_label((next_quad_label() + 2));
                                                                                                                emit_assign(ASSIGN, result, make_constbool_expression(CONSTBOOL_EXPR, 0));
                                                                                                            }

                                                                                                       }
      | TOK_DBL_PLUS lvalue                                                                            {
                                                                                                            $$ = manage_db_plus_lvalue($$, $2);
                                                                                                       }
      | lvalue TOK_DBL_PLUS                                                                            {
                                                                                                            $$ = manage_lvalue_db_plus($$, $1);
                                                                                                       }
      | TOK_DBL_MINUS lvalue                                                                           {
                                                                                                            $$ = manage_db_minus_lvalue($$, $2);
                                                                                                       }
      | lvalue TOK_DBL_MINUS                                                                           {
                                                                                                            $$ = manage_lvalue_db_minus($$, $1);    
                                                                                                       }
      | primary {
            $$ = $1;
      }
;

primary: lvalue                                                                                        {
                                                                                                            $$ = emit_if_table_item($1);
                                                                                                            std::cout<<"LVALUE HERE HERE" << std::endl;
                                                                                                       }
         | call                                                                                        {$$=$1;}
         | objectdef                                                                                   {

                                                                                                            $$ = $1;
                                                                                                       }
         | TOK_L_PARENTH funcdef TOK_R_PARENTH                                                         {
                                                                                                            $$ = $2;
                                                                                                       }
         | const                                                                                       {
                                                                                                            $$ = $1;
                                                                                                       }
;

lvalue: TOK_ID                                                                                         {
                                                                                                            $$ = manage_lvalue_id($$, $1, yylineno);
                                                                                                            //std::cout << "LVALUE EXPR SYMBOL AFTER MANAGE LVALUE ID: "  << std::endl;
                                                                                                       }
        | TOK_LOCAL TOK_ID                                                                             {
                                                                                                            //std::cout << "LOCAL ID: " << $2 << std::endl;  
                                                                                                            $$ = manage_lvalue_local_id($$, $2, yylineno);
                                                                                                       }
        | TOK_DBL_COLON TOK_ID                                                                         {
                                                                                                            $$ = manage_lvalue_global_id($$, $2, yylineno);
                                                                                                       }
        | member {
                    is_a_member_access = 1;
                    $$ = $1;
                 }
;

member: lvalue TOK_DOT TOK_ID                                                                          {
                                                                                                            $$ = manage_lvalue_dot_id($$, $1, $3);
                                                                                                            is_a_member_access = 0;
                                                                                                       }
        | lvalue TOK_L_BR expression TOK_R_BR                                                          {
            
                                                                                                            $$ = manage_lvalue_lbr_expr_rbr($$, $1, $3);
                                                                                                            clear_expression_list();
                                                                                                       }
        | call TOK_DOT TOK_ID                                                                          {}
        | call TOK_L_BR expression TOK_R_BR                                                            {}
;

call: call normcall {
            std::cout << "CALL NORMCALL" << std::endl;
            $$ = make_call($1, $2.elist);
            
       }
      | lvalue callsuffix                                                                              {
                                                                                                            if ($2.method == 0) {
                                                                                                                $$= make_call($1, $2.elist);
                                                                                                            } else                           
                                                                                                                $$ = manage_lvalue_callsuffix($$, $1, $2.name, $2.elist, $2.method);
                                                                                                       }
      | TOK_L_PARENTH funcdef TOK_R_PARENTH normcall {
            //expression* func = make_func_expression(PROGRAMFUNC_EXPR, get_symbol($2->symbol->name, get_scope()));
            //std::cout << "MAKE CALL HERE" << std::endl;
            $$ = make_call($2, $4.elist);     
      }
;

callsuffix: normcall {
                //printf("callsuffix -> normcall\n");
                $$ = $1;
            }
            | methodcall {
                //printf("callsuffix -> methodcall\n");
                $$ = $1;
            }
;

normcall: TOK_L_PARENTH elist TOK_R_PARENTH {
    //std::cout << "(elist)" << std::endl;
    $$.elist = $2;
    $$.method = 0;
    $$.name = NULL;

}
;

methodcall: TOK_DBL_DOT TOK_ID normcall {
    //printf("methodcall -> ..id normcall\n");
    $$.elist = $3.elist;
    $$.method = 1;
    $$.name = $2;

    //get_last_expression();
}
;

elist: elist TOK_COMMA expression {
            //printf("elist -> elist,\n");
            $3->next = $1;
            $$ = $3;
            
        }
        | expression {
            //printf("elist -> expression\n");
            //std::cout << "$1 -> " << $1->symbol->name << std::endl;
            $1->next = NULL;
            $$ = $1;
        }
       
       |  %empty {
            $$ = NULL;
        }
;

indexedelem: TOK_L_CURLY_BR expression TOK_COLON expression 
TOK_R_CURLY_BR {
    //printf("indexedelem -> {expression:expression}\n");
    //std::cout << $2->str_const << $4->type << std::endl;
    //insert_indexed_map($2, $4);

    $$ = make_indexed_elem($2,$4);
}
;

indexed: indexedelem {
            //printf("elist -> expression\n");
            $1->next = NULL;
            $$ = $1;
        }
        | indexed TOK_COMMA indexedelem {
            //printf("elist -> elist,\n");
            $3->next = $1;
            $$ = $3;
        }
;

objectdef: TOK_L_BR elist TOK_R_BR {
            $$ = manage_lbr_elist_rbr($$,$2);
          }
         | TOK_L_BR indexed TOK_R_BR {
            $$ = manage_lbr_indexed_rbr($$,$2);
         }
;

block: TOK_L_CURLY_BR {
    push_blocks_prec(1);
    push_vector();
    is_in_block_mode++;
    scope_counter++;
    set_prev_scope_space_counter(get_scope_space_counter());
    set_scope_space_counter(1);
/*std::cout<<scope_counter<<std::endl;*/} statements TOK_R_CURLY_BR
{/*printf("block -> {statements}\n");*/ is_in_block_mode--;handle_block_end(get_scope()); printf("Prev Scope Space Counter: %d\n", get_prev_scope_space_counter()); set_scope_space_counter(get_prev_scope_space_counter()); printf("Scope Space Counter: %d\n", get_scope_space_counter()); scope_counter--;
/*std::cout<< "Scope Counter is " << scope_counter<<std::endl;*/}
;

funcdef_block: TOK_L_CURLY_BR statements TOK_R_CURLY_BR {
        reset_formal_arg_offset();
    }
;

funcname: TOK_ID {
    $$ = $1;
}
;

funcprefix: TOK_FUNCTION funcname {
        is_in_function_mode++;
        push_state_stack("not loop");
        $$ = manage_funcprefix($$, $2, yylineno);
        push_returnlist();
        scope_counter++;
    } | TOK_FUNCTION {
        is_in_function_mode++;
        push_state_stack("not loop");
        $$ = manage_funcprefix_anonym($$, yylineno);
        push_returnlist();
        scope_counter++;
    }
;

funcargs: TOK_L_PARENTH idlist TOK_R_PARENTH {
        push_blocks_prec(2);
        set_scope_space_counter(2);
        push_func_local_offset_table();
    }
;

funcbody: funcdef_block {
        $$ = curr_scope_offset();
        set_scope_space_counter(get_prev_scope_space_counter());
        pop_func_local_offset_table();
        reset_func_local_offset();
    }
;

funcdef: funcprefix funcargs funcbody {
        patchlist_returnlist(next_quad_label());
        $$ = manage_funcdef($1, $3);
        
        scope_counter--;
        is_in_function_mode--;
        
        pop_returnlist();
        pop_state_stack();
        
    }
;

const: TOK_INT {
            $$ = make_constnum_expression(CONSTNUM_EXPR, $1);
        }
       | TOK_FLOAT {
            $$ = make_constnum_expression(CONSTNUM_EXPR, $1);
        }
       | TOK_STRING {
            $$ = make_conststring_expression(CONSTSTRING_EXPR, $1);
        }
       | TOK_NIL {
            $$ = make_nil_expression(NIL_EXPR);
        }
       | TOK_TRUE {
            $$ = make_constbool_expression(CONSTBOOL_EXPR, 1);
        }
       | TOK_FALSE {
            $$ = make_constbool_expression(CONSTBOOL_EXPR, 0);
        }
;

idlist: TOK_ID {
                    
                    handle_func_w_1arg($1, get_scope(), yylineno, curr_scope_space(), curr_scope_offset());

               }
        | idlist TOK_COMMA TOK_ID {
                                      
                                       handle_func_w_1arg($3, get_scope(), yylineno, curr_scope_space(), curr_scope_offset());
                                  }
        | %empty { push_vector();}
;

ifprefix: TOK_IF TOK_L_PARENTH expression TOK_R_PARENTH {
    if (!$3->truelist.empty() || !$3->falselist.empty()) {
        SymbolType type = GLOBAL;
        if (get_scope() != 0) {
            type = LOCAL;
        }
        SymbolTableEntry* new_temp = new_temp_var(-1, type);
        expression* result = make_assign_expression(ASSIGN_EXPR, get_symbol(new_temp->name, get_scope()));

    
        for (auto& it : $3->truelist) {
            std::cout << "TRUELIST: " << it << std::endl;
            backpatch(it, next_quad_label());
        }
    
        
        for (auto& it : $3->falselist) {
            std::cout << "FALSE LIST: " << it << std::endl;
            backpatch(it, next_quad_label() + 2);
        }
    

        emit_assign(ASSIGN, result, make_constbool_expression(CONSTBOOL_EXPR, 1));
        emit_jump(JUMP, (next_quad_label() + 2));
        set_curr_quad_label((next_quad_label() + 2));
        emit_assign(ASSIGN, result, make_constbool_expression(CONSTBOOL_EXPR, 0));
        $3 = result;
        $$ = manage_ifprefix($$, $3); // Emit IF_EQ and JUMP quads
    } else {
        $$ = manage_ifprefix($$, $3); // Emit IF_EQ and JUMP quads
    }
}
;

elseprefix: TOK_ELSE {
    $$ = next_quad_label(); // Record the label for the JUMP quad
    emit_jump(JUMP, 0);     // Emit a JUMP quad with a placeholder label
}
;

if_statement: ifprefix statement {
    
    patchlabel($1, next_quad_label()); // Patch the JUMP quad from ifprefix
    $$ = $1; // Return the label of the patched JUMP quad
    
}
;

ifelse_statement: ifprefix statement elseprefix statement {
    patchlabel($1, $3+1); // Patch the jump from ifprefix to the start of the else block
    patchlabel($3, next_quad_label()); // Patch the jump from elseprefix to the end of the else block
    $$ = $1; // Return the label from ifprefix
}
;

whilestart: TOK_WHILE {
    $$ = next_quad_label();
    push_breaklist();
    push_contlist();
}
;

whilecond: TOK_L_PARENTH expression TOK_R_PARENTH {
    if (!$2->truelist.empty() || !$2->falselist.empty()) {
        SymbolType type = GLOBAL;
        if (get_scope() != 0) {
            type = LOCAL;
        }
        SymbolTableEntry* new_temp = new_temp_var(-1, type);
        expression* result = make_assign_expression(ASSIGN_EXPR, get_symbol(new_temp->name, get_scope()));

    
        for (auto& it : $2->truelist) {
            std::cout << "TRUELIST: " << it << std::endl;
            backpatch(it, next_quad_label());
        }
    
        
        for (auto& it : $2->falselist) {
            std::cout << "FALSE LIST: " << it << std::endl;
            backpatch(it, next_quad_label() + 2);
        }
    
        emit_assign(ASSIGN, result, make_constbool_expression(CONSTBOOL_EXPR, 1));
        emit_jump(JUMP, (next_quad_label() + 2));
        set_curr_quad_label((next_quad_label() + 2));
        emit_assign(ASSIGN, result, make_constbool_expression(CONSTBOOL_EXPR, 0));
        $2 = result;
        emit_if_equal(IF_EQ, $2, make_constbool_expression(CONSTBOOL_EXPR, 1), next_quad_label()+2);
        $$ = next_quad_label();
        emit_jump(JUMP, 0);
        push_state_stack("loop");
    } else {
        emit_if_equal(IF_EQ, $2, make_constbool_expression(CONSTBOOL_EXPR, 1), next_quad_label()+2);
        $$ = next_quad_label();
        emit_jump(JUMP, 0);
        push_state_stack("loop");
    }
}
;

whileblock: TOK_L_CURLY_BR statements TOK_R_CURLY_BR {
    $$ = $2;
} 
;

while_statement: whilestart whilecond statement {
    //std::cout << "Top of State Stack: " << state_stack_top() << std::endl; 
    emit_jump(JUMP, $1);
    patchlabel($2, next_quad_label());
    //std::cout<< "contlist Size: " << contlist_size() << std::endl;
    patchlist_breaklist(next_quad_label());
    patchlist_contlist($1);
    pop_breaklist();
    pop_contlist();
    pop_state_stack();
}
;

N: {
    $$ = next_quad_label();
    emit_jump(JUMP, 0);
}
;

M1: {
    $$ = next_quad_label();
}
;

forprefix: TOK_FOR TOK_L_PARENTH elist TOK_SEMICOLON M1 expression TOK_SEMICOLON {
    if (!$6->truelist.empty() || !$6->falselist.empty()) {
        SymbolType type = GLOBAL;
        if (get_scope() != 0) {
            type = LOCAL;
        }
        SymbolTableEntry* new_temp = new_temp_var(-1, type);
        expression* result = make_assign_expression(ASSIGN_EXPR, get_symbol(new_temp->name, get_scope()));

        for (auto& it : $6->truelist) {
            std::cout << "TRUELIST: " << it << std::endl;
            backpatch(it, next_quad_label());
        }

        for (auto& it : $6->falselist) {
            std::cout << "FALSE LIST: " << it << std::endl;
            backpatch(it, next_quad_label() + 2);
        }

        emit_assign(ASSIGN, result, make_constbool_expression(CONSTBOOL_EXPR, 1));
        emit_jump(JUMP, (next_quad_label() + 2));
        set_curr_quad_label((next_quad_label() + 2));
        emit_assign(ASSIGN, result, make_constbool_expression(CONSTBOOL_EXPR, 0));

    }


    $$.test = $5;
    $$.enter = next_quad_label();
    emit_if_equal(IF_EQ, $6, make_constbool_expression(CONSTBOOL_EXPR, 1), 0);
    push_breaklist();
    push_contlist();
    push_state_stack("loop");
}
;

forblock: TOK_L_CURLY_BR statements TOK_R_CURLY_BR {
    $$ = $2;
}

for_statement: forprefix N elist TOK_R_PARENTH N statement N               {
    std::cout << "Enter : " << $1.enter << std::endl;
    patchlabel($1.enter, $5+1);
    patchlabel($2, next_quad_label());
    patchlabel($5, $1.test);
    patchlabel($7, $2+1);
    patchlist_breaklist(next_quad_label());
    patchlist_contlist($1.enter+2);
    pop_breaklist();
    pop_contlist();
    pop_state_stack();
}
;

return_statement: TOK_RETURN TOK_SEMICOLON                                 {
    emit_return(RET, NULL);
    add_returnelem(next_quad_label(), 0);
    emit_jump(JUMP, 0);
}
                  | TOK_RETURN expression TOK_SEMICOLON
                                                                           {
    emit_return(RET, $2);
    add_returnelem(next_quad_label(), 0);                                                                        
    emit_jump(JUMP, 0);                                                                      
    }
;
%%

int main(int argc, char** argv) {
    //struct alpha_token_t *token = (struct
//alpha_token_t*)malloc(sizeof(struct alpha_token_t));
    init_vector();
    if (argc > 1) {
        if (!(yyin = fopen(argv[1], "r"))) {
            fprintf(stderr, "This file is not readable: %s\n", argv[1]);
            return 1;
        } else {
            yyparse();
        }
    }
    //print_symtable();
    //print_symtable_inactive();
    std::cout << std::endl;
    //quad_table_print();
    //printf("\n");
    //print_symtable();
    //print_symtable_inactive();
    //printf("\n");
    //print_expression_list();
    //printf("\n");
    //print_reversed_expression_list();
    //quad_table_print();
    generate_tcode();
    print_instruction_table();
    avm_binary_file("tcode.abc");
    avm_binary_dump("tcode.abc");
    return 0;
}

void yyerror(const char* s) {
    fprintf(stderr, "Parse error: %s at line %d\n", s, yylineno);
    exit(1);
}
