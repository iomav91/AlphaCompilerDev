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
    int is_in_function_mode = 0;
    int is_in_block_mode = 0;
%}
%output "parser.c"
%defines "parser.h"
%define parse.error detailed
%define parse.lac full

%start program

%code requires {
    struct SymbolTableEntry {
        bool isActive;
        char* name;
        int scope;
        int line;
        int type;
    };
}

%union {
    int intVal;
    double floatVal;
    char* strVal;
    SymbolTableEntry* symbol;
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
%token TOK_TRUE
%token TOK_FALSE
%token TOK_NIL
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

%type <symbol> lvalue
%type <strVal> idlist
%type <strVal> expression

%right TOK_ASSIGN
%left TOK_OR
%left TOK_AND
%nonassoc TOK_EQUAL TOK_N_EQUAL
%nonassoc TOK_GREATER TOK_LESS TOK_GR_EQUAL TOK_LS_EQUAL
%left TOK_PLUS TOK_MINUS
%left TOK_MULTIPLY TOK_DIVIDE TOK_MODULO
%right TOK_NOT TOK_DBL_PLUS TOK_DBL_MINUS
%nonassoc UMINUS
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
                    //handle_expression($1,$3, get_scope(), yylineno);
                }
            | expression TOK_MINUS expression {
                    //printf("expression -> expression - expression\n");
                    //handle_expression($1,$3, get_scope(), yylineno);
                }
            | expression TOK_MULTIPLY expression {
                    //printf("expression -> expression * expression\n");
                    //handle_expression($1,$3, get_scope(), yylineno);
                }
            | expression TOK_DIVIDE expression {
                    //printf("expression -> expression / expression\n");
                    //handle_expression($1,$3, get_scope(), yylineno);
                }
            | expression TOK_MODULO expression {
                    //printf("expression -> expression modulo expression\n");
                    //handle_expression($1,$3, get_scope(), yylineno);
                }
            | expression TOK_GREATER expression {
                    //printf("expression -> expression > expression\n");
                    //handle_expression($1,$3, get_scope(), yylineno);
                }
            | expression TOK_LESS expression {
                    //printf("expression -> expression < expression\n");
                    //handle_expression($1,$3, get_scope(), yylineno);
                }
            | expression TOK_GR_EQUAL expression { 
                    //printf("expression -> expression >= expression\n");
                    //handle_expression($1,$3, get_scope(), yylineno);
                }
            | expression TOK_LS_EQUAL expression {
                    //printf("expression -> expression <= expression\n");
                    //handle_expression($1,$3, get_scope(), yylineno);
                }
            | expression TOK_EQUAL expression {
                    //printf("expression -> expression == expression\n");
                    //handle_expression($1,$3, get_scope(), yylineno);
                }
            | expression TOK_N_EQUAL expression {
                    //printf("expression -> expression != expression\n");
                    //handle_expression($1,$3, get_scope(), yylineno);
                }
            | expression TOK_AND expression {
                    //printf("expression -> expression and expression\n");
                    //handle_expression($1,$3, get_scope(), yylineno);
                }
            | expression TOK_OR expression {
                    //printf("expression -> expression or expression\n");
                    //handle_expression($1,$3, get_scope(), yylineno);
                }
            | term //{printf("expression -> term\n");}
;

term: TOK_L_PARENTH expression TOK_R_PARENTH //{printf("term ->
//(expression)\n");}
      | UMINUS expression //{printf("term -> - expression\n");}
      | TOK_NOT expression  //{printf("term -> not expression\n");}
      | TOK_DBL_PLUS lvalue {/*printf("term -> ++lvalue\n");*/ handle_assign_expr($2->name,get_scope());}
      | lvalue TOK_DBL_PLUS {/*printf("term -> lvalue++\n");*/ handle_assign_expr($1->name,get_scope());}
      | TOK_DBL_MINUS lvalue {/*printf("term -> --lvalue\n");*/ handle_assign_expr($2->name,get_scope());}
      | lvalue TOK_DBL_MINUS {/*printf("term -> lvalue--\n");*/ handle_assign_expr($1->name,get_scope());}
      | primary //{printf("term -> primary\n");}
;

assignexpr: lvalue TOK_ASSIGN expression {
        //printf("assignexpr -> lvalue = expression\n");

        handle_assign_expr($1->name, get_scope());

                                         }
;

primary: lvalue //{printf("primary -> lvalue\n");}
         | call //{printf("primary -> call\n");}
         | objectdef //{printf("primary -> objectdef\n");}
         | TOK_L_PARENTH funcdef TOK_R_PARENTH //{printf("primary ->
//(funcdef)\n");}
         | const //{printf("primary -> const\n");}
;

lvalue: TOK_ID {//printf("lvalue -> ID\n"); 
                    handle_id($1, get_scope(), yylineno);
                    $$ = (SymbolTableEntry*)malloc(sizeof(SymbolTableEntry));
                    $$->name = $1;
                    $$->scope = get_scope();
                    $$->line = yylineno;
                    $$->isActive = true;
                    if (get_scope == 0) {
                        $$->type = 0;
                    } else if (get_scope() > 0) {
                        $$->type = 1;
                    }
                    //printf("name of lvalue: %s\n", $$->name);
                    //printf("scope of lvalue: %d\n", $$->scope);
               }
        | TOK_LOCAL TOK_ID {//printf("lvalue -> local id\n");
                                handle_local_id($2, get_scope(), yylineno);
                                $$ = (SymbolTableEntry*)malloc(sizeof(SymbolTableEntry));
                                $$->name = $2;
                                $$->scope = get_scope();
                                $$->line = yylineno;
                                $$->isActive = true;
                                $$->type = 1;
                                //printf("name of lvalue: %s\n", $$->name);
                                //printf("scope of lvalue: %d\n", $$->scope);
                           }
        | TOK_DBL_COLON TOK_ID  {//printf("lvalue -> :: id\n");
                                    handle_global_access_id($2);
                                }
        | member //{printf("lvalue -> member\n");}
;

member: lvalue TOK_DOT TOK_ID //{printf("member -> lvalue.id\n");}
        | lvalue TOK_L_BR expression TOK_R_BR //{printf("member ->
//lvalue[expression]\n");}
        | call TOK_DOT TOK_ID //{printf("member -> call.id\n");}
        | call TOK_L_BR expression TOK_R_BR //{printf("member ->
//call[expression]\n");}
;

call: call normcall //{printf("call -> call normcall\n");}
      | lvalue callsuffix //{printf("call -> lvalue callsuffix\n");}
      | TOK_L_PARENTH funcdef TOK_R_PARENTH normcall //{printf("call
//-> (funcdef) normcall\n");}
;

callsuffix: normcall //{printf("callsuffix -> normcall\n");}
            | methodcall //{printf("callsuffix -> methodcall\n");}
;

normcall: TOK_L_PARENTH elist TOK_R_PARENTH //{printf("normcall -> (elist)\n");}
;

methodcall: TOK_DBL_DOT TOK_ID normcall //{printf("methodcall -> ..id
//normcall\n");}
;

elist: expression //{printf("elist -> expression\n");}
       | elist TOK_COMMA expression //{printf("elist -> elist,\n");}
       |  %empty //{printf("elist -> \n");}
;

indexed: indexedelem //{printf("elist -> expression\n");}
       | indexed TOK_COMMA indexedelem //{printf("elist -> elist,\n");}
;

objectdef: TOK_L_BR elist TOK_R_BR //{printf("objectdef -> [elist]\n");}
           | TOK_L_BR indexed TOK_R_BR //{printf("objectdef -> [indexed]\n");}
;

indexedelem: TOK_L_CURLY_BR expression TOK_COLON expression
TOK_R_CURLY_BR //{printf("indexedelem -> {expression:expression}\n");}
;

block: TOK_L_CURLY_BR {
    push_blocks_prec(1);
    push_vector();
    is_in_block_mode++;
    scope_counter++;
std::cout<<scope_counter<<std::endl;} statements TOK_R_CURLY_BR
{/*printf("block -> {statements}\n");*/ is_in_block_mode--; handle_block_end(get_scope()); scope_counter--;
/*std::cout<< "Scope Counter is " << scope_counter<<std::endl;*/}
;

funcdef_block: TOK_L_CURLY_BR {push_blocks_prec(2);} statements TOK_R_CURLY_BR {
        /*printf("funcdef_block -> {statements}\n");*/
        handle_funcdef_block_end(get_scope());
        //scope_counter--; 
        is_in_function_mode = 0;
        //std::cout<<scope_counter<<std::endl;
    }
;

funcdef: TOK_FUNCTION TOK_ID TOK_L_PARENTH {
    
    is_in_function_mode = 1;
    handle_funcdef_w_name($2, get_scope(), yylineno);
    scope_counter++;
    /*std::cout<<scope_counter<<std::endl;*/} idlist TOK_R_PARENTH
funcdef_block {
                   //printf("funcdef -> function id (idlist) funcdef_block\n");
                   //handle_funcdef_block_end(get_scope());
                   scope_counter--;
              }
         | TOK_FUNCTION TOK_L_PARENTH {
         is_in_function_mode = 1;
         handle_funcdef_anonym_name(get_scope(), yylineno);
         scope_counter++;
/*std::cout<<scope_counter<<std::endl;*/} idlist TOK_R_PARENTH
funcdef_block {
                    //printf("funcdef -> function (idlist) funcdef_block\n");
                    //handle_funcdef_block_end(get_scope());
                    scope_counter--;
              }
;

const: TOK_INT //{printf("const -> integer %d\n", $1);}
       | TOK_FLOAT //{printf("const -> float %f\n", $1);}
       | TOK_STRING //{printf("const -> string %s\n", $1);}
       | TOK_NIL //{printf("const -> nil\n");}
       | TOK_TRUE //{printf("const -> true\n");}
       | TOK_FALSE //{printf("const -> false\n");}
;

idlist: TOK_ID {
                    //printf("%s", $1);
                    handle_func_w_1arg($1, get_scope(), yylineno);

               }
        | idlist TOK_COMMA TOK_ID {
                                       //printf("%s", $3);
                                       handle_func_w_1arg($3, get_scope(), yylineno);
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
    print_symtable();
    print_symtable_inactive();
    return 0;
}

void yyerror(const char* s) {
    fprintf(stderr, "Parse error: %s at line %d\n", s, yylineno);
    exit(1);
}
