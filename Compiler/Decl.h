//
// Created by jewoo on 2024-06-14.
//

#pragma once

// Scan.c
void rejectToken(struct Token *t_);

int scan(struct Token *t_);


// Tree.c
struct ASTNode *makeASTNode(int op_, int type_,
                            struct ASTNode *left,
                            struct ASTNode *mid_,
                            struct ASTNode *right_,
                            int int_value_);

struct ASTNode *makeASTLeaf(int op_, int type_, int int_value_);

struct ASTNode *makeASTUnary(int op_, int type_,
                             struct ASTNode *left_,
                             int int_value_);

// Gen.c
int genLabel();

int genAST(struct ASTNode *node_, int reg_, int parentASTop_);

void genPreamble();

void genPostamble();

void genFreeRegs();

void genPrintInt(int reg_);

void genGlobalSymbols(int id_);

int genPrimSize(int type_);

void genReturn(int reg_, int id_);

// Cg.c
void freeAllRegisters();

void cgPreamble();

void cgFuncPreamble(int id_);

void cgFuncPostamble(int id_);

int cgLoadInt(int value_, int type_);

int cgLoadGlobal(int id_);

int cgAdd(int r1_, int r2_);

int cgSub(int r1_, int r2_);

int cgMul(int r1_, int r2_);

int cgDiv(int r1_, int r2_);

void cgPrintInt(int r_);

int cgCall(int r_, int id_);

int cgStorGlob(int r_, int id_);

void cgGlobSym(int id_);

int cgCompareAndSet(int ASTop_, int r1_, int r2_);

int cgCompareAndJump(int ASTop_, int r1_, int r2_, int label_);

void cgLabel(int label_);

void cgJump(int label_);

int cgWiden(int r_, int oldType_, int newType_);

int cgPrimSize(int type_);

void cgReturn(int reg_, int id_);

// Expr.c

struct ASTNode *funcCall();

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

int addGlob(char *name_, int type_, int stype_, int endlabel_);

// Decl.c
void varDeclaration();

struct ASTNode *functionDeclaration();

// Types.c
int typeCompatible(int *left_, int *right_, int onlyRight_);