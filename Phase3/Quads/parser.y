%{
    #include "symbol_table.h"
    #include "handlers.h"
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
}

%union {
    int intVal;
    double floatVal;
    char* strVal;
    expression* expr;
    struct Call call_struct;
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
%type <expr> funcdef
%type <expr> objectdef
%type <expr> term
%type <expr> indexed
%type <expr> indexedelem

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

program: statements statement //{printf("program -> statements statement\n");}
         | %empty //{printf("program -> \n");}
;

statement: expression TOK_SEMICOLON //{printf("statement -> expression;\n");}
          | if_statement //{printf("statement -> if_statement\n");}
          | while_statement //{printf("statement -> while_statement\n");}
          | for_statement //{printf("statement -> for_statement\n");}
          | return_statement //{printf("statement -> return_statement\n");}
          | TOK_CONTINUE TOK_SEMICOLON //{printf("statement -> continue;\n");}
          | TOK_BREAK TOK_SEMICOLON //{printf("statement -> break;\n");}
          | block //{printf("statement -> block\n");}
          | funcdef //{printf("statement -> funcdef\n");}
          | TOK_SEMICOLON //{printf("statement -> ;\n");}
          | error TOK_SEMICOLON {yyerrok; yyclearin;}
;

statements: statements statement //{printf("\nstatements -> statements
//statement\n");}
            | %empty //{printf("statements -> statements statement\n");}
;

expression: assignexpr //{printf("expression -> assignexpr\n");}
            | expression TOK_PLUS expression {
                    //printf("expression -> expression + expression\n");
                    //handle_expression($1,$3, get_scope(), yylineno));
                    if (check_if_is_arithm($1) == true && check_if_is_arithm($3) == true) {
                        SymbolType type = GLOBAL;
                        if (get_scope() != 0) {
                            type = LOCAL;
                        }
                        SymbolTableEntry new_temp = new_temp_var(-1, type);
                        //std::cout << new_temp.scope << std::endl;
                        $$ = make_arithm_expression(ARITHM_EXPR, get_symbol(new_temp.name, get_scope()));
                        std::cout << "$1->type " << $1->type << std::endl;
                        emit_add(ADD, $$, $1, $3);
                    }
                    
                }
            | expression TOK_MINUS expression {
                    //printf("expression -> expression - expression\n");
                    //handle_expression($1,$3, get_scope(), yylineno);
                    if (check_if_is_arithm($1) == true && check_if_is_arithm($3) == true) {
                        SymbolType type = GLOBAL;
                        if (get_scope() != 0) {
                            type = LOCAL;
                        }
                        SymbolTableEntry new_temp = new_temp_var(-1, type);
                        //std::cout << new_temp.scope << std::endl;
                        $$ = make_arithm_expression(ARITHM_EXPR, get_symbol(new_temp.name, get_scope()));
                        std::cout << "$1->type " << $1->type << std::endl;
                        emit_sub(SUB, $$, $1, $3);
                    }
                    
                }
            | expression TOK_MULTIPLY expression {
                    //printf("expression -> expression * expression\n");
                    //handle_expression($1,$3, get_scope(), yylineno);
                    if (check_if_is_arithm($1) == true && check_if_is_arithm($3) == true) {
                        SymbolType type = GLOBAL;
                        if (get_scope() != 0) {
                            type = LOCAL;
                        }
                        SymbolTableEntry new_temp = new_temp_var(-1, type);
                        //std::cout << new_temp.scope << std::endl;
                        $$ = make_arithm_expression(ARITHM_EXPR, get_symbol(new_temp.name, get_scope()));
                        std::cout << "$1->type " << $1->type << std::endl;
                        emit_mul(MUL, $$, $1, $3);
                    }
                }
            | expression TOK_DIVIDE expression {
                    //printf("expression -> expression / expression\n");
                    //handle_expression($1,$3, get_scope(), yylineno);
                    if (check_if_is_arithm($1) == true && check_if_is_arithm($3) == true) {
                        SymbolType type = GLOBAL;
                        if (get_scope() != 0) {
                            type = LOCAL;
                        }
                        SymbolTableEntry new_temp = new_temp_var(-1, type);
                        //std::cout << new_temp.scope << std::endl;
                        $$ = make_arithm_expression(ARITHM_EXPR, get_symbol(new_temp.name, get_scope()));
                        std::cout << "$1->type " << $1->type << std::endl;
                        emit_div(DIV, $$, $1, $3);
                    }
                }
            | expression TOK_MODULO expression {
                    //printf("expression -> expression modulo expression\n");
                    //handle_expression($1,$3, get_scope(), yylineno);
                    if (check_if_is_arithm($1) == true && check_if_is_arithm($3) == true) {
                        SymbolType type = GLOBAL;
                        if (get_scope() != 0) {
                            type = LOCAL;
                        }
                        SymbolTableEntry new_temp = new_temp_var(-1, type);
                        //std::cout << new_temp.scope << std::endl;
                        $$ = make_arithm_expression(ARITHM_EXPR, get_symbol(new_temp.name, get_scope()));
                        std::cout << "$1->type " << $1->type << std::endl;
                        emit_mod(MOD, $$, $1, $3);
                    }
                }
            | expression TOK_GREATER expression {
                    //printf("expression -> expression > expression\n");
                    //handle_expression($1,$3, get_scope(), yylineno);
                    SymbolType type = GLOBAL;
                    if (get_scope() != 0) {
                        type = LOCAL;
                    }
                    SymbolTableEntry new_temp = new_temp_var(-1, type);
                    //std::cout << new_temp.scope << std::endl;
                    $$ = make_bool_expression(BOOL_EXPR, get_symbol(new_temp.name, get_scope()));
                    emit_if_greater(IF_GR, $1, $3, next_quad_label()+3);
                    set_curr_quad_label((next_quad_label()+3));
                    emit_assign(ASSIGN, $$, make_constbool_expression(CONSTBOOL_EXPR, 1));
                    emit_jump(JUMP, (next_quad_label()+2));
                    set_curr_quad_label((next_quad_label()+2));
                    emit_assign(ASSIGN, $$, make_constbool_expression(CONSTBOOL_EXPR, 0));
                }
            | expression TOK_LESS expression {
                    //printf("expression -> expression < expression\n");
                    //handle_expression($1,$3, get_scope(), yylineno);
                    SymbolType type = GLOBAL;
                    if (get_scope() != 0) {
                        type = LOCAL;
                    }
                    SymbolTableEntry new_temp = new_temp_var(-1, type);
                    //std::cout << new_temp.scope << std::endl;
                    $$ = make_bool_expression(BOOL_EXPR, get_symbol(new_temp.name, get_scope()));
                    emit_if_less(IF_L, $1, $3, next_quad_label()+3);
                    set_curr_quad_label((next_quad_label()+3));
                    emit_assign(ASSIGN, $$, make_constbool_expression(CONSTBOOL_EXPR, 1));
                    emit_jump(JUMP, (next_quad_label()+2));
                    set_curr_quad_label((next_quad_label()+2));
                    emit_assign(ASSIGN, $$, make_constbool_expression(CONSTBOOL_EXPR, 0));
                }
            | expression TOK_GR_EQUAL expression { 
                    //printf("expression -> expression >= expression\n");
                    //handle_expression($1,$3, get_scope(), yylineno);
                    SymbolType type = GLOBAL;
                    if (get_scope() != 0) {
                        type = LOCAL;
                    }
                    SymbolTableEntry new_temp = new_temp_var(-1, type);
                    //std::cout << new_temp.scope << std::endl;
                    $$ = make_bool_expression(BOOL_EXPR, get_symbol(new_temp.name, get_scope()));
                    emit_if_greatereq(IF_GR_EQ, $1, $3, next_quad_label()+3);
                    set_curr_quad_label((next_quad_label()+3));
                    emit_assign(ASSIGN, $$, make_constbool_expression(CONSTBOOL_EXPR, 1));
                    emit_jump(JUMP, (next_quad_label()+2));
                    set_curr_quad_label((next_quad_label()+2));
                    emit_assign(ASSIGN, $$, make_constbool_expression(CONSTBOOL_EXPR, 0));
                }
            | expression TOK_LS_EQUAL expression {
                    //printf("expression -> expression <= expression\n");
                    //handle_expression($1,$3, get_scope(), yylineno);
                    SymbolType type = GLOBAL;
                    if (get_scope() != 0) {
                        type = LOCAL;
                    }
                    SymbolTableEntry new_temp = new_temp_var(-1, type);
                    //std::cout << new_temp.scope << std::endl;
                    $$ = make_bool_expression(BOOL_EXPR, get_symbol(new_temp.name, get_scope()));
                    emit_if_lesseq(IF_L_EQ, $1, $3, next_quad_label()+3);
                    set_curr_quad_label((next_quad_label()+3));
                    emit_assign(ASSIGN, $$, make_constbool_expression(CONSTBOOL_EXPR, 1));
                    emit_jump(JUMP, (next_quad_label()+2));
                    set_curr_quad_label((next_quad_label()+2));
                    emit_assign(ASSIGN, $$, make_constbool_expression(CONSTBOOL_EXPR, 0));
                }
            | expression TOK_EQUAL expression {
                    //printf("expression -> expression == expression\n");
                    //handle_expression($1,$3, get_scope(), yylineno);
                    SymbolType type = GLOBAL;
                    if (get_scope() != 0) {
                        type = LOCAL;
                    }
                    SymbolTableEntry new_temp = new_temp_var(-1, type);
                    //std::cout << new_temp.scope << std::endl;
                    $$ = make_bool_expression(BOOL_EXPR, get_symbol(new_temp.name, get_scope()));
                    emit_if_equal(IF_EQ, $1, $3, next_quad_label()+3);
                    set_curr_quad_label((next_quad_label()+3));
                    emit_assign(ASSIGN, $$, make_constbool_expression(CONSTBOOL_EXPR, 1));
                    emit_jump(JUMP, (next_quad_label()+2));
                    set_curr_quad_label((next_quad_label()+2));
                    emit_assign(ASSIGN, $$, make_constbool_expression(CONSTBOOL_EXPR, 0));
                }
            | expression TOK_N_EQUAL expression {
                    //printf("expression -> expression != expression\n");
                    //handle_expression($1,$3, get_scope(), yylineno);
                    SymbolType type = GLOBAL;
                    if (get_scope() != 0) {
                        type = LOCAL;
                    }
                    SymbolTableEntry new_temp = new_temp_var(-1, type);
                    //std::cout << new_temp.scope << std::endl;
                    $$ = make_bool_expression(BOOL_EXPR, get_symbol(new_temp.name, get_scope()));
                    emit_if_not_equal(IF_N_EQ, $1, $3, next_quad_label()+3);
                    set_curr_quad_label((next_quad_label()+3));
                    emit_assign(ASSIGN, $$, make_constbool_expression(CONSTBOOL_EXPR, 1));
                    emit_jump(JUMP, (next_quad_label()+2));
                    set_curr_quad_label((next_quad_label()+2));
                    emit_assign(ASSIGN, $$, make_constbool_expression(CONSTBOOL_EXPR, 0));
                }
            | expression TOK_AND expression {
                    //printf("expression -> expression and expression\n");
                    //handle_expression($1,$3, get_scope(), yylineno);
                }
            | expression TOK_OR expression {
                    //printf("expression -> expression or expression\n");
                    //handle_expression($1,$3, get_scope(), yylineno);
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
      | TOK_MINUS expression %prec TOK_UMINUS {
            //printf("term -> - expression\n");
            if (check_if_is_arithm($2)) {
                SymbolType type = GLOBAL;
                if (get_scope() != 0) {
                    type = LOCAL;
                }
                SymbolTableEntry new_temp = new_temp_var(-1, type);
                std::cout << new_temp.scope << std::endl;
                $$ = make_arithm_expression(ARITHM_EXPR, get_symbol(new_temp.name, get_scope()));
                emit_arithm_one(UMINUS, $$, $2);
            }
      }
      | TOK_NOT expression  {
            //printf("term -> not expression\n");
            SymbolType type = GLOBAL;
            if (get_scope() != 0) {
                type = LOCAL;
            }
            SymbolTableEntry new_temp = new_temp_var(-1, type);
            std::cout << new_temp.scope << std::endl;
            $$ = make_bool_expression(BOOL_EXPR, get_symbol(new_temp.name, get_scope()));
            emit_bool(NOT, $$, $2);
      }
      | TOK_DBL_PLUS lvalue {
            /*printf("term -> ++lvalue\n");*/ 
            if (handle_assign_expr($2->symbol->name, get_scope())) {
                if (check_if_is_arithm($2)) {
                    //std::cout << "--L-VALUE" << std::endl;
                    if ($2->type == TABLEITEM_EXPR) {
                        $$ = emit_if_table_item($2);
                        emit_add(ADD, $$, $$, make_constnum_expression(CONSTNUM_EXPR, 1));
                        emit_table_set_item(TABLESETELEM, $2, $2->index, $$);
                    } else {
                        //std::cout << "EMIT ADD 1" << std::endl;
                        emit_add(ADD, $2, $2, make_constnum_expression(CONSTNUM_EXPR, 1));
                        SymbolType type = GLOBAL;
                            if (get_scope() != 0) {
                                type = LOCAL;
                            }
                        SymbolTableEntry new_temp = new_temp_var(-1, type);
                        std::cout << new_temp.scope << std::endl;
                        $$ = make_arithm_expression(ARITHM_EXPR, get_symbol(new_temp.name, get_scope()));
                        emit_assign(ASSIGN, $$, $2);
                        
                    }
                }
            }
      }
      | lvalue TOK_DBL_PLUS {
            /*printf("term -> lvalue++\n");*/ 
            if (handle_assign_expr($1->symbol->name,get_scope())) {
                if (check_if_is_arithm($1)) {
                    SymbolType type = GLOBAL;
                    if (get_scope() != 0) {
                        type = LOCAL;
                    }
                    SymbolTableEntry new_temp = new_temp_var(-1, type);
                    std::cout << new_temp.scope << std::endl;
                    $$ = make_lvalue_expression(VAR_EXPR, get_symbol(new_temp.name, get_scope()));
                    if ($1->type == TABLEITEM_EXPR) {
                        expression* val = emit_if_table_item($1);
                        emit_assign(ASSIGN, $$, val);
                        emit_add(ADD, val, val, make_constnum_expression(CONSTNUM_EXPR, 1));
                        emit_table_set_item(TABLESETELEM, $1, $1->index, val);
                    } else {
                        std::cout << "EMIT ASSIGN 1" << std::endl;
                        emit_assign(ASSIGN, $$, $1);
                        emit_add(ADD, $1, $1, make_constnum_expression(CONSTNUM_EXPR, 1));
                    }
                }
            }
      }
      | TOK_DBL_MINUS lvalue {
            /*printf("term -> --lvalue\n");*/ 
            if (handle_assign_expr($2->symbol->name, get_scope())) {
                if (check_if_is_arithm($2)) {
                    //std::cout << "--L-VALUE" << std::endl;
                    if ($2->type == TABLEITEM_EXPR) {
                        $$ = emit_if_table_item($2);
                        emit_sub(SUB, $$, $$, make_constnum_expression(CONSTNUM_EXPR, 1));
                        emit_table_set_item(TABLESETELEM, $2, $2->index, $$);
                    } else {
                        //std::cout << "EMIT ADD 1" << std::endl;
                        emit_sub(SUB, $2, $2, make_constnum_expression(CONSTNUM_EXPR, 1));
                        SymbolType type = GLOBAL;
                        if (get_scope() != 0) {
                            type = LOCAL;
                        }
                        SymbolTableEntry new_temp = new_temp_var(-1, type);
                        std::cout << new_temp.scope << std::endl;
                        $$ = make_arithm_expression(ARITHM_EXPR, get_symbol(new_temp.name, get_scope()));
                        emit_assign(ASSIGN, $$, $2);
                        
                    }
                }
            }
      }
      | lvalue TOK_DBL_MINUS {
            /*printf("term -> lvalue--\n");*/ 
            if (handle_assign_expr($1->symbol->name,get_scope())) {
                if (check_if_is_arithm($1)) {
                    SymbolType type = GLOBAL;
                    if (get_scope() != 0) {
                        type = LOCAL;
                    }
                    SymbolTableEntry new_temp = new_temp_var(-1, type);
                    std::cout << new_temp.scope << std::endl;
                    $$ = make_lvalue_expression(VAR_EXPR, get_symbol(new_temp.name, get_scope()));
                    if ($1->type == TABLEITEM_EXPR) {
                        expression* val = emit_if_table_item($1);
                        emit_assign(ASSIGN, $$, val);
                        emit_sub(SUB, val, val, make_constnum_expression(CONSTNUM_EXPR, 1));
                        emit_table_set_item(TABLESETELEM, $1, $1->index, val);
                    } else {
                        std::cout << "EMIT ASSIGN 1" << std::endl;
                        emit_assign(ASSIGN, $$, $1);
                        emit_sub(SUB, $1, $1, make_constnum_expression(CONSTNUM_EXPR, 1));
                    }
                }
            }
      }
      | primary {
            printf("term -> primary\n");
            $$ = $1;
            //std::cout << "$$->type " << $$->type << "\t\t" << $$->symbol->name << std::endl;
      }
;

assignexpr: lvalue TOK_ASSIGN expression {
        printf("assignexpr -> lvalue = expression\n");
        //printf("%s\n", $1->name);
        std::cout << "$3->type " << $3->type << "\t\t" << $3->num_const << std::endl;
        if (handle_assign_expr($1->symbol->name, get_scope())) {
            std::cout << "ASSIGN EXPR $$$" << std::endl;
            if ($1->type == TABLEITEM_EXPR) {
                //std::cout << "Expression has symbol " << $3->symbol->name << std::endl;
                emit_table_set_item(TABLESETELEM, $1, $1->index, $3);
                $$ = emit_if_table_item($1);
                $$->type = ASSIGN_EXPR;
                //std::cout << "ASSIGN EXPR 2" << std::endl;
            } else {
                std::cout << "ASSIGN EXPR $$$$" << std::endl;
                std::cout << "IN ASSIGN L-VALUE Name is "<< $1->symbol->name << std::endl;
                //std::cout << "IN ASSIGN EXPRESSION is " << $3->symbol->name << std::endl;
                if (check_if_is_arithm_alt($1) == true && check_if_is_arithm_alt($3) == true) {
                    std::cout << "ASSIGN EXPR $$$$$" << std::endl;
                    emit_assign(ASSIGN, $1, $3);

                    SymbolType type = GLOBAL;
                    if (get_scope() != 0) {
                        type = LOCAL;
                    }
                    SymbolTableEntry new_temp = new_temp_var(-1, type);
                    std::cout << new_temp.scope << std::endl;
                    $$ = make_assign_expression(ASSIGN_EXPR, get_symbol(new_temp.name, get_scope()));
                    //std::cout << $$->symbol->name << std::endl;
                    //std::cout << $1->type << std::endl;
                    emit_assign(ASSIGN, $$, $1);
                    if (get_scope() > 0) {
                        quad_table_print();
                    }
                } else {
                    std::cout << "Error r-value is not an arithmetic expression" << std::endl;
                }
            }
        }
                                         }
;

primary: lvalue {
                        //printf("primary -> lvalue\n");
                        $$ = emit_if_table_item($1);
                  }
         | call //{printf("primary -> call\n");}
         | objectdef {
            printf("primary -> objectdef\n");
            std::cout << "OBJECTDEF in PRIMARY" << $1->symbol->name << std::endl;
            $$ = $1;
         }
         | TOK_L_PARENTH funcdef TOK_R_PARENTH {
            //printf("primary ->(funcdef)\n");
            //std::cout << $2->symbol->name << std::endl;
            //quad_table_print();
            $$ = $2;
         }
         | const {
            printf("primary -> const\n");
            $$ = $1;
         }
;

lvalue: TOK_ID {//printf("lvalue -> ID\n"); 
                    handle_id($1, get_scope(), curr_scope_space(), curr_scope_offset(), yylineno);
                    if (get_symbol($1, get_scope())->type == LIBFUNC) {
                        $$ = make_lvalue_expression(LIBFUNC_EXPR, get_symbol($1, get_scope()));

                    } else if (get_symbol($1, get_scope())->type == USERFUNC) {
                        $$ = make_lvalue_expression(PROGRAMFUNC_EXPR, get_symbol($1, get_scope()));
                    } else {
                        $$ = make_lvalue_expression(VAR_EXPR, get_symbol($1, get_scope()));
                    }
               }
        | TOK_LOCAL TOK_ID {//printf("lvalue -> local id\n");
                    handle_local_id($2, get_scope(), curr_scope_space(), curr_scope_offset(), yylineno);
                    if (get_symbol($2, get_scope())->type == LIBFUNC) {
                        $$ = make_lvalue_expression(LIBFUNC_EXPR, get_symbol($2, get_scope()));
                    } else if (get_symbol($2, get_scope())->type == USERFUNC) {
                        $$ = make_lvalue_expression(PROGRAMFUNC_EXPR, get_symbol($2, get_scope()));
                    } else {
                        $$ = make_lvalue_expression(VAR_EXPR, get_symbol($2, get_scope()));
                    }
                           }
        | TOK_DBL_COLON TOK_ID  {//printf("lvalue -> :: id\n");
                                    handle_global_access_id($2);
                                    $$ = make_lvalue_expression(VAR_EXPR, get_symbol($2,0));
                                    /*$$ = (SymbolTableEntry*)malloc(sizeof(SymbolTableEntry));
                                    $$->name = $2;
                                    $$->scope = 0;
                                    $$->line = yylineno;
                                    $$->isActive = true;
                                    $$->type = 1;*/
                                }
        | member {
                    //printf("lvalue -> member\n");
                    is_a_member_access = 1;
                    $$ = $1;

                 }
;

member: lvalue TOK_DOT TOK_ID {
            printf("member -> lvalue.id\n");
            std::cout << "$3 -> " << $3 << std::endl;
            //$$ = member_item($1, get_symbol($3, get_scope())->name);
            $$ = member_item($1, $3);
            //std::cout << "$$->index->str_const " << $$->index->str_const << std::endl;
            is_a_member_access = 0;
        }
        | lvalue TOK_L_BR expression TOK_R_BR  {
            //printf("member -> lvalue[expression]\n");
            std::cout << "Expression is : " << $3->type << std::endl;
            $1 = emit_if_table_item($1);
            if ($3->type == CONSTNUM_EXPR) {
                $$ = make_table_item_expression(TABLEITEM_EXPR, $1->symbol, make_constnum_expression(CONSTNUM_EXPR, $3->num_const));
            } else {
                $$ = make_table_item_expression(TABLEITEM_EXPR, $1->symbol, make_conststring_expression(CONSTSTRING_EXPR, $3->symbol->name));
            }
            clear_expression_list();
        }
        | call TOK_DOT TOK_ID //{printf("member -> call.id\n");}
        | call TOK_L_BR expression TOK_R_BR {
            printf("member -> call[expression]\n");
        }
;

call: call normcall {
            //printf("call -> call normcall\n");
            $$ = make_call($1);
       }
      | lvalue callsuffix {
            printf("call -> lvalue callsuffix\n");
            $1 = emit_if_table_item($1);
            std::cout << "CALLSUFFIX is " << $1->symbol->name << std::endl; 
            if ($2.method == 0) {
                std::cout << "$2 Method is " << $2.method << std::endl; 
                if ($1->type == LIBFUNC_EXPR || $1->type == PROGRAMFUNC_EXPR || $1->type == VAR_EXPR) {
                    //std::cout << "$2 Method is and $1->type is LIBFUNC_EXPR " << $2.method << std::endl; 
                    //$1 = emit_if_table_item(member_item($1, ""));

                } else {
                    std::cout << "L-VALUE is " << $1->symbol->name << std::endl;
                    push_expression_list($1);
                    get_last_expression();
                    $1 = emit_if_table_item(member_item($1, $1->index->str_const));
                }
            } else {
                std::cout << "$1->symbol->name " << $1->symbol->name << std::endl;
                push_expression_list($1);
                get_last_expression();
                $1 = emit_if_table_item(member_item($1, $2.name));
                
            }
            $$ = make_call($1);
            if (get_scope() > 0) {
                quad_table_print();
            }
      }
      | TOK_L_PARENTH funcdef TOK_R_PARENTH normcall {
            //printf("call -> (funcdef) normcall\n");
            expression* func = make_func_expression(PROGRAMFUNC_EXPR, get_symbol($2->symbol->name, get_scope()));
            $$ = make_call(func);     
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
    $$.elist = $2;
    $$.method = 0;
    $$.name = NULL;

    get_last_expression();
    //pop_expression_list();
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

elist: expression {
            //printf("elist -> expression\n");
            push_expression_list($1);
        }
       | elist TOK_COMMA expression {
            //printf("elist -> elist,\n");
            push_expression_list($3);
        }
       |  %empty {
            //printf("elist -> \n");
            //push_expression_list(NULL);
        }
;

indexed: indexedelem {
            //printf("elist -> expression\n");
            $$ = $1;
        }
       | indexed TOK_COMMA indexedelem {
            //printf("elist -> elist,\n");
            $$ = $1;
        }
;

objectdef: TOK_L_BR elist TOK_R_BR {
                std::cout << "OBJECTDEF" << std::endl;
                //printf("objectdef -> [elist]\n");
                SymbolType type = GLOBAL;
                if (get_scope() != 0) {
                    type = LOCAL;
                }
                SymbolTableEntry new_temp = new_temp_var(-1, type);
                //std::cout << new_temp.name << std::endl;
                expression* table = make_new_table_expression(NEWTABLE_EXPR, get_symbol(new_temp.name, get_scope()));
                emit_table_create(TABLECREATE, table);
                set_elist_expression(table);
                $$ = table;
          }
         | TOK_L_BR indexed TOK_R_BR {
                printf("objectdef -> [indexed]\n");
                SymbolType type = GLOBAL;
                if (get_scope() != 0) {
                    type = LOCAL;
                }
                SymbolTableEntry new_temp = new_temp_var(-1, type);
                std::cout << new_temp.name << std::endl;
                expression* table = make_new_table_expression(NEWTABLE_EXPR, get_symbol(new_temp.name, get_scope()));
                emit_table_create(TABLECREATE, table);
                set_indexed_map(table);
                $$ = table;

         }
;

indexedelem: TOK_L_CURLY_BR expression TOK_COLON expression 
TOK_R_CURLY_BR {
    printf("indexedelem -> {expression:expression}\n");
    std::cout << $2->str_const << $4->type << std::endl;
    insert_indexed_map($2,$4);
    $$ = $2;
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
{/*printf("block -> {statements}\n");*/ is_in_block_mode--; handle_block_end(get_scope()); printf("Prev Scope Space Counter: %d\n", get_prev_scope_space_counter()); set_scope_space_counter(get_prev_scope_space_counter()); printf("Scope Space Counter: %d\n", get_scope_space_counter()); scope_counter--;
/*std::cout<< "Scope Counter is " << scope_counter<<std::endl;*/}
;

funcdef_block: TOK_L_CURLY_BR {push_blocks_prec(2);set_scope_space_counter(2); push_func_local_offset_table();} statements TOK_R_CURLY_BR {
        /*printf("funcdef_block -> {statements}\n");*/
        
        handle_funcdef_block_end(get_scope());
        //scope_counter--; 
        is_in_function_mode = 0;
        //std::cout<<scope_counter<<std::endl;
        printf("The current scope space is: %d\n", curr_scope_space());

        reset_formal_arg_offset();
    }
;

funcdef: TOK_FUNCTION TOK_ID TOK_L_PARENTH {
    
    is_in_function_mode = 1;
    handle_funcdef_w_name($2, get_scope(), curr_scope_space(), curr_scope_offset(), yylineno);
    //printf("%s", get_symbol($2, get_scope())->name);
    //$$ = make_func_expression(PROGRAMFUNC_EXPR, get_symbol($2, get_scope()));
    emit_funcdef(FUNCSTART, make_func_expression(PROGRAMFUNC_EXPR, get_symbol($2, get_scope())));
    scope_counter++;
    set_prev_scope_space_counter(get_scope_space_counter());
    set_scope_space_counter(3);
    push_formal_arg_offset_table();
    /*std::cout<<scope_counter<<std::endl;*/} idlist TOK_R_PARENTH
funcdef_block {
                   //printf("funcdef -> function id (idlist) funcdef_block\n");
                   //handle_funcdef_block_end(get_scope());
                   scope_counter--;
                   printf("Prev Scope Space Counter: %d\n", get_prev_scope_space_counter()); 
                   set_scope_space_counter(get_prev_scope_space_counter());
                   printf("Scope Space Counter: %d\n", get_scope_space_counter());
                   pop_func_local_offset_table();
                   reset_func_local_offset();
                   emit_funcdef(FUNCEND, make_func_expression(PROGRAMFUNC_EXPR, get_symbol($2, get_scope())));
                   $$ = make_func_expression(PROGRAMFUNC_EXPR, get_symbol($2, get_scope()));
              }
         | TOK_FUNCTION TOK_L_PARENTH {
         is_in_function_mode = 1;
         name = func_name_generator();
         handle_funcdef_anonym_name(name, get_scope(), curr_scope_space(), curr_scope_offset(), yylineno);
         emit_funcdef(FUNCSTART, make_func_expression(PROGRAMFUNC_EXPR, get_symbol(name, get_scope())));
         scope_counter++;
         set_prev_scope_space_counter(get_scope_space_counter());
         set_scope_space_counter(3);
         push_formal_arg_offset_table();
/*std::cout<<scope_counter<<std::endl;*/} idlist TOK_R_PARENTH
funcdef_block {
                    //printf("funcdef -> function (idlist) funcdef_block\n");
                    //handle_funcdef_block_end(get_scope());
                    scope_counter--;
                    printf("Prev Scope Space Counter: %d\n", get_prev_scope_space_counter()); 
                    set_scope_space_counter(get_prev_scope_space_counter());
                    printf("Scope Space Counter: %d\n", get_scope_space_counter());
                    pop_func_local_offset_table();
                    reset_func_local_offset();
                    emit_funcdef(FUNCEND, make_func_expression(PROGRAMFUNC_EXPR, get_symbol(name, get_scope())));
                    //exit_scope_space();
                    //exit_scope_space();
                    //printf("The current scope space is: %d\n", curr_scope_space());
                    $$ = make_func_expression(PROGRAMFUNC_EXPR, get_symbol(name, get_scope()));
              }
;

const: TOK_INT {
            printf("const -> integer %d\n", $1); 
            $$ = make_constnum_expression(CONSTNUM_EXPR, $1);
        }
       | TOK_FLOAT {
            printf("const -> float %f\n", $1);
            $$ = make_constnum_expression(CONSTNUM_EXPR, $1);
        }
       | TOK_STRING {
            //printf("const -> string %s\n", $1);
            std::cout << "CONST -> STRING " << $1 << std::endl;
            $$ = make_conststring_expression(CONSTSTRING_EXPR, $1);
        }
       | TOK_NIL {
            //printf("const -> nil\n");
            $$ = make_nil_expression(NIL_EXPR);
        }
       | TOK_TRUE {
            //printf("const -> true\n");
            $$ = make_constbool_expression(CONSTBOOL_EXPR, $1);
        }
       | TOK_FALSE {
            //printf("const -> false\n");
            $$ = make_constbool_expression(CONSTBOOL_EXPR, $1);
        }
;

idlist: TOK_ID {
                    //printf("%s", $1);
                    handle_func_w_1arg($1, get_scope(), curr_scope_space(), curr_scope_offset(), yylineno);

               }
        | idlist TOK_COMMA TOK_ID {
                                       //printf("%s", $3);
                                       handle_func_w_1arg($3, get_scope(), curr_scope_space(), curr_scope_offset(), yylineno);
                                  }
        | %empty { push_vector();}
;

if_statement: TOK_IF TOK_L_PARENTH expression TOK_R_PARENTH statement
//{printf("if -> if (expression) statements\n");}
              | TOK_IF TOK_L_PARENTH expression TOK_R_PARENTH
statement TOK_ELSE statement //{printf("if -> if (expression)
//statements else statements\n");}
;

while_statement: TOK_WHILE TOK_L_PARENTH expression TOK_R_PARENTH
statement //{printf("while -> while (expression) statements\n");}
;

for_statement: TOK_FOR TOK_L_PARENTH elist TOK_SEMICOLON expression
TOK_SEMICOLON elist TOK_R_PARENTH statement //{printf("for -> for
//(expression) statements\n");}
;

return_statement: TOK_RETURN TOK_SEMICOLON //{printf("return -> return;\n");}
                  | TOK_RETURN expression TOK_SEMICOLON
//{printf("return -> return expression;\n");}
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
    quad_table_print();
    printf("\n");
    print_symtable();
    print_symtable_inactive();
    printf("\n");
    //print_expression_list();
    printf("\n");
    //print_reversed_expression_list();


    return 0;
}

void yyerror(const char* s) {
    fprintf(stderr, "Parse error: %s at line %d\n", s, yylineno);
    exit(1);
}
