/**
 * Semantic analysis
 * @file: semantic_analysis.h
 * @date: 13.12.2022
 */

#ifndef SOMA_COMPILER_SEMANTIC_ANALYSIS_H
#define SOMA_COMPILER_SEMANTIC_ANALYSIS_H

class SyntaxTree;

class SemanticAnalysis {
public:
    SemanticAnalysis() = default;

    ~SemanticAnalysis() = default;

    static bool is_defined(std::string *identifier);

    void process_assign(SyntaxTree *tree);

    void analyze_tree(SyntaxTree *syntax_tree);
};

#endif// SOMA_COMPILER_SEMANTIC_ANALYSIS_H
