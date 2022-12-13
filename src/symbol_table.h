/**
 * Symbol table implementation using BST
 * @file: symbol_table.h
 * @date: 13.12.2022
 */

#ifndef SOMA_COMPILER_SYMBOL_TABLE_H
#define SOMA_COMPILER_SYMBOL_TABLE_H

#include <string>
#include "util/enum.h"

typedef enum {
    SYM_TABLE_TYPE_INT = 0x01,
    SYM_TABLE_TYPE_FLOAT = 0x02,
} SYM_TABLE_DATA_TYPE;

ENUM_BIT_CASTING(SYM_TABLE_DATA_TYPE)

typedef enum {
    SYM_TABLE_IS_DEFINED = 0x01,
    SYM_TABLE_IS_CONSTANT = 0x02,
} SYM_TABLE_NODE_FLAG;

ENUM_BIT_CASTING(SYM_TABLE_NODE_FLAG)

class SymbolTableTreeData {
private:
    SYM_TABLE_DATA_TYPE type;
    SYM_TABLE_NODE_FLAG flags;

public:
    SymbolTableTreeData() = default;

    void set_flag(SYM_TABLE_NODE_FLAG flag);

    void unset_flag(SYM_TABLE_NODE_FLAG flag);

    SYM_TABLE_DATA_TYPE get_type() const;

    SYM_TABLE_NODE_FLAG get_flags() const;
};

typedef enum {
    SYN_TABLE_COMP_OP_EQ,
    SYN_TABLE_COMP_OP_LT,
    SYN_TABLE_COMP_OP_GT,
} SYN_TABLE_COMPARATOR_FLAG;

class SymbolTableTreeNode {
public:
    std::string *key;
    SymbolTableTreeData *data;
    SymbolTableTreeNode *left;
    SymbolTableTreeNode *right;

    explicit SymbolTableTreeNode(std::string *key);

    ~SymbolTableTreeNode();
};

class SymbolTableTree {
private:
    SymbolTableTreeNode *root = nullptr;

    static SYN_TABLE_COMPARATOR_FLAG comparator(std::string *a, std::string *b);

    SymbolTableTreeNode *find(std::string *search_key, SymbolTableTreeNode *node);

    SymbolTableTreeNode *insert(std::string *insert_key, SymbolTableTreeNode *node);

    void remove(std::string *remove_key, SymbolTableTreeNode *node);

public:
    ~SymbolTableTree();

    SymbolTableTreeNode *find(std::string *search_key);

    SymbolTableTreeNode *insert(std::string *insert_key);

    void remove(std::string *remove_key);
};

#endif// SOMA_COMPILER_SYMBOL_TABLE_H
