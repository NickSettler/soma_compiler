/**
 * Syntax analysis processing tokens from lexical analysis and building syntax tree
 * @file: syntax_analysis.h
 * @date: 12.12.2022
 */

#ifndef SOMA_COMPILER_SYNTAX_ANALYSIS_H
#define SOMA_COMPILER_SYNTAX_ANALYSIS_H

#include <map>
#include <string>

#define GET_NEXT_TOKEN                                                                                                 \
    delete current_token;                                                                                              \
    current_token = lexical_analysis->get_token();

typedef enum {
    // System types
    SYN_NODE_SEQUENCE,

    // Operator types
    SYN_NODE_ADD,
    SYN_NODE_SUB,
    SYN_NODE_MUL,
    SYN_NODE_DIV,

    // Literal types
    SYN_NODE_INTEGER_LITERAL,
    SYN_NODE_FLOAT_LITERAL,
} SYNTAX_ANALYSIS_NODE_TYPE;

class SyntaxAnalysisAttribute {
private:
    std::string text;
    bool is_binary_operator;
    bool is_unary_operator;
    int precedence;
    SYNTAX_ANALYSIS_NODE_TYPE type;

public:
    SyntaxAnalysisAttribute(std::string text, bool is_binary_operator, bool is_unary_operator, int precedence,
                            SYNTAX_ANALYSIS_NODE_TYPE type);

    std::string get_text() const;

    bool is_binary() const;

    bool is_unary() const;

    int get_precedence() const;

    SYNTAX_ANALYSIS_NODE_TYPE get_type() const;
};

class SyntaxTree {
public:
    SYNTAX_ANALYSIS_NODE_TYPE type;
    std::string *value;
    SyntaxTree *left;
    SyntaxTree *right;

    SyntaxTree(SYNTAX_ANALYSIS_NODE_TYPE type, std::string *value);
    SyntaxTree(SYNTAX_ANALYSIS_NODE_TYPE type, SyntaxTree *left, SyntaxTree *right);
};

class SyntaxAnalysis {
private:
    LexicalAnalysis *lexical_analysis;
    LexicalToken *current_token;

    void expect_token(LEXICAL_TOKEN_TYPE type);

public:
    explicit SyntaxAnalysis(LexicalAnalysis *lexical_analysis);

    SyntaxTree *expression(int precedence);

    SyntaxTree *parenthesis_expression();

    SyntaxTree *statement();

    SyntaxTree *build_tree();
};

#endif// SOMA_COMPILER_SYNTAX_ANALYSIS_H
