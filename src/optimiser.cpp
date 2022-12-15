/**
 * Optimiser process
 * @file: optimiser.cpp
 * @date: 12.12.2022
 */

#include "optimiser.h"
#include "syntax_analysis.h"
#include "semantic_analysis.h"

void Optimiser::calculate_expression(SyntaxTree *tree) {
    if (tree == nullptr) return;
    if (!(tree->type & (SYN_NODE_ADD | SYN_NODE_SUB | SYN_NODE_MUL | SYN_NODE_DIV))) return;

    auto can_optimize = tree->left && tree->left->type & (SYN_NODE_INTEGER_LITERAL | SYN_NODE_FLOAT_LITERAL) &&
                        tree->right && tree->right->type & (SYN_NODE_INTEGER_LITERAL | SYN_NODE_FLOAT_LITERAL);

    if (!can_optimize) return;

    bool is_float = tree->type == SYN_NODE_DIV || tree->left->type == SYN_NODE_FLOAT_LITERAL ||
                    tree->right->type == SYN_NODE_FLOAT_LITERAL;

    float left_number = std::stof(*tree->left->value);
    float right_number = std::stof(*tree->right->value);
    float result = optimiser_math_map.at(tree->type)(left_number, right_number);
    int result_int = (int) result;

    tree->type = is_float ? SYN_NODE_FLOAT_LITERAL : SYN_NODE_INTEGER_LITERAL;
    tree->value = new std::string(is_float ? std::to_string(result) : std::to_string(result_int));
    delete tree->left;
    tree->left = nullptr;
    delete tree->right;
    tree->right = nullptr;
}

void Optimiser::optimize_assignment(SyntaxTree *tree) {
    tree->right->process_tree_using([&](SyntaxTree *tree) { calculate_expression(tree); }, POSTORDER);
    current_replace_variable = tree->left->value;
    replace_variable_with_value(tree);
}

void Optimiser::optimize() {
    root_tree->process_tree_using(
            [&](SyntaxTree *tree) {
                if (tree->type == SYN_NODE_ASSIGNMENT) { optimize_assignment(tree); }
            },
            POSTORDER);
}
