//
// Created by jewoo on 2024-06-14.
//

#include "Defs.h"
#include "Data.h"
#include "Decl.h"


// List of AT operators
static char *ASTop[] = {"+", "-",
                        "*", "/"};


// Given an AST, interpret the operatos in it and return a final value
int interpretAST(struct ASTNode *node_) {
    int leftval, rightval;

    if (node_->left_) {
        leftval = interpretAST(node_->left_);
    }
    if (node_->right_) {
        rightval = interpretAST(node_->right_);
    }

    // Debug: Print what we are about to do
    if (node_->op_ == A_INTLIT) {
        printf("int %d\n", node_->int_value_);
    } else {
        printf("%d %s %d\n", leftval, ASTop[node_->op_], rightval);
    }

    switch (node_->op_) {
        case A_ADD:
            return leftval + rightval;

        case A_SUBTRACT:
            return leftval - rightval;

        case A_MULTIPLY:
            return leftval * rightval;

        case A_DIVIDE:
            return leftval / rightval;

        case A_INTLIT:
            return node_->int_value_;

        default:
            fprintf(stderr, "Unknown AST operator %d\n", node_->op_);
            exit(1);
    }
}
