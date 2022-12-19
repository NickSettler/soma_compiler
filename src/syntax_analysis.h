/**
 * Syntax analysis processing tokens from lexical analysis and building syntax tree
 * @file: syntax_analysis.h
 * @date: 12.12.2022
 */

#ifndef SOMA_COMPILER_SYNTAX_ANALYSIS_H
#define SOMA_COMPILER_SYNTAX_ANALYSIS_H

#include <map>
#include <string>
#include <functional>
#include "util/enum.h"
#include "util/types.h"

#define GET_NEXT_TOKEN                                                                                                 \
    delete current_token;                                                                                              \
    current_token = lexical_analysis->get_token();

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

typedef enum {
    PREORDER,
    INORDER,
    POSTORDER,
} TRAVERSAL_TYPE;

typedef enum {
    SYN_TREE_ATTR_NONE = 0x00,
    SYN_TREE_ATTR_CONSTANT = 0x01,
    SYN_TREE_ATTR_DECLARATION = 0x02,
} SYN_TREE_ATTRIBUTE;

ENUM_BIT_CASTING(SYN_TREE_ATTRIBUTE)

class SyntaxTree {
public:
    SYNTAX_ANALYSIS_NODE_TYPE type;
    std::string *value;
    SyntaxTree *left;
    SyntaxTree *right;
    SYN_TREE_ATTRIBUTE attributes;

    SyntaxTree(SYNTAX_ANALYSIS_NODE_TYPE type, std::string *value);

    SyntaxTree(SYNTAX_ANALYSIS_NODE_TYPE type, SyntaxTree *left, SyntaxTree *right);

    void process_tree_using(const std::function<void(SyntaxTree *)> &function, TRAVERSAL_TYPE traversal_type);
};

class LexicalAnalysis;

class LexicalToken;

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
