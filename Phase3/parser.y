%{
    #include "symbol_table.h"
    #include "handlers.h"
    #include "icode_managers.h"
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
}

%union {
    unsigned unsignedVal;
    int intVal;
    double floatVal;
    char* strVal;
    SymbolTableEntry* symbol;
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
%type <strVal> funcname
%type <symbol> funcprefix
%type <unsignedVal> funcbody
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

program: statements statement                                                                          {}
         | %empty                                                                                      {}
;

statement: expression TOK_SEMICOLON                                                                    {}
          | if_statement                                                                               {}
          | while_statement                                                                            {}
          | for_statement                                                                              {}
          | return_statement                                                                           {handle_return_st();}
          | TOK_CONTINUE TOK_SEMICOLON                                                                 {handle_continue_st();}
          | TOK_BREAK TOK_SEMICOLON                                                                    {handle_break_st();}
          | block                                                                                      {}
          | funcdef                                                                                    {}
          | TOK_SEMICOLON                                                                              {}
          | error TOK_SEMICOLON {yyerrok; yyclearin;}
;

statements: statements statement                                                                       {}
            | %empty                                                                                   {}
;

expression: assignexpr                                                                                 {}
            | expression TOK_PLUS expression                                                           {

                                                                                                            $$ = manage_expr_plus_expr($$, $1, $3);
                    
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
                                                                                                            $$ = manage_expr_gr_expr($$, $1, $3);
                                                                                                       }
            | expression TOK_N_EQUAL expression                                                        {
                   
                                                                                                            $$ = manage_expr_not_eq_expr($$, $1, $3);
                                                                                                       
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
      | TOK_MINUS expression %prec TOK_UMINUS                                                          {
            
                                                                                                            $$ = manage_minus_expr($$, $2);

                                                                                                       }
      | TOK_NOT expression                                                                             {
                                                                                                            $$ = manage_not_expr($$, $2);
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
            printf("term -> primary\n");
            $$ = $1;
            //std::cout << "$$->type " << $$->type << "\t\t" << $$->symbol->name << std::endl;
      }
;

assignexpr: lvalue TOK_ASSIGN expression                                                               {
                                                                                                            $$ = manage_assign_expr($$, $1, $3);
                                                                                                       }
;

primary: lvalue                                                                                        {
                                                                                                            $$ = emit_if_table_item($1);
                                                                                                       }
         | call                                                                                        {}
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
                                                                                                       }
        | TOK_LOCAL TOK_ID                                                                             {
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
            $$ = make_call($1);
       }
      | lvalue callsuffix                                                                              {
                                                                                                            $$ = manage_lvalue_callsuffix($$, $1, $2.name, $2.method);
                                                                                                       }
      | TOK_L_PARENTH funcdef TOK_R_PARENTH normcall {
            //expression* func = make_func_expression(PROGRAMFUNC_EXPR, get_symbol($2->symbol->name, get_scope()));
            $$ = make_call($2);     
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

funcdef_block: TOK_L_CURLY_BR statements TOK_R_CURLY_BR {
        handle_funcdef_block_end(get_scope());
        reset_formal_arg_offset();
    }
;

funcprefix: TOK_FUNCTION TOK_ID {
        is_in_function_mode++;
        $$ = manage_funcprefix($$, $2, yylineno);
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
        
        printf("Prev Scope Space Counter: %d\n", get_prev_scope_space_counter()); 
        set_scope_space_counter(get_prev_scope_space_counter());
        printf("Scope Space Counter: %d\n", get_scope_space_counter());
        pop_func_local_offset_table();
        reset_func_local_offset();
    }
;

funcdef: funcprefix funcargs funcbody {

        $1->total_locals = $3;
        $$ = make_func_expression(PROGRAMFUNC_EXPR, get_symbol($1->name, get_scope()));
        emit_funcdef(FUNCEND,$$);

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
            $$ = make_constbool_expression(CONSTBOOL_EXPR, $1);
        }
       | TOK_FALSE {
            $$ = make_constbool_expression(CONSTBOOL_EXPR, $1);
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

if_statement: TOK_IF TOK_L_PARENTH expression TOK_R_PARENTH statement
                                                                           {}
              | TOK_IF TOK_L_PARENTH expression TOK_R_PARENTH
statement TOK_ELSE statement                                               {}
;

while_statement: TOK_WHILE TOK_L_PARENTH expression TOK_R_PARENTH
statement                                                                  {}
;

for_statement: TOK_FOR TOK_L_PARENTH elist TOK_SEMICOLON expression
TOK_SEMICOLON elist TOK_R_PARENTH statement                                {}
;

return_statement: TOK_RETURN TOK_SEMICOLON                                 {}
                  | TOK_RETURN expression TOK_SEMICOLON
                                                                           {}
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
