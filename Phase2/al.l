%{
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

TOK_IF           "if"
TOK_ELSE         "else"
TOK_WHILE        "while"
TOK_FOR          "for"
TOK_FUNCTION     "function"
TOK_RETURN       "return"
TOK_BREAK        "break"
TOK_CONTINUE     "continue"
TOK_AND          "and"
TOK_NOT          "not"
TOK_OR           "or"
TOK_LOCAL        "local"
TOK_TRUE         "true"
TOK_FALSE        "false"
TOK_NIL          "nil"
TOK_ASSIGN       "="
TOK_PLUS         "+"
TOK_MINUS        "-"
TOK_MULTIPLY     "*"
TOK_DIVIDE       "/"
TOK_MODULO       "%"
TOK_EQUAL        "=="
TOK_N_EQUAL      "!="
TOK_DBL_PLUS     "++"
TOK_DBL_MINUS    "--"
TOK_GREATER      ">"
TOK_LESS         "<"
TOK_GR_EQUAL     ">="
TOK_LS_EQUAL     "<="
TOK_L_CURLY_BR   "{"
TOK_R_CURLY_BR   "}"
TOK_L_BR         "["
TOK_R_BR         "]"
TOK_L_PARENTH    "("
TOK_R_PARENTH    ")"
TOK_SEMICOLON    ";"
TOK_COMMA        ","
TOK_COLON        ":"
TOK_DBL_COLON    "::"
TOK_DOT          "."
TOK_DBL_DOT      ".."
TOK_ID           [a-zA-Z][a-zA-Z_0-9]*
TOK_INT          [0-9]+
TOK_FLOAT        ([0-9])+"."([0-9])+
TOK_STRING       \" \"

%x ml_comment
%x string

whitespace      [\r\n \t\v]
newline         \n
sl_comment      "//".*


%%

{TOK_IF}         {handle_keyword(yylineno, yytext); return TOK_IF;}
{TOK_ELSE}       {handle_keyword(yylineno, yytext); return TOK_ELSE;}
{TOK_WHILE}      {handle_keyword(yylineno, yytext); return TOK_WHILE;}
{TOK_FOR}        {handle_keyword(yylineno, yytext); return TOK_FOR;}
{TOK_FUNCTION}   {handle_keyword(yylineno, yytext); return TOK_FUNCTION;}
{TOK_RETURN}     {handle_keyword(yylineno, yytext); return TOK_RETURN;}
{TOK_BREAK}      {handle_keyword(yylineno, yytext); return TOK_BREAK;}
{TOK_CONTINUE}   {handle_keyword(yylineno, yytext); return TOK_CONTINUE;}
{TOK_AND}        {handle_keyword(yylineno, yytext); return TOK_AND;}
{TOK_NOT}        {handle_keyword(yylineno, yytext); return TOK_NOT;}
{TOK_OR}         {handle_keyword(yylineno, yytext); return TOK_OR;}
{TOK_LOCAL}      {handle_keyword(yylineno, yytext); return TOK_LOCAL;}
{TOK_TRUE}       {handle_keyword(yylineno, yytext); return TOK_TRUE;}
{TOK_FALSE}      {handle_keyword(yylineno, yytext); return TOK_FALSE;}
{TOK_NIL}        {handle_keyword(yylineno, yytext); return TOK_NIL;}
{TOK_ASSIGN}     {handle_operator(yylineno, yytext); return TOK_ASSIGN;}
{TOK_PLUS}       {handle_operator(yylineno, yytext); return TOK_PLUS;}
{TOK_MINUS}      {handle_operator(yylineno, yytext); return TOK_MINUS;}
{TOK_MULTIPLY}   {handle_operator(yylineno, yytext); return TOK_MULTIPLY;}
{TOK_DIVIDE}     {handle_operator(yylineno, yytext); return TOK_DIVIDE;}
{TOK_MODULO}     {handle_operator(yylineno, yytext); return TOK_MODULO;}
{TOK_EQUAL}      {handle_operator(yylineno, yytext); return TOK_EQUAL;}
{TOK_N_EQUAL}    {handle_operator(yylineno, yytext); return TOK_N_EQUAL;}
{TOK_DBL_PLUS}   {handle_operator(yylineno, yytext); return TOK_DBL_PLUS;}
{TOK_DBL_MINUS}  {handle_operator(yylineno, yytext); return TOK_DBL_MINUS;}
{TOK_GREATER}    {handle_operator(yylineno, yytext); return TOK_GREATER;}
{TOK_LESS}       {handle_operator(yylineno, yytext); return TOK_LESS;}
{TOK_GR_EQUAL}   {handle_operator(yylineno, yytext); return TOK_GR_EQUAL;}
{TOK_LS_EQUAL}   {handle_operator(yylineno, yytext); return TOK_LS_EQUAL;}
{TOK_L_CURLY_BR} {handle_punctuation(yylineno, yytext); return TOK_L_CURLY_BR;}
{TOK_R_CURLY_BR} {handle_punctuation(yylineno, yytext); return TOK_R_CURLY_BR;}
{TOK_L_BR}       {handle_punctuation(yylineno, yytext); return TOK_L_BR;}
{TOK_R_BR}       {handle_punctuation(yylineno, yytext); return TOK_R_BR;}
{TOK_L_PARENTH}  {handle_punctuation(yylineno, yytext); return TOK_L_PARENTH;}
{TOK_R_PARENTH}  {handle_punctuation(yylineno, yytext); return TOK_R_PARENTH;}
{TOK_SEMICOLON}  {handle_punctuation(yylineno, yytext); return TOK_SEMICOLON;}
{TOK_COMMA}      {handle_punctuation(yylineno, yytext); return TOK_COMMA;}
{TOK_COLON}      {handle_punctuation(yylineno, yytext); return TOK_COLON;}
{TOK_DBL_COLON}  {handle_punctuation(yylineno, yytext); return TOK_DBL_COLON;}
{TOK_DOT}        {handle_punctuation(yylineno, yytext); return TOK_DOT;}
{TOK_DBL_DOT}    {handle_punctuation(yylineno, yytext); return TOK_DBL_DOT;}
{TOK_ID}         {handle_identifier(yylineno, yytext); yylval.strVal =
strdup(yytext); return TOK_ID;}
{TOK_INT}        {handle_integer(yylineno, yytext); yylval.intVal =
atoi(yytext); return TOK_INT;}
{TOK_FLOAT}      {handle_float(yylineno, yytext); yylval.floatVal =
atof(yytext); return TOK_FLOAT;}

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
        for (int i = 0; i < buffer.size(); i++) {
                str_p[i] = buffer.c_str()[i];
        }
        yylval.strVal = str_p;
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