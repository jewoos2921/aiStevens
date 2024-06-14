//
// Created by jewoo on 2024-06-14.
//

#pragma once

int scan(struct Token *t_);

// Tree.c
struct ASTNode *makeASTNode(int op_,
                            struct ASTNode *left,
                            struct ASTNode *right_,
                            int int_value_);

struct ASTNode *makeASTLeaf(int op_, int int_value_);

struct ASTNode *makeASTUnary(int op_,
                             struct ASTNode *left_,
                             int int_value_);

// Gen.c
int genAST(struct ASTNode *node_);

void genPreamble();

void genPostamble();

void genFreeRegs();

void genPrintInt(int reg_);

// Cg.c
void freeAllRegisters();

void cgPreamble();

void cgPostamble();

int cgLoad(int value_);

int cgAdd(int r1_, int r2_);

int cgSub(int r1_, int r2_);

int cgMul(int r1_, int r2_);

int cgDiv(int r1_, int r2_);

void cgPrintInt(int r_);

// Expr.c
struct ASTNode *binexpr(int rbp_);

// Stmt.c
void statements();

// Misc.c
void match(int expected_, char *what_);

void semi();