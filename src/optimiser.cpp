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

void Optimiser::replace_variable_usage() {
    std::vector<SyntaxTree> replace_trees;
    SyntaxTree *current_tree = root_tree;

    while (current_tree->left) {
        if (current_tree->right == current_replace_tree) break;

        if (current_tree->right != current_replace_tree && current_tree->right->type == SYN_NODE_ASSIGNMENT &&
            *current_tree->right->left->value == *current_replace_tree->left->value) {
            replace_trees.clear();
        }

        replace_trees.push_back(*current_tree);
        current_tree = current_tree->left;
    }

    auto replacer = [&](SyntaxTree *tree) {
        if (tree->type == SYN_NODE_IDENTIFIER && *tree->value == *current_replace_tree->left->value) {
            tree->type = current_replace_tree->right->type;
            tree->value = current_replace_tree->right->value;
        }
    };

    for (auto &tree: replace_trees) {
        auto replacing_tree = tree.right;

        replacing_tree->process_tree_using(
                [&](SyntaxTree *_tree) {
                    if (_tree->type == SYN_NODE_ASSIGNMENT) { _tree->right->process_tree_using(replacer, INORDER); }
                },
                POSTORDER);
    }
}

void Optimiser::optimize_assignment(SyntaxTree *tree) {
    tree->right->process_tree_using([&](SyntaxTree *tree) { calculate_expression(tree); }, POSTORDER);
    current_replace_tree = tree;
    replace_variable_usage();
}

void Optimiser::optimize() {
    root_tree->process_tree_using(
            [&](SyntaxTree *tree) {
                if (tree->type == SYN_NODE_ASSIGNMENT) { optimize_assignment(tree); }
            },
            POSTORDER);
}
