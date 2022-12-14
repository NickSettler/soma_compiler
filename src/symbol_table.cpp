/**
 * Symbol table implementation using BST
 * @file: symbol_table.cpp
 * @date: 13.12.2022
 */

#include "symbol_table.h"

#include <utility>

auto *global_symbol_table = new SymbolTableTree();

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

void SymbolTableTreeData::set_flag(SYM_TABLE_NODE_FLAG flag) { this->flags |= flag; }

void SymbolTableTreeData::unset_flag(SYM_TABLE_NODE_FLAG flag) { this->flags &= ~flag; }

void SymbolTableTreeData::set_type(SYM_TABLE_DATA_TYPE new_type) { this->type = new_type; }

SYM_TABLE_DATA_TYPE SymbolTableTreeData::get_type() const { return this->type; }

SYM_TABLE_NODE_FLAG SymbolTableTreeData::get_flags() const { return this->flags; }

SYN_TABLE_COMPARATOR_FLAG SymbolTableTree::comparator(std::string *a, std::string *b) {
    return *a == *b ? SYN_TABLE_COMP_OP_EQ : *a < *b ? SYN_TABLE_COMP_OP_LT : SYN_TABLE_COMP_OP_GT;
}

SymbolTableTree::~SymbolTableTree() {
    delete this->root;
    this->root = nullptr;
}

SymbolTableTreeNode *SymbolTableTree::find(std::string *search_key, SymbolTableTreeNode *node) {
    if (node == nullptr) return nullptr;

    auto comp = comparator(search_key, node->key);

    if (comp == SYN_TABLE_COMP_OP_EQ) {
        return node;
    } else if (comp == SYN_TABLE_COMP_OP_LT) {
        return find(search_key, node->left);
    } else {
        return find(search_key, node->right);
    }
}

SymbolTableTreeNode *SymbolTableTree::find(std::string *search_key) {
    if (this->root == nullptr) return nullptr;

    return find(search_key, root);
}

SymbolTableTreeNode *SymbolTableTree::insert(std::string *insert_key, SymbolTableTreeNode *node) {
    auto comp = comparator(insert_key, node->key);

    if (comp == SYN_TABLE_COMP_OP_EQ) {
        return node;
    } else if (comp == SYN_TABLE_COMP_OP_LT) {
        if (node->left == nullptr) {
            node->left = new SymbolTableTreeNode(insert_key);
            return node->left;
        } else {
            return insert(insert_key, node->left);
        }
    } else {
        if (node->right == nullptr) {
            node->right = new SymbolTableTreeNode(insert_key);
            return node->right;
        } else {
            return insert(insert_key, node->right);
        }
    }
}

SymbolTableTreeNode *SymbolTableTree::insert(std::string *insert_key) {
    if (this->root == nullptr) {
        this->root = new SymbolTableTreeNode(insert_key);
        return this->root;
    }

    return insert(insert_key, this->root);
}

void SymbolTableTree::remove(std::string *remove_key, SymbolTableTreeNode *node) {
    auto comp = comparator(remove_key, node->key);

    if (comp == SYN_TABLE_COMP_OP_EQ) {
        if (node->left == nullptr && node->right == nullptr) {
            delete node;
        } else if (node->left == nullptr || node->right == nullptr) {
            node->key = node->right->key;
            node->data = node->right->data;
            node->left = node->right->left;
            node->right = node->right->right;
        } else {
            auto replacement = node->right;

            while (replacement->left != nullptr) replacement = replacement->left;

            node->key = replacement->key;
            node->data = replacement->data;
            node->right = replacement->right;
        }
    } else if (comp == SYN_TABLE_COMP_OP_LT) {
        if (node->left == nullptr) return;

        return remove(remove_key, node->left);
    } else {
        if (node->right == nullptr) return;

        return remove(remove_key, node->right);
    }
}

void SymbolTableTree::remove(std::string *insert_key) {
    if (this->root == nullptr) return;

    return remove(insert_key, this->root);
}
