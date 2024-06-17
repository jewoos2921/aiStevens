//
// Created by jewoo on 2024-06-14.
//

#include "Defs.h"
#include "Data.h"
#include "Decl.h"

// statements: statement
//      |      statement statements
//      ;
//
// statement: 'print' expression ';'
//      |     'int'   identifier ';'
//      |     identifier '=' expression ';'
//      ;
//
// identifier: T_IDENT
//      ;

static void printStatement() {
    struct ASTNode *tree;
    int reg;

    match(T_PRINT, "print");

    // Parse the following expression and
    // generate the assembly code
    tree = binexpr(0);
    reg = genAST(tree, -1);
    genPrintInt(reg);
    genFreeRegs();

    // Match the following semicolon
    semi();
}

static void assignmentStatement() {
    struct ASTNode *left, *right, *tree;
    int id;

    // Ensure we have an identifier
    ident();

    // Check it's been defined then make a leaf node for it
    if ((id = findGlob(Text_)) == -1) {
        fatals("Undeclared varaibles", Text_);
    }
    right = makeASTLeaf(A_LVIDENT, id);

    // Ensure we have an equals sign
    match(T_EQUALS, "=");

    // Parse the following expression
    left = binexpr(0);

    // Make an assignment AST Tree
    tree = makeASTNode(A_ASSIGN, left, right, 0);

    // Generate the assembly code
    genAST(tree, -1);
    genFreeRegs();

    // Match the following semicolon
    semi();
}

void statements() {

    while (1) {
        switch (Token_.token_) {
            case T_PRINT:
                printStatement();
                break;
            case T_INT:
                varDeclaration();
                break;
            case T_IDENT:
                assignmentStatement();
                break;
            case T_EOF:
                return;
            default:
                fatald("Syntax error, token", Token_.token_);
        }
    }
}