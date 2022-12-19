/**
 * Optimiser process
 * @file: optimiser.h
 * @date: 12.12.2022
 */

#ifndef SOMA_COMPILER_OPTIMISER_H
#define SOMA_COMPILER_OPTIMISER_H

#include <map>
#include <string>
#include <functional>
#include <vector>
#include "util/types.h"

class SyntaxTree;

const std::map<SYNTAX_ANALYSIS_NODE_TYPE, const std::function<float(float, float)>> optimiser_math_map = {
        {SYN_NODE_ADD, std::plus<>()},
        {SYN_NODE_SUB, std::minus<>()},
        {SYN_NODE_MUL, std::multiplies<>()},
        {SYN_NODE_DIV, std::divides<>()},
};

class Optimiser {
private:
    SyntaxTree *root_tree;
    SyntaxTree *current_replace_tree;

public:
    Optimiser(SyntaxTree *tree) : root_tree(tree) {}

    ~Optimiser() = default;

    static void calculate_expression(SyntaxTree *tree);

    void replace_variable_usage();

    void optimize_assignment(SyntaxTree *tree);

    void optimize();
};

#endif// SOMA_COMPILER_OPTIMISER_H
