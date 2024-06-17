//
// Created by jewoo on 2024-06-14.
//


#include "Defs.h"
#include "Data.h"
#include "Decl.h"


// Build and returns a generic AST node
struct ASTNode *makeASTNode(int op_,
                            struct ASTNode *left, struct ASTNode *mid_, struct ASTNode *right_,
                            int int_value_) {


    struct ASTNode *node = (struct ASTNode *) malloc(sizeof(struct ASTNode));
    if (node == NULL)
        fatal("Unalbe to malloc in makeASTNode()");

    node->op_ = op_;
    node->left_ = left;
    node->mid_ = mid_;
    node->right_ = right_;
    node->v_.int_value_ = int_value_;
    return node;
}

struct ASTNode *makeASTLeaf(int op_, int int_value_) {
    return makeASTNode(op_, NULL, NULL, NULL, int_value_);
}

struct ASTNode *makeASTUnary(int op_,
                             struct ASTNode *left_,
                             int int_value_) {
    return makeASTNode(op_, left_, NULL, NULL, int_value_);
}