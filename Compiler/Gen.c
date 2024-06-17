//
// Created by jewoo on 2024-06-14.
//

#include "Defs.h"
#include "Data.h"
#include "Decl.h"

// Generic code generator

// Given an AST, generate assembly code recursively
int genAST(struct ASTNode *node, int reg_) {
    int leftreg, rightreg;

    // Get the left and right sub-tree values
    if (node->left_) {
        leftreg = genAST(node->left_, -1);
    }

    if (node->right_) {
        rightreg = genAST(node->right_, leftreg);
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
        case A_EQ:
            return cgEqual(leftreg, rightreg);
        case A_NE:
            return cgNotEqual(leftreg, rightreg);
        case A_LT:
            return cgLessThan(leftreg, rightreg);
        case A_GT:
            return cgGreaterThan(leftreg, rightreg);
        case A_LE:
            return cgLessEqual(leftreg, rightreg);
        case A_GE:
            return cgGreaterEqual(leftreg, rightreg);

        case A_INTLIT:
            return cgLoadInt(node->v_.int_value_);
        case A_IDENT:
            return cgLoadGlobal(Gsym_[node->v_.id_].name_);
        case A_LVIDENT:
            return cgStorGlob(reg_, Gsym_[node->v_.id_].name_);
        case A_ASSIGN:
            // The work has aleady been done, return the result
            return rightreg;
        default:
            fatald("Unknown AST operator", node->op_);
    }
}

void genPreamble() {
    cgPreamble();
}

void genPostamble() {
    cgPostamble();
}

void genFreeRegs() {
    freeAllRegisters();
}

void genPrintInt(int reg_) {
    cgPrintInt(reg_);
}

void genGlobalSymbols(char *s_) {
    cgGlobSym(s_);
}