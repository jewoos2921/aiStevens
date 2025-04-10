//
// Created by jewoo on 2024-06-14.
//

#include "Defs.h"
#include "Data.h"
#include "Decl.h"

// Generic code generator

// Generate and return a new label number
int genLabel() {
    static int id = 1;
    return id++;
}

// Generate the code for an IF statement and an optional ELSE clause
static int genIfAST(struct ASTNode *node_) {
    int lFalse, lEnd;

    // Generate two labels: one for the
    // false compound statement, and one
    // for the end of the overall IF statement.
    // When there is no ELSE clause, Lfalse _is_
    // the ending label!
    lFalse = genLabel();
    if (node_->right_)
        lEnd = genLabel();

    // Generate the condition code followed
    // by a zero jump to the false label.
    // We cheat by sending the Lfalse label as a register.
    genAST(node_->left_, lFalse, node_->op_);
    genFreeRegs();

    // Generate the true compound statement
    genAST(node_->mid_, NOREG, node_->op_);
    genFreeRegs();

    // If there is an optional ELSE clause, generate the jump to skip to the end
    if (node_->right_)
        cgJump(lEnd);

    cgLabel(lFalse);

    // Optional ELSE clause: generate the false compound statement and the end label
    if (node_->right_) {
        genAST(node_->right_, NOREG, node_->op_);
        genFreeRegs();
        cgLabel(lEnd);
    }

    return NOREG;
}

// Generate the code for a WHILE statement
// and an optional ELSE clause
static int genWHILE(struct ASTNode *node_) {
    int lStart, lEnd;

    // Generate the start and end labels
    // and ouput the start label
    lStart = genLabel();
    lEnd = genLabel();
    cgLabel(lStart);

    // Generate the condition code followed
    // by a jump to the end label.
    // We cheat by sending the Lfalse label as a register.
    genAST(node_->left_, lEnd, node_->op_);
    genFreeRegs();

    // Generate the compound statement for the body.
    genAST(node_->right_, NOREG, node_->op_);
    genFreeRegs();

    // Finally output the jump back to the condition, and the end label
    cgJump(lStart);
    cgLabel(lEnd);
    return NOREG;
}

// Given an AST, the register (if any) that holds
// the previous rvalue, and the AST op of the parent,
// generate assembly code recursively.
// Return the register id with the tree's final value
int genAST(struct ASTNode *node, int reg_, int parentASTop_) {
    int leftreg, rightreg;

    // We not have specific AST node handling at the top
    switch (node->op_) {
        case A_IF:
            return genIfAST(node);
        case A_WHILE:
            return genWHILE(node);
        case A_GLUE:
            // Do each child statement, and free the registers after each child
            genAST(node->left_, NOREG, node->op_);
            genFreeRegs();
            genAST(node->right_, NOREG, node->op_);
            genFreeRegs();
            return NOREG;

        case A_FUNCTION:
            // Generate the function's preamble before the code
            cgFuncPreamble(node->v_.id_);
            genAST(node->left_, NOREG, node->op_);
            cgFuncPostamble(node->v_.id_);
            return NOREG;
    }

    // General AST node handing below

    // Get the left and right sub-tree values
    if (node->left_)
        leftreg = genAST(node->left_, NOREG, -1);
    if (node->right_)
        rightreg = genAST(node->right_, leftreg, node->op_);

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
        case A_NE:
        case A_LT:
        case A_GT:
        case A_LE:
        case A_GE:
            // If the parent AST node is an A_IF, generate a compare
            // followed by a jump. Otherwise, compare registers and
            // set one to 1 or 0 based on the comparison.
            if (parentASTop_ == A_IF)
                return cgCompareAndJump(node->op_, leftreg, rightreg, reg_);
            else
                return cgCompareAndSet(node->op_, leftreg, rightreg);

        case A_INTLIT:
            return cgLoadInt(node->v_.int_value_, node->type_);
        case A_IDENT:
            return cgLoadGlobal(node->v_.id_);
        case A_LVIDENT:
            return cgStorGlob(reg_, node->v_.id_);
        case A_ASSIGN:
            // The work has aleady been done, return the result
            return rightreg;
        case A_PRINT:
            // Print the left-child's value
            // and return no register
            genPrintInt(leftreg);
            genFreeRegs();
            return NOREG;
        case A_WIDEN:
            // Widen the child's type to the parent's type
            return cgWiden(leftreg, node->left_->type_, node->type_);
        case A_RETURN:
            cgReturn(leftreg, FunctionId_);
            return NOREG;
        case A_FUNCCALL:
            return cgCall(leftreg, node->v_.id_);
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

void genGlobalSymbols(int id_) {
    cgGlobSym(id_);
}

int genPrimSize(int type_) {
    return cgPrimSize(type_);
}