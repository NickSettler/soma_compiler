/**
 *
 * @file: lexical_analysis.h
 * @date: 11.12.2022
 */

#ifndef SOMA_COMPILER_LEXICAL_ANALYSIS_H
#define SOMA_COMPILER_LEXICAL_ANALYSIS_H

#include <fstream>
#include <map>
#include <string>
#include <vector>

#define START_FALLBACK                                                                                                 \
    state = LEX_START_STATE;                                                                                           \
    input_stream->unget();

typedef enum {
    LEX_START_STATE,
    LEX_OPERATOR_STATE,
    LEX_INTEGER_STATE,
    LEX_FLOAT_STATE,
} LEXICAL_ANALYSIS_STATE;

typedef enum {
    // System types
    LEX_TOKEN_EOF,

    // Bracket types
    LEX_TOKEN_LEFT_PARENTHESIS,
    LEX_TOKEN_RIGHT_PARENTHESIS,
    LEX_TOKEN_LEFT_SQUARE_BRACKET,
    LEX_TOKEN_RIGHT_SQUARE_BRACKET,
    LEX_TOKEN_LEFT_CURLY_BRACKET,
    LEX_TOKEN_RIGHT_CURLY_BRACKET,

    // Operator types
    LEX_TOKEN_PLUS,
    LEX_TOKEN_MINUS,
    LEX_TOKEN_MULTIPLY,
    LEX_TOKEN_DIVIDE,

    // Literal types
    LEX_TOKEN_INTEGER_LITERAL,
    LEX_TOKEN_FLOAT_LITERAL,
} LEXICAL_TOKEN_TYPE;

const std::vector<std::string> whitespaces = {" ", "\t", "\n"};

const std::vector<char> end = {'\0', EOF};

const std::map<std::string, LEXICAL_TOKEN_TYPE> brackets = {
        {"(", LEX_TOKEN_LEFT_PARENTHESIS},    {")", LEX_TOKEN_RIGHT_PARENTHESIS},
        {"[", LEX_TOKEN_LEFT_SQUARE_BRACKET}, {"]", LEX_TOKEN_RIGHT_SQUARE_BRACKET},
        {"{", LEX_TOKEN_LEFT_CURLY_BRACKET},  {"}", LEX_TOKEN_RIGHT_CURLY_BRACKET},
};

const std::map<std::string, LEXICAL_TOKEN_TYPE> operators = {
        {"+", LEX_TOKEN_PLUS},
        {"-", LEX_TOKEN_MINUS},
        {"*", LEX_TOKEN_MULTIPLY},
        {"/", LEX_TOKEN_DIVIDE},
};

class LexicalToken {
private:
    std::string value;
    LEXICAL_TOKEN_TYPE type;

public:
    LexicalToken(std::string value, LEXICAL_TOKEN_TYPE type);

    ~LexicalToken();

    std::string get_value();

    LEXICAL_TOKEN_TYPE get_type();
};

class LexicalAnalysis {
private:
    std::istream *input_stream;
    LEXICAL_ANALYSIS_STATE state;

public:
    explicit LexicalAnalysis(std::istream *input_stream);

    LexicalToken *get_token();
};

#endif// SOMA_COMPILER_LEXICAL_ANALYSIS_H
