/**
 *
 * @file: lexical_analysis.cpp
 * @date: 11.12.2022
 */

#include "lexical_analysis.h"
#include "errors.h"

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
                switch (c) {
                    case ' ':
                    case '\n':
                    case '\t':
                        break;
                    case EOF:
                    case '\0':
                        token = new LexicalToken("", LEX_TOKEN_EOF);
                        return token;
                    case '+':
                    case '-':
                    case '*':
                    case '/':
                    case '=':
                        token_value->push_back(c);
                        state = LEX_OPERATOR_STATE;
                        break;
                    case '(':
                    case ')':
                    case '[':
                    case ']':
                    case '{':
                    case '}':
                        token = new LexicalToken(char_str, brackets.find(char_str)->second);
                        return token;
                    default:
                        if (isdigit(c)) {
                            token_value->push_back(c);
                            state = LEX_INTEGER_STATE;
                            break;
                        } else if (isalnum(c)) {
                            token_value->push_back(c);
                            state = LEX_KEYWORD_IDENTIFIER_STATE;
                            break;
                        } else {
                            throw LexicalAnalysisError("Unexpected character: %c", c);
                        }
                }
                break;
            }
            case LEX_OPERATOR_STATE: {
                if (c == '+' || c == '-' || c == '*' || c == '/' || c == '=') {
                    token_value->push_back(c);
                    break;
                }

                START_FALLBACK

                auto operator_type = operators.find(*token_value);
                if (operator_type == operators.end())
                    throw LexicalAnalysisError("Unknown operator: %s", token_value->c_str());

                token = new LexicalToken(*token_value, operator_type->second);
                return token;
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

                START_FALLBACK
                token = new LexicalToken(*token_value, LEX_TOKEN_INTEGER_LITERAL);
                return token;
            }
            case LEX_FLOAT_STATE: {
                if (isdigit(c) || c == '+' || c == '-' || c == 'e' || c == 'E' || c == '.') {
                    token_value->push_back(c);
                    continue;
                }

                if (!std::regex_match(token_value->c_str(),
                                      std::regex(R"(^(\d+([.]\d*)?([eE][+-]?\d+)?|[.]\d+([eE][+-]?\d+)?)$)"))) {
                    throw LexicalAnalysisError("Invalid float: %s", token_value->c_str());
                }

                START_FALLBACK
                token = new LexicalToken(*token_value, LEX_TOKEN_FLOAT_LITERAL);
                return token;
            }
            case LEX_KEYWORD_IDENTIFIER_STATE: {
                if (isalnum(c) || c == '_') {
                    token_value->push_back(c);
                    continue;
                }

                START_FALLBACK

                auto keyword = keywords.find(*token_value);
                token = new LexicalToken(*token_value,
                                         keyword == keywords.end() ? LEX_TOKEN_IDENTIFIER : keyword->second);
                return token;
            }
            default: {
                return nullptr;
            }
        }
    }
}
