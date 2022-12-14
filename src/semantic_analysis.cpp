/**
 * Semantic analysis
 * @file: semantic_analysis.cpp
 * @date: 13.12.2022
 */

#include "util/errors.h"
#include "symbol_table.h"
#include "syntax_analysis.h"
#include "semantic_analysis.h"

extern SymbolTableTree *global_symbol_table;

SYM_TABLE_DATA_TYPE SemanticAnalysisUtil::type_checking(SYM_TABLE_DATA_TYPE type1, SYM_TABLE_DATA_TYPE type2) {
    return type1 > type2 ? type1 : type2;
}

bool SemanticAnalysis::is_defined(std::string *identifier) {
    if (identifier == nullptr || current_symbol_table == nullptr) return false;

    auto token = current_symbol_table->find(identifier);

    return token && token->data->get_flags() & SYM_TABLE_IS_DEFINED;
}

SYM_TABLE_DATA_TYPE SemanticAnalysis::get_data_type(SyntaxTree *tree) {
    if (tree == nullptr) return SYM_TABLE_TYPE_UNKNOWN;

    switch (tree->type) {
        case SYN_NODE_INTEGER_LITERAL:
            return SYM_TABLE_TYPE_INT;
        case SYN_NODE_FLOAT_LITERAL:
            return SYM_TABLE_TYPE_FLOAT;
        case SYN_NODE_IDENTIFIER: {
            auto token = current_symbol_table->find(tree->value);
            if (token == nullptr) throw SemanticAnalysisOtherError("Undefined identifier: %s", tree->value);

            return token->data->get_type();
        }
        case SYN_NODE_ADD:
        case SYN_NODE_SUB:
        case SYN_NODE_MUL:
            return SemanticAnalysisUtil::type_checking(get_data_type(tree->left), get_data_type(tree->right));
        case SYN_NODE_DIV:
            return SYM_TABLE_TYPE_FLOAT;
        default:
            return SYM_TABLE_TYPE_UNKNOWN;
    }
}

void SemanticAnalysis::process_assign(SyntaxTree *tree) {
    SymbolTableTreeNode *symtable_token;

    if (tree->attributes & SYN_TREE_ATTR_DECLARATION) {
        if (is_defined(tree->left->value))
            throw SemanticAnalysisRedefineVariableError("Variable %s is already declared", tree->left->value->c_str());

        symtable_token = current_symbol_table->insert(tree->left->value);
    } else {
        symtable_token = current_symbol_table->find(tree->left->value);

        if (symtable_token == nullptr)
            throw SemanticAnalysisUndefinedVariableError("Variable %s is not declared", tree->left->value->c_str());

        if (symtable_token->data->get_flags() & SYM_TABLE_IS_CONSTANT)
            throw SemanticAnalysisReassignConstantError("Variable %s is constant and cannot be reassigned",
                                                        tree->left->value->c_str());
    }

    if (tree->attributes & SYN_TREE_ATTR_CONSTANT) symtable_token->data->set_flag(SYM_TABLE_IS_CONSTANT);

    tree->right->process_tree_using(
            [this](SyntaxTree *expression_tree) {
                if (expression_tree->type != SYN_NODE_IDENTIFIER) return;

                if (!is_defined(expression_tree->value)) {
                    throw SemanticAnalysisUndefinedVariableError("Variable %s is used before definition",
                                                                 expression_tree->value->c_str());
                }
            },
            POSTORDER);

    symtable_token->data->set_type(get_data_type(tree->right));

    symtable_token->data->set_flag(SYM_TABLE_IS_DEFINED);
}

void SemanticAnalysis::analyze_tree(SyntaxTree *syntax_tree) {
    current_symbol_table = global_symbol_table;

    syntax_tree->process_tree_using(
            [this](SyntaxTree *tree) {
                if (tree->type == SYN_NODE_ASSIGNMENT) { process_assign(tree); }
            },
            POSTORDER);
}
