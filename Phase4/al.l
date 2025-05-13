%{
   #include "symbol_table.h"  
   #include "help_functions.h"
   #include <stack>
   #include <string>
   #include <stdlib.h>
   std::stack<int> comment_stack;
   std::string buffer, str;
   int num_of_line = 0;
   int start_comment_line = 0;
   int state = 0;
   int i = 0;
   extern int num_of_token;
   #define YY_DECL int yylex()
   #include "parser.h"

%}
%option outfile = "al.c"
%option header-file="al.h"
%option noyywrap
%option yylineno

AL_TOK_IF           "if"
AL_TOK_ELSE         "else"
AL_TOK_WHILE        "while"
AL_TOK_FOR          "for"
AL_TOK_FUNCTION     "function"
AL_TOK_RETURN       "return"
AL_TOK_BREAK        "break"
AL_TOK_CONTINUE     "continue"
AL_TOK_AND          "and"
AL_TOK_NOT          "not"
AL_TOK_OR           "or"
AL_TOK_LOCAL        "local"
AL_TOK_TRUE         "true"
AL_TOK_FALSE        "false"
AL_TOK_NIL          "nil"
AL_TOK_ASSIGN       "="
AL_TOK_PLUS         "+"
AL_TOK_MINUS        "-"
AL_TOK_MULTIPLY     "*"
AL_TOK_DIVIDE       "/"
AL_TOK_MODULO       "%"
AL_TOK_EQUAL        "=="
AL_TOK_N_EQUAL      "!="
AL_TOK_DBL_PLUS     "++"
AL_TOK_DBL_MINUS    "--"
AL_TOK_GREATER      ">"
AL_TOK_LESS         "<"
AL_TOK_GR_EQUAL     ">="
AL_TOK_LS_EQUAL     "<="
AL_TOK_L_CURLY_BR   "{"
AL_TOK_R_CURLY_BR   "}"
AL_TOK_L_BR         "["
AL_TOK_R_BR         "]"
AL_TOK_L_PARENTH    "("
AL_TOK_R_PARENTH    ")"
AL_TOK_SEMICOLON    ";"
AL_TOK_COMMA        ","
AL_TOK_COLON        ":"
AL_TOK_DBL_COLON    "::"
AL_TOK_DOT          "."
AL_TOK_DBL_DOT      ".."
AL_TOK_ID           [a-zA-Z][a-zA-Z_0-9]*
AL_TOK_INT          [0-9]+
AL_TOK_FLOAT        ([0-9])+"."([0-9])+
AL_TOK_STRING       \" \"

%x ml_comment
%x string

whitespace      [\r\n \t\v]
newline         \n
sl_comment      "//".*


%%

{AL_TOK_IF}         {handle_keyword(yylineno, yytext); return TOK_IF;}
{AL_TOK_ELSE}       {handle_keyword(yylineno, yytext); return TOK_ELSE;}
{AL_TOK_WHILE}      {handle_keyword(yylineno, yytext); return TOK_WHILE;}
{AL_TOK_FOR}        {handle_keyword(yylineno, yytext); return TOK_FOR;}
{AL_TOK_FUNCTION}   {handle_keyword(yylineno, yytext); return TOK_FUNCTION;}
{AL_TOK_RETURN}     {handle_keyword(yylineno, yytext); return TOK_RETURN;}
{AL_TOK_BREAK}      {handle_keyword(yylineno, yytext); return TOK_BREAK;}
{AL_TOK_CONTINUE}   {handle_keyword(yylineno, yytext); return TOK_CONTINUE;}
{AL_TOK_AND}        {handle_keyword(yylineno, yytext); return TOK_AND;}
{AL_TOK_NOT}        {handle_keyword(yylineno, yytext); return TOK_NOT;}
{AL_TOK_OR}         {handle_keyword(yylineno, yytext); return TOK_OR;}
{AL_TOK_LOCAL}      {handle_keyword(yylineno, yytext); return TOK_LOCAL;}
{AL_TOK_TRUE}       {handle_keyword(yylineno, yytext); yylval.intVal = atoi(yytext); return TOK_TRUE;}
{AL_TOK_FALSE}      {handle_keyword(yylineno, yytext); yylval.intVal = atoi(yytext); return TOK_FALSE;}
{AL_TOK_NIL}        {handle_keyword(yylineno, yytext); return TOK_NIL;}
{AL_TOK_ASSIGN}     {handle_operator(yylineno, yytext); return TOK_ASSIGN;}
{AL_TOK_PLUS}       {handle_operator(yylineno, yytext); return TOK_PLUS;}
{AL_TOK_MINUS}      {handle_operator(yylineno, yytext); return TOK_MINUS;}
{AL_TOK_MULTIPLY}   {handle_operator(yylineno, yytext); return TOK_MULTIPLY;}
{AL_TOK_DIVIDE}     {handle_operator(yylineno, yytext); return TOK_DIVIDE;}
{AL_TOK_MODULO}     {handle_operator(yylineno, yytext); return TOK_MODULO;}
{AL_TOK_EQUAL}      {handle_operator(yylineno, yytext); return TOK_EQUAL;}
{AL_TOK_N_EQUAL}    {handle_operator(yylineno, yytext); return TOK_N_EQUAL;}
{AL_TOK_DBL_PLUS}   {handle_operator(yylineno, yytext); return TOK_DBL_PLUS;}
{AL_TOK_DBL_MINUS}  {handle_operator(yylineno, yytext); return TOK_DBL_MINUS;}
{AL_TOK_GREATER}    {handle_operator(yylineno, yytext); return TOK_GREATER;}
{AL_TOK_LESS}       {handle_operator(yylineno, yytext); return TOK_LESS;}
{AL_TOK_GR_EQUAL}   {handle_operator(yylineno, yytext); return TOK_GR_EQUAL;}
{AL_TOK_LS_EQUAL}   {handle_operator(yylineno, yytext); return TOK_LS_EQUAL;}
{AL_TOK_L_CURLY_BR} {handle_punctuation(yylineno, yytext); return TOK_L_CURLY_BR;}
{AL_TOK_R_CURLY_BR} {handle_punctuation(yylineno, yytext); return TOK_R_CURLY_BR;}
{AL_TOK_L_BR}       {handle_punctuation(yylineno, yytext); return TOK_L_BR;}
{AL_TOK_R_BR}       {handle_punctuation(yylineno, yytext); return TOK_R_BR;}
{AL_TOK_L_PARENTH}  {handle_punctuation(yylineno, yytext); return TOK_L_PARENTH;}
{AL_TOK_R_PARENTH}  {handle_punctuation(yylineno, yytext); return TOK_R_PARENTH;}
{AL_TOK_SEMICOLON}  {handle_punctuation(yylineno, yytext); return TOK_SEMICOLON;}
{AL_TOK_COMMA}      {handle_punctuation(yylineno, yytext); return TOK_COMMA;}
{AL_TOK_COLON}      {handle_punctuation(yylineno, yytext); return TOK_COLON;}
{AL_TOK_DBL_COLON}  {handle_punctuation(yylineno, yytext); return TOK_DBL_COLON;}
{AL_TOK_DOT}        {handle_punctuation(yylineno, yytext); return TOK_DOT;}
{AL_TOK_DBL_DOT}    {handle_punctuation(yylineno, yytext); return TOK_DBL_DOT;}
{AL_TOK_ID}         {handle_identifier(yylineno, yytext); yylval.strVal = strdup(yytext); return TOK_ID;}
{AL_TOK_INT}        {handle_integer(yylineno, yytext); yylval.intVal = atoi(yytext); return TOK_INT;}
{AL_TOK_FLOAT}      {handle_float(yylineno, yytext); yylval.floatVal = atof(yytext); return TOK_FLOAT;}

{newline} {
             num_of_line = yylineno;
             //print_token_list();
          }
{whitespace} {
               //print_token_list();
             }
{sl_comment} {
               handle_sl_comment(yylineno, yytext);
               //add_token((struct alpha_token_t*)ylval, yylineno, 0,
//num_of_token, " ", "SINGLE LINE COMMENT");
               //print_token_list();
             }
^[_~@`#$^?]            {
        printf("Undefined Characters or Wrong Identifier\n");
        return -1;
}

"/*"     {
        start_comment_line = yylineno;
        state = 1;
        comment_stack.push(start_comment_line);
        BEGIN(ml_comment);
}
<ml_comment>{
        "*/" {
                state--;
                comment_stack.pop();
                handle_ml_comment(start_comment_line, yylineno, state);
                if (state == 0) {
                        BEGIN(INITIAL);
                }
        }

        "/*" {
                state++;
                start_comment_line = yylineno;
                comment_stack.push(start_comment_line);
        }
        [^.] {}

}
<ml_comment><<EOF>> {
        if (comment_stack.empty()) {
                std::cout << "Error Non Closing Block Comment" << std::endl;
                yyterminate();
        } else {
               std::cout << "Error Non Closing Nested Comment" << std::endl;
               yyterminate();
        }
}

\" {
        buffer.clear();
        BEGIN(string);
}
<string>\\n {
        buffer.push_back('\n');
}
<string>\\t {
        buffer.push_back('\t');
}
<string>\\\\ {
        buffer.push_back('\\');
}
<string>\\\" {
        buffer.push_back('\"');
}
<string>\" {
        handle_string(yylineno, buffer);
        //buffer.resize(0);
        char *str_p = (char*)malloc(buffer.size()+1);
        int i;
        for (i = 0; i < buffer.size(); i++) {
                str_p[i] = buffer.c_str()[i];
        }
        str_p[i] = '\0';
        yylval.strVal = strdup(str_p);
        std::cout << yylval.strVal << std::endl;
        BEGIN(INITIAL);
        return TOK_STRING;
}
<string>\\[.]* {
        std::cout << "Warning: Undefined Character" << std::endl;
        buffer.push_back(*yytext);
}
<string>[^.] {
        buffer.push_back(*yytext);
}
<string><<EOF>>  {
        std::cout << "Error non closing string" << std::endl;
        yyterminate();
}

%%