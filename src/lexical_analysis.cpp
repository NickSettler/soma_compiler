/**
 *
 * @file: lexical_analysis.cpp
 * @date: 11.12.2022
 */

#include "lexical_analysis.h"

#include <regex>
#include <utility>

LexicalToken::LexicalToken(std::string value, LEXICAL_TOKEN_TYPE type) {
    this->value = std::move(value);
    this->type = type;
}

LexicalToken::~LexicalToken() = default;

std::string LexicalToken::get_value() { return value; }

LEXICAL_TOKEN_TYPE LexicalToken::get_type() { return type; }

LexicalAnalysis::LexicalAnalysis(std::istream *input_stream) {
    this->input_stream = input_stream;
    state = LEX_START_STATE;
}

LexicalToken *LexicalAnalysis::get_token() {
    auto token_value = new std::string();
    LexicalToken *token;

    while (true) {
        char c = (char) input_stream->get();

        switch (state) {
            case LEX_START_STATE: {
                std::string char_str = std::string(1, c);

                if (std::find(end.begin(), end.end(), c) != end.end()) {
                    token = new LexicalToken("", LEX_TOKEN_EOF);
                    return token;
                } else if (std::find(whitespaces.begin(), whitespaces.end(), char_str) != whitespaces.end()) {
                    break;
                } else if (brackets.find(char_str) != brackets.end()) {
                    token_value->push_back(c);
                    token = new LexicalToken(char_str, brackets.find(char_str)->second);
                    return token;
                } else if (operators.find(char_str) != operators.end()) {
                    token_value->push_back(c);
                    token = new LexicalToken(char_str, operators.find(char_str)->second);
                    return token;
                } else {
                    if (isdigit(c)) {
                        token_value->push_back(c);
                        state = LEX_INTEGER_STATE;
                        break;
                    } else {
                        printf("Unexpected character: %c\n", c);
                        return nullptr;
                    }
                }
            }
            case LEX_INTEGER_STATE: {
                if (isdigit(c)) {
                    token_value->push_back(c);
                    continue;
                } else if (c == '.' || c == 'e' || c == 'E') {
                    token_value->push_back(c);
                    state = LEX_FLOAT_STATE;
                    continue;
                }

                token = new LexicalToken(*token_value, LEX_TOKEN_INTEGER_LITERAL);
                state = LEX_START_STATE;
                input_stream->unget();
                return token;
            }
            case LEX_FLOAT_STATE: {
                if (isdigit(c) || c == '+' || c == '-') {
                    token_value->push_back(c);
                    continue;
                }

                if (!std::regex_match(token_value->c_str(), std::regex("([0-9]*[.eE])-?[0-9]+"))) {
                    throw std::runtime_error("Invalid float literal");
                }

                token = new LexicalToken(*token_value, LEX_TOKEN_FLOAT_LITERAL);
                state = LEX_START_STATE;
                input_stream->unget();
                return token;
            }
            default: {
                return nullptr;
            }
        }
    }
}
