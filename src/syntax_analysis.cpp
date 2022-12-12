/**
 * Syntax analysis processing tokens from lexical analysis and building syntax tree
 * @file: syntax_analysis.cpp
 * @date: 12.12.2022
 */

#include "lexical_analysis.h"
#include "syntax_analysis.h"
#include "errors.h"

std::map<LEXICAL_TOKEN_TYPE, SyntaxAnalysisAttribute> attributes = {
        {LEX_TOKEN_EOF, SyntaxAnalysisAttribute("EOF", false, false, -1, (SYNTAX_ANALYSIS_NODE_TYPE) -1)},
        {LEX_TOKEN_SEMICOLON, SyntaxAnalysisAttribute(";", false, false, -1, (SYNTAX_ANALYSIS_NODE_TYPE) -1)},
        {LEX_TOKEN_IDENTIFIER, SyntaxAnalysisAttribute("ID", false, false, -1, SYN_NODE_IDENTIFIER)},
        {LEX_TOKEN_LEFT_PARENTHESIS, SyntaxAnalysisAttribute("(", false, false, -1, (SYNTAX_ANALYSIS_NODE_TYPE) -1)},
        {LEX_TOKEN_RIGHT_PARENTHESIS, SyntaxAnalysisAttribute(")", false, false, -1, (SYNTAX_ANALYSIS_NODE_TYPE) -1)},
        {LEX_TOKEN_ASSIGN, SyntaxAnalysisAttribute("=", false, false, -1, SYN_NODE_ASSIGNMENT)},
        {LEX_TOKEN_PLUS, SyntaxAnalysisAttribute("+", true, true, 7, SYN_NODE_ADD)},
        {LEX_TOKEN_MINUS, SyntaxAnalysisAttribute("-", true, true, 7, SYN_NODE_SUB)},
        {LEX_TOKEN_MULTIPLY, SyntaxAnalysisAttribute("*", true, false, 8, SYN_NODE_MUL)},
        {LEX_TOKEN_DIVIDE, SyntaxAnalysisAttribute("/", true, false, 8, SYN_NODE_DIV)},
        {LEX_TOKEN_INTEGER_LITERAL,
         SyntaxAnalysisAttribute("INTEGER_LITERAL", false, false, -1, SYN_NODE_INTEGER_LITERAL)},
        {LEX_TOKEN_FLOAT_LITERAL, SyntaxAnalysisAttribute("FLOAT_LITERAL", false, false, -1, SYN_NODE_FLOAT_LITERAL)},
        {LEX_TOKEN_CONST, SyntaxAnalysisAttribute("CONST_KEYWORD", false, false, -1, (SYNTAX_ANALYSIS_NODE_TYPE) -1)},
        {LEX_TOKEN_VAR, SyntaxAnalysisAttribute("VAR_KEYWORD", false, false, -1, (SYNTAX_ANALYSIS_NODE_TYPE) -1)},
};

SyntaxAnalysisAttribute::SyntaxAnalysisAttribute(std::string text, bool is_binary_operator, bool is_unary_operator,
                                                 int precedence, SYNTAX_ANALYSIS_NODE_TYPE type) {
    this->text = std::move(text);
    this->is_binary_operator = is_binary_operator;
    this->is_unary_operator = is_unary_operator;
    this->precedence = precedence;
    this->type = type;
}

std::string SyntaxAnalysisAttribute::get_text() const { return text; }

bool SyntaxAnalysisAttribute::is_binary() const { return is_binary_operator; }

bool SyntaxAnalysisAttribute::is_unary() const { return is_unary_operator; }

int SyntaxAnalysisAttribute::get_precedence() const { return precedence; }

SYNTAX_ANALYSIS_NODE_TYPE SyntaxAnalysisAttribute::get_type() const { return type; }

SyntaxTree::SyntaxTree(SYNTAX_ANALYSIS_NODE_TYPE type, std::string *value) {
    this->type = type;
    this->value = value;
    this->left = nullptr;
    this->right = nullptr;
    this->attributes = SYN_TREE_ATTR_NONE;
}

SyntaxTree::SyntaxTree(SYNTAX_ANALYSIS_NODE_TYPE type, SyntaxTree *left, SyntaxTree *right) {
    this->type = type;
    this->value = nullptr;
    this->left = left;
    this->right = right;
    this->attributes = SYN_TREE_ATTR_NONE;
}

#pragma clang diagnostic push
#pragma ide diagnostic ignored "misc-no-recursion"
void SyntaxTree::process_tree_using(const std::function<void(SyntaxTree *)> &function, TRAVERSAL_TYPE traversal_type) {
    if (this == nullptr) return;

    if (traversal_type == PREORDER) function(this);
    if (this->left != nullptr) this->left->process_tree_using(function, traversal_type);
    if (traversal_type == INORDER) function(this);
    if (this->right != nullptr) this->right->process_tree_using(function, traversal_type);
    if (traversal_type == POSTORDER) function(this);
}
#pragma clang diagnostic pop

SyntaxAnalysis::SyntaxAnalysis(LexicalAnalysis *lexical_analysis)
    : current_token(nullptr), lexical_analysis(lexical_analysis) {}

void SyntaxAnalysis::expect_token(LEXICAL_TOKEN_TYPE type) {
    if (current_token->get_type() == type) {
        GET_NEXT_TOKEN
        return;
    }

    throw SyntaxAnalysisError("Unexpected token: %s. Expected: %s", current_token->get_value().c_str(),
                              attributes.at(type).get_text().c_str());
}

SyntaxTree *SyntaxAnalysis::expression(int precedence) {
    SyntaxTree *x = nullptr, *node;
    SYNTAX_ANALYSIS_NODE_TYPE op;

    switch (current_token->get_type()) {
        case LEX_TOKEN_LEFT_PARENTHESIS:
            x = parenthesis_expression();
            break;
        case LEX_TOKEN_INTEGER_LITERAL:
            x = new SyntaxTree(SYN_NODE_INTEGER_LITERAL, new std::string(current_token->get_value()));
            GET_NEXT_TOKEN
            break;
        case LEX_TOKEN_FLOAT_LITERAL:
            x = new SyntaxTree(SYN_NODE_FLOAT_LITERAL, new std::string(current_token->get_value()));
            GET_NEXT_TOKEN
            break;
        default:
            throw SyntaxAnalysisError("Expected expression but found: %s", current_token->get_value().c_str());
    }

    while (attributes.at(current_token->get_type()).is_binary() &&
           attributes.at(current_token->get_type()).get_precedence() >= precedence) {
        LEXICAL_TOKEN_TYPE internal_op = current_token->get_type();

        GET_NEXT_TOKEN

        int q = attributes.at(internal_op).get_precedence();

        node = expression(q + 1);
        x = new SyntaxTree(attributes.at(internal_op).get_type(), x, node);
    }

    return x;
}

SyntaxTree *SyntaxAnalysis::parenthesis_expression() {
    this->expect_token(LEX_TOKEN_LEFT_PARENTHESIS);
    auto *tree = expression(0);
    this->expect_token(LEX_TOKEN_RIGHT_PARENTHESIS);

    return tree;
}

SyntaxTree *SyntaxAnalysis::statement() {
    SyntaxTree *tree = nullptr, *v, *e, *s, *e2;

    switch (current_token->get_type()) {
        case LEX_TOKEN_INTEGER_LITERAL:
        case LEX_TOKEN_FLOAT_LITERAL:
            tree = expression(0);
            expect_token(LEX_TOKEN_SEMICOLON);
            break;
        case LEX_TOKEN_CONST:
        case LEX_TOKEN_VAR: {
            bool is_constant = current_token->get_type() == LEX_TOKEN_CONST;
            GET_NEXT_TOKEN

            v = new SyntaxTree(SYN_NODE_IDENTIFIER, new std::string(current_token->get_value()));

            expect_token(LEX_TOKEN_IDENTIFIER);
            expect_token(LEX_TOKEN_ASSIGN);

            tree = new SyntaxTree(SYN_NODE_ASSIGNMENT, v, expression(0));
            if (is_constant) tree->attributes |= SYN_TREE_ATTR_CONSTANT;

            expect_token(LEX_TOKEN_SEMICOLON);
            break;
        }
        default:
            throw SyntaxAnalysisError("Expected statement but found: %s", current_token->get_value().c_str());
    }

    return tree;
}

SyntaxTree *SyntaxAnalysis::build_tree() {
    GET_NEXT_TOKEN

    SyntaxTree *tree = nullptr;

    while (current_token != nullptr && current_token->get_type() != LEX_TOKEN_EOF) {
        tree = new SyntaxTree(SYN_NODE_SEQUENCE, tree, statement());
    }

    return tree;
}
