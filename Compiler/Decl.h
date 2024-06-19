//
// Created by jewoo on 2024-06-14.
//

#pragma once

int scan(struct Token *t_);

// Tree.c
struct ASTNode *makeASTNode(int op_,
                            struct ASTNode *left,
                            struct ASTNode *mid_,
                            struct ASTNode *right_,
                            int int_value_);

struct ASTNode *makeASTLeaf(int op_, int int_value_);

struct ASTNode *makeASTUnary(int op_,
                             struct ASTNode *left_,
                             int int_value_);

// Gen.c
int genAST(struct ASTNode *node_, int reg_, int parentASTop_);

void genPreamble();

void genPostamble();

void genFreeRegs();

void genPrintInt(int reg_);

void genGlobalSymbols(char *s_);

// Cg.c
void freeAllRegisters();

void cgPreamble();

void cgFuncPreamble(char *name_);

void cgFuncPostamble();

int cgLoadInt(int value_);

int cgLoadGlobal(char *identifier_);

int cgAdd(int r1_, int r2_);

int cgSub(int r1_, int r2_);

int cgMul(int r1_, int r2_);

int cgDiv(int r1_, int r2_);

void cgPrintInt(int r_);

int cgStorGlob(int r_, char *identifier_);

void cgGlobSym(char *sym_);

int cgCompareAndSet(int ASTop_, int r1_, int r2_);

int cgCompareAndJump(int ASTop_, int r1_, int r2_, int label_);

void cgLabel(int label_);

void cgJump(int label_);

// Expr.c
struct ASTNode *binexpr(int ptp_);

// Stmt.c
struct ASTNode *compoundStatement();

// Misc.c
void match(int expected_, char *what_);

void semi();

void lBrace();

void rBrace();

void lParen();

void rParen();

void ident();

void fatal(char *s_);

void fatals(char *s1_, char *s2_);

void fatald(char *s_, int d_);

void fatalc(char *s_, int c_);

// Sym.c
int findGlob(char *s_);

int addGlob(char *name_);

// Decl.c
void varDeclaration();

struct ASTNode *functionDeclaration();