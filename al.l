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
%}
%option outfile = "al.c"
%option header-file = "al.h"
%option noyywrap
%option yylineno

%x ml_comment
%x string 

identifier      [a-zA-Z][a-zA-Z_0-9]*
whitespace      [\r\n \t\v]
newline         \n
integer         [0-9]+
float           ([0-9])+"."([0-9])+
sl_comment      "//".*

%%
"if" {  
        handle_keyword(yylineno, yytext);
        //add_token((struct alpha_token_t*)ylval, yylineno, 0, num_of_token, "if", "KEYWORD");
        //num_of_token++;
        //print_token_list();
     }
"else" {
        handle_keyword(yylineno, yytext);
        //add_token((struct alpha_token_t*)ylval, yylineno, 0,num_of_token, "else", "KEYWORD");
        //num_of_token++;
        //print_token_list();
       }
"while" {
        handle_keyword(yylineno, yytext);
        //add_token((struct alpha_token_t*)ylval, yylineno, 0, num_of_token, "while", "KEYWORD");
        //num_of_token++;
        //print_token_list();
     }
"for" {
        handle_keyword(yylineno, yytext);
        //add_token((struct alpha_token_t*)ylval, yylineno, 0, num_of_token, "for", "KEYWORD");
        //num_of_token++;
        //print_token_list();
     }
"function" {
        handle_keyword(yylineno, yytext);
        //add_token((struct alpha_token_t*)ylval, yylineno, 0, num_of_token, "function", "KEYWORD");
        //num_of_token++;
        //print_token_list();
     }
"return" {
        handle_keyword(yylineno, yytext);
        //add_token((struct alpha_token_t*)ylval, yylineno, 0, num_of_token, "return", "KEYWORD");
        //num_of_token++;
        //print_token_list();
     }
"break" {
        handle_keyword(yylineno, yytext);
        //add_token((struct alpha_token_t*)ylval, yylineno, 0, num_of_token, "break", "KEYWORD");
        //num_of_token++;
        //print_token_list();
     }
"continue" {
        handle_keyword(yylineno, yytext);
        //add_token((struct alpha_token_t*)ylval, yylineno, 0, num_of_token, "continue", "KEYWORD");
        //num_of_token++;
        //print_token_list();
     }
"and" {
        handle_keyword(yylineno, yytext);
        //add_token((struct alpha_token_t*)ylval, yylineno, 0, num_of_token, "and", "KEYWORD");
        //num_of_token++;
        //print_token_list();
     }
"or" {
        handle_keyword(yylineno, yytext);
        //add_token((struct alpha_token_t*)ylval, yylineno, 0, num_of_token, "or", "KEYWORD");
        //num_of_token++;
        //print_token_list();
     }
"not" {
        handle_keyword(yylineno, yytext);
        //add_token((struct alpha_token_t*)ylval, yylineno, 0, num_of_token, "not", "KEYWORD");
        //num_of_token++;
        //print_token_list();
     }
"local" {
        handle_keyword(yylineno, yytext);
        //add_token((struct alpha_token_t*)ylval, yylineno, 0, num_of_token, "local", "KEYWORD");
        //num_of_token++;
        //print_token_list();
     }
"true" {
        handle_keyword(yylineno, yytext);
        //add_token((struct alpha_token_t*)ylval, yylineno, 0, num_of_token, "true", "KEYWORD");
        //num_of_token++;
        //print_token_list();
     }
"false" {
        handle_keyword(yylineno, yytext);
        //add_token((struct alpha_token_t*)ylval, yylineno, 0, num_of_token, "false", "KEYWORD");
        //num_of_token++;
        //print_token_list();
     }
"nil" {
        handle_keyword(yylineno, yytext);
        //add_token((struct alpha_token_t*)ylval, yylineno, 0, num_of_token, "nil", "KEYWORD");
        //num_of_token++;
        //print_token_list();
     }
"="  {
        handle_operator(yylineno, yytext);
        //dd_token((struct alpha_token_t*)ylval, yylineno, 0, num_of_token, "assign", "OPERATOR");
        //num_of_token++;
        //print_token_list();
     }
"+"  {
        handle_operator(yylineno, yytext);
        //add_token((struct alpha_token_t*)ylval, yylineno, 0, num_of_token, "plus", "OPERATOR");
        //num_of_token++;
        //print_token_list();
     }
"-"  {
        handle_operator(yylineno, yytext);
        //add_token((struct alpha_token_t*)ylval, yylineno, 0, num_of_token, "minus", "OPERATOR");
        //num_of_token++;
        //print_token_list();
     }
"*"  {
        handle_operator(yylineno, yytext);
        //add_token((struct alpha_token_t*)ylval, yylineno, 0, num_of_token, "multiply", "OPERATOR");
        //num_of_token++;
        //print_token_list();
     }
"/"  {
        handle_operator(yylineno, yytext);
        //add_token((struct alpha_token_t*)ylval, yylineno, 0, num_of_token, "divide", "OPERATOR");
        //num_of_token++;
        //print_token_list();
     }
'%'  {
        handle_operator(yylineno, yytext);
        //add_token((struct alpha_token_t*)ylval, yylineno, 0, num_of_token, "modulo", "OPERATOR");
        //num_of_token++;
        //print_token_list();
     }
"=="  {
        handle_operator(yylineno, yytext);
        //add_token((struct alpha_token_t*)ylval, yylineno, 0, num_of_token, "equal", "OPERATOR");
        //num_of_token++;
        //print_token_list();
     }
"!="  {
        handle_operator(yylineno, yytext);
        //add_token((struct alpha_token_t*)ylval, yylineno, 0, num_of_token, "not equal", "OPERATOR");
        //num_of_token++;
        //print_token_list();
     }
"++"  {
        handle_operator(yylineno, yytext);
        //add_token((struct alpha_token_t*)ylval, yylineno, 0, num_of_token, "double assign", "OPERATOR");
        //num_of_token++;
        //print_token_list();
     }
"--"  {
        handle_operator(yylineno, yytext);
        //add_token((struct alpha_token_t*)ylval, yylineno, 0, num_of_token, "double minus", "OPERATOR");
        //num_of_token++;
        //print_token_list();
     }
">"  {
        handle_operator(yylineno, yytext);
        //add_token((struct alpha_token_t*)ylval, yylineno, 0, num_of_token, "greater than", "OPERATOR");
        //num_of_token++;
        //print_token_list();
     }
"<"  {
        handle_operator(yylineno, yytext);
        //add_token((struct alpha_token_t*)ylval, yylineno, 0, num_of_token, "less than", "OPERATOR");
        //num_of_token++;
        //print_token_list();
     }
">="  {
        handle_operator(yylineno, yytext);
        //add_token((struct alpha_token_t*)ylval, yylineno, 0, num_of_token, "greater or equal", "OPERATOR");
        //num_of_token++;
        //print_token_list();
     }
"<="  {
        handle_operator(yylineno, yytext);
        //add_token((struct alpha_token_t*)ylval, yylineno, 0, num_of_token, "less or equal", "OPERATOR");
        //num_of_token++;
        //print_token_list();
     }
"{"  {
        handle_punctuation(yylineno, yytext);
        //add_token((struct alpha_token_t*)ylval, yylineno, 0, num_of_token, "left brace", "PUNCTUATION");
        //num_of_token++;
        //print_token_list();

     }
"}"  {
        handle_punctuation(yylineno, yytext);
        //add_token((struct alpha_token_t*)ylval, yylineno, 0, num_of_token, "right brace", "PUNCTUATION");
        //num_of_token++;
        //print_token_list();

     }
"["  {
        handle_punctuation(yylineno, yytext);
        //add_token((struct alpha_token_t*)ylval, yylineno, 0, num_of_token, "left bracket", "PUNCTUATION");
        //num_of_token++;
        //print_token_list();

     }
"]"  {
        handle_punctuation(yylineno, yytext);
        //add_token((struct alpha_token_t*)ylval, yylineno, 0, num_of_token, "right bracket", "PUNCTUATION");
        //num_of_token++;
        //print_token_list();

     }
"("  {
        handle_punctuation(yylineno, yytext);
        //add_token((struct alpha_token_t*)ylval, yylineno, 0, num_of_token, "left parenth", "PUNCTUATION");
        //num_of_token++;
        //print_token_list();

     }
")"  {
        handle_punctuation(yylineno, yytext);
        //add_token((struct alpha_token_t*)ylval, yylineno, 0, num_of_token, "right parenth", "PUNCTUATION");
        //num_of_token++;
        //print_token_list();

     }
";"  {
        handle_punctuation(yylineno, yytext);
        //add_token((struct alpha_token_t*)ylval, yylineno, 0, num_of_token, "semicolon", "PUNCTUATION");
        //num_of_token++;
        //print_token_list();
     }
","  {
        handle_punctuation(yylineno, yytext);
        //add_token((struct alpha_token_t*)ylval, yylineno, 0, num_of_token, "comma", "PUNCTUATION");
        //num_of_token++;
        //print_token_list();
     }
":"  {
        handle_punctuation(yylineno, yytext);
        //add_token((struct alpha_token_t*)ylval, yylineno, 0, num_of_token, "colon", "PUNCTUATION");
        //num_of_token++;
        //print_token_list();
     }
"::"  {
        handle_punctuation(yylineno, yytext);
        //add_token((struct alpha_token_t*)ylval, yylineno, 0, num_of_token, "double colon", "PUNCTUATION");
        //num_of_token++;
        //print_token_list();
     }
"."  {
        handle_punctuation(yylineno, yytext);
        //add_token((struct alpha_token_t*)ylval, yylineno, 0, num_of_token, "dot", "PUNCTUATION");
        //num_of_token++;
        //print_token_list();
     }
".."  {
        handle_punctuation(yylineno, yytext);
        //add_token((struct alpha_token_t*)ylval, yylineno, 0, num_of_token, "double dot", "PUNCTUATION");
        //num_of_token++;
        //print_token_list();
      }
{identifier}  {
                 handle_identifier(yylineno, yytext);
                //add_token((struct alpha_token_t*)ylval, yylineno, 0, num_of_token, yytext, "IDENTIFIER");
                //num_of_token++;
                //print_token_list();
              }
{integer}  {
             handle_integer(yylineno, yytext);
             //add_token((struct alpha_token_t*)ylval, yylineno, 0, num_of_token, yytext, "INTEGER CONSTANT");
             //num_of_token++;
             //print_token_list();
           }
{float}  {
             handle_float(yylineno, yytext);
             //add_token((struct alpha_token_t*)ylval, yylineno, 0, num_of_token, yytext, "FLOAT CONSTANT");
             //num_of_token++;
             //print_token_list();
           }
{newline} {
             num_of_line = yylineno;
             //print_token_list();
          }
{whitespace} {
               //print_token_list();
             }
{sl_comment} {
               handle_sl_comment(yylineno, yytext);
               //add_token((struct alpha_token_t*)ylval, yylineno, 0, num_of_token, " ", "SINGLE LINE COMMENT");
               //print_token_list();
             }

[_~@`#$^?!&&||]            { 
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
        if (!comment_stack.empty()) {
                if (state == 1) {
                    std::cout << "Error Non Closing Block Comment" << std::endl;
                    yyterminate();
                } else if (state > 1) {
                    std::cout << "Error Non Closing Nested Comment" << std::endl;
                    yyterminate();
                }
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
        //yylval.strVal = str_p;
        BEGIN(INITIAL);
        //return TOK_STRING;
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
