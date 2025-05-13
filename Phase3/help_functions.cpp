
#include "help_functions.h"
#include "al.h"
#include <list>

int num_of_token = 0;
FILE *out_file = fopen("outfile.txt", "w");

struct alpha_token_t {
    int token_line_start;
    int token_line_end;
    int token_number;
    std::string token_content;
    std::string token_cat;
};

std::list<alpha_token_t> token_list;

void handle_keyword(int token_line, std::string token_content) {
    //std::cout << token_line << " " << num_of_token << " " <<
//token_content << " "
      //        << "KEYWORD" << std::endl;
    //fprintf(out_file, "%d\t%d\t%s\t%s\n", token_line, num_of_token,
//token_content, "KEYWORD");
    alpha_token_t new_token;
    new_token.token_line_start = token_line;
    new_token.token_line_end = 0;
    new_token.token_number = num_of_token;
    new_token.token_content = token_content;
    new_token.token_cat = "KEYWORD";

    token_list.push_back(new_token);
    num_of_token++;
    return;
}
void handle_punctuation(int token_line, std::string token_content) {
    //std::cout << token_line << " " << num_of_token << " " <<
//token_content << " "
      //   << "PUNCTUATION" << std::endl;
    alpha_token_t new_token;
    new_token.token_line_start = token_line;
    new_token.token_line_end = 0;
    new_token.token_number = num_of_token;
    new_token.token_content = token_content;
    new_token.token_cat = "PUNCTUATION";

    token_list.push_back(new_token);
    num_of_token++;
    return;
}
void handle_operator(int token_line, std::string token_content) {
    // std::cout << token_line << " " << num_of_token << " " <<
//token_content << " "
       //  << "OPERATOR" << std::endl;
    alpha_token_t new_token;
    new_token.token_line_start = token_line;
    new_token.token_line_end = 0;
    new_token.token_number = num_of_token;
    new_token.token_content = token_content;
    new_token.token_cat = "OPERATOR";

    token_list.push_back(new_token);
    num_of_token++;
    return;
}
void handle_identifier(int token_line, std::string token_content) {
    // std::cout << token_line << " " << num_of_token << " " <<
//token_content << " "
       //  << "IDENTIFIER" << std::endl;
    alpha_token_t new_token;
    new_token.token_line_start = token_line;
    new_token.token_line_end = 0;
    new_token.token_number = num_of_token;
    new_token.token_content = token_content;
    new_token.token_cat = "IDENTIFIER";

    token_list.push_back(new_token);
    num_of_token++;
    return;
}
void handle_integer(int token_line, std::string token_content) {
    // std::cout << token_line << " " << num_of_token << " " <<
//token_content << " "
       //  << "INTEGER CONST" << std::endl;
    alpha_token_t new_token;
    new_token.token_line_start = token_line;
    new_token.token_line_end = 0;
    new_token.token_number = num_of_token;
    new_token.token_content = token_content;
    new_token.token_cat = "INTEGER CONST";

    token_list.push_back(new_token);
    num_of_token++;
    return;
}
void handle_float(int token_line, std::string token_content) {
    // std::cout << token_line << " " << num_of_token << " " <<
//token_content << " "
       //  << "FLOAT CONST" << std::endl;
    alpha_token_t new_token;
    new_token.token_line_start = token_line;
    new_token.token_line_end = 0;
    new_token.token_number = num_of_token;
    new_token.token_content = token_content;
    new_token.token_cat = "FLOAT CONST";

    token_list.push_back(new_token);
    num_of_token++;
    return;
}
void handle_sl_comment(int token_line, std::string token_content) {
    // std::cout << token_line << " " << num_of_token << " " <<
//token_content << " "
       //  << "SINGLE LINE COMMENT" << std::endl;
    alpha_token_t new_token;
    new_token.token_line_start = token_line;
    new_token.token_line_end = token_line;
    new_token.token_number = num_of_token;
    new_token.token_content = token_content;
    new_token.token_cat = "SINGLE LINE COMMENT";

    token_list.push_back(new_token);
    num_of_token++;
    return;
}
void handle_ml_comment(int start_line, int end_line, int state) {
    if (state == 0) {
        //std::cout << start_line << " " << end_line << " " <<
//num_of_token << " " << " "
         //<< "BLOCK COMMENT" << std::endl;
        alpha_token_t new_token;
        new_token.token_line_start = start_line;
        new_token.token_line_end = end_line;
        new_token.token_number = num_of_token;
        new_token.token_content = " ";
        new_token.token_cat = "BLOCK COMMENT";

        token_list.push_back(new_token);
        num_of_token++;
    } else {
        //std::cout << start_line << " " << end_line << " " <<
//num_of_token << " " << " "
         //<< "NESTED COMMENT" << std::endl;
        alpha_token_t new_token;
        new_token.token_line_start = start_line;
        new_token.token_line_end = end_line;
        new_token.token_number = num_of_token;
        new_token.token_content = " ";
        new_token.token_cat = "NESTED COMMENT";

        token_list.push_back(new_token);
    }
    return;
}

void handle_string(int token_line, std::string token_content) {
    //std::cout << token_line << " " << num_of_token << " " <<
//token_content << " "
      //   << "STRING CONST" << std::endl;
    alpha_token_t new_token;
    new_token.token_line_start = token_line;
    new_token.token_line_end = token_line;
    new_token.token_number = num_of_token;
    new_token.token_content = token_content;
    new_token.token_cat = "STRING CONST";

    token_list.push_back(new_token);
    num_of_token++;
    return;
}
