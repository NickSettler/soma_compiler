/**
 * Semantic analysis
 * @file: semantic_analysis.h
 * @date: 13.12.2022
 */

#ifndef SOMA_COMPILER_SEMANTIC_ANALYSIS_H
#define SOMA_COMPILER_SEMANTIC_ANALYSIS_H

#include "util/types.h"

class SyntaxTree;

class SymbolTableTree;

class SemanticAnalysisUtil {
public:
    static SYM_TABLE_DATA_TYPE type_checking(SYM_TABLE_DATA_TYPE type1, SYM_TABLE_DATA_TYPE type2);
};

class SemanticAnalysis {
private:
    SymbolTableTree *current_symbol_table;

public:
    SemanticAnalysis() = default;

    ~SemanticAnalysis() = default;

    bool is_defined(std::string *identifier);

    void process_assign(SyntaxTree *tree);

    void analyze_tree(SyntaxTree *syntax_tree);
};

#endif// SOMA_COMPILER_SEMANTIC_ANALYSIS_H
