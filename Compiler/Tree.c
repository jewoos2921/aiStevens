//
// Created by jewoo on 2024-06-14.
//


#include "Defs.h"
#include "Data.h"
#include "Decl.h"


// Build and returns a generic AST node
struct ASTNode *makeASTNode(int op_,
                            struct ASTNode *left, struct ASTNode *right_,
                            int int_value_) {


    struct ASTNode *node = (struct ASTNode *) malloc(sizeof(struct ASTNode));
    if (node == NULL) {
        fprintf(stderr, "Unable to allocate memory for AST node\n");
        exit(1);
    }
    node->op_ = op_;
    node->left_ = left;
    node->right_ = right_;
    node->int_value_ = int_value_;
    return node;
}

struct ASTNode *makeASTLeaf(int op_, int int_value_) {
    return makeASTNode(op_, NULL, NULL, int_value_);
}

struct ASTNode *makeASTUnary(int op_,
                             struct ASTNode *left_,
                             int int_value_) {
    return makeASTNode(op_, left_, NULL, int_value_);
}