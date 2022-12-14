/**
 * Semantic analysis
 * @file: semantic_analysis.cpp
 * @date: 13.12.2022
 */

#include "symbol_table.h"
#include "syntax_analysis.h"
#include "semantic_analysis.h"

extern SymbolTableTree *global_symbol_table;

bool SemanticAnalysis::is_defined(std::string *identifier) {
    if (identifier == nullptr || global_symbol_table == nullptr) return false;

    auto token = global_symbol_table->find(identifier);

    return token && token->data->get_flags() & SYM_TABLE_IS_DEFINED;
}

void SemanticAnalysis::process_assign(SyntaxTree *tree) {
    auto symtable_token = global_symbol_table->insert(tree->left->value);

    if (tree->attributes & SYN_TREE_ATTR_CONSTANT) symtable_token->data->set_flag(SYM_TABLE_IS_CONSTANT);

    tree->right->process_tree_using(
            [this](SyntaxTree *expression_tree) {
                if (expression_tree->type != SYN_NODE_IDENTIFIER) return;

                if (!is_defined(expression_tree->value)) {
                    throw std::runtime_error("Semantic error: variable " + *expression_tree->value + " is not defined");
                }
            },
            POSTORDER);

    symtable_token->data->set_flag(SYM_TABLE_IS_DEFINED);
}

void SemanticAnalysis::analyze_tree(SyntaxTree *syntax_tree) {
    syntax_tree->process_tree_using(
            [this](SyntaxTree *tree) {
                if (tree->type == SYN_NODE_ASSIGNMENT) { process_assign(tree); }
            },
            POSTORDER);
}