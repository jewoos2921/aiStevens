//
// Created by jewoo on 2024-06-14.
//

#include "Defs.h"
#include "Data.h"
#include "Decl.h"

// Generic code generator

// Given an AST, generate assembly code recursively
static int genAST(struct ASTNode *node) {
    int leftreg, rightreg;

    // Get the left and right sub-tree values
    if (node->left_) {
        leftreg = genAST(node->left_);
    }
    if (node->right_) {
        rightreg = genAST(node->right_);
    }

    switch (node->op_) {
        case A_ADD:
            return cgAdd(leftreg, rightreg);
        case A_SUBTRACT:
            return cgSub(leftreg, rightreg);
        case A_MULTIPLY:
            return cgMul(leftreg, rightreg);
        case A_DIVIDE:
            return cgDiv(leftreg, rightreg);
        case A_INTLIT:
            return cgLoad(node->int_value_);

        default:
            fprintf(stderr, "Unknown AST operator %d\n", node->op_);
            exit(1);
    }
}


void generateCode(struct ASTNode *node_) {
    int reg;

    cgPreamble();
    reg = genAST(node_);
    cgPrintInt(reg);
    cgPostamble();
}