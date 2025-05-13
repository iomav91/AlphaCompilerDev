
#ifndef _HELP_FUNCTIONS_H
#define _HELP_FUNCTIONS_H

#include <iostream>
#include <string>

struct alpha_token_t;

void handle_keyword(int token_line, std::string token_content);
void handle_punctuation(int token_line, std::string token_content);
void handle_operator(int token_line, std::string token_content);
void handle_identifier(int token_line, std::string token_content);
void handle_integer(int token_line, std::string token_content);
void handle_float(int token_line, std::string token_content);
void handle_sl_comment(int token_line, std::string token_content);
void handle_ml_comment(int start_line, int end_line, int state);
void handle_string(int token_line, std::string token_content);

#endif
