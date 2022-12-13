/**
 * Symbol table implementation using BST
 * @file: symbol_table.cpp
 * @date: 13.12.2022
 */

#include "symbol_table.h"

#include <utility>

SymbolTableTreeNode *global_symbol_table = nullptr;

SymbolTableTreeNode::SymbolTableTreeNode(std::string *key) {
    this->key = key;
    this->data = new SymbolTableTreeData();
    this->left = nullptr;
    this->right = nullptr;
}

SymbolTableTreeNode::~SymbolTableTreeNode() {
    delete this->key;
    delete this->data;
    delete this->left;
    delete this->right;
}

SYN_TABLE_COMPARATOR_FLAG SymbolTableTreeNode::comparator(std::string *a, std::string *b) {
    return *a == *b ? SYN_TABLE_COMP_OP_EQ : *a < *b ? SYN_TABLE_COMP_OP_LT : SYN_TABLE_COMP_OP_GT;
}

SymbolTableTreeNode *SymbolTableTreeNode::find(std::string *search_key) {
    auto comp = comparator(search_key, this->key);

    if (comp == SYN_TABLE_COMP_OP_EQ) {
        return this;
    } else if (comp == SYN_TABLE_COMP_OP_LT) {
        return this->left->find(search_key);
    } else {
        return this->right->find(search_key);
    }
}

SymbolTableTreeNode *SymbolTableTreeNode::insert(std::string *insert_key) {
    if (global_symbol_table == nullptr) {
        global_symbol_table = new SymbolTableTreeNode(insert_key);
        return this;
    }

    auto comp = comparator(insert_key, this->key);

    if (comp == SYN_TABLE_COMP_OP_EQ) {
        return this;
    } else if (comp == SYN_TABLE_COMP_OP_LT) {
        if (this->left == nullptr) {
            this->left = new SymbolTableTreeNode(insert_key);
            return this->left;
        } else {
            return this->left->insert(insert_key);
        }
    } else {
        if (this->right == nullptr) {
            this->right = new SymbolTableTreeNode(insert_key);
            return this->right;
        } else {
            return this->right->insert(insert_key);
        }
    }
}

SymbolTableTreeNode *SymbolTableTreeNode::remove(std::string *remove_key) {
    auto comp = comparator(remove_key, this->key);

    if (comp == SYN_TABLE_COMP_OP_EQ) {
        if (this->left == nullptr && this->right == nullptr) {
            delete this;
            return nullptr;
        } else if (this->left == nullptr) {
            return this->right;
        } else if (this->right == nullptr) {
            return this->left;
        } else {
            auto replacement = this->right;

            while (replacement->left != nullptr) replacement = replacement->left;

            this->key = replacement->key;
            this->data = replacement->data;
            this->right = replacement->remove(replacement->key);

            return this;
        }
    } else if (comp == SYN_TABLE_COMP_OP_LT) {
        if (this->left == nullptr) {
            return this;
        } else {
            this->left = this->left->remove(remove_key);
            return this;
        }
    } else {
        if (this->right == nullptr) {
            return this;
        } else {
            this->right = this->right->remove(remove_key);
            return this;
        }
    }
}

SymbolTableTreeData *SymbolTableTreeNode::get_data() const { return this->data; }

void SymbolTableTreeData::set_flag(SYM_TABLE_NODE_FLAG flag) { this->flags |= flag; }

void SymbolTableTreeData::unset_flag(SYM_TABLE_NODE_FLAG flag) { this->flags &= ~flag; }

SYM_TABLE_DATA_TYPE SymbolTableTreeData::get_type() const { return this->type; }

SYM_TABLE_NODE_FLAG SymbolTableTreeData::get_flags() const { return this->flags; }
