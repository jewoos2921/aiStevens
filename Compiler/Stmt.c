//
// Created by jewoo on 2024-06-14.
//

#include "Defs.h"
#include "Data.h"
#include "Decl.h"

// compound_statement:          // empty, i.e. no statement
//      |      statement
//      |      statement statements
//      ;
//
// statement: print_statement
//      |     declaration
//      |     assignment_statement
//      |     if_statement
//      ;
//
// print_statement: 'print' expression ';'  ;
//
// declaration: 'int' identifier ';'  ;
//
// assignment_statement: identifier '=' expression ';'   ;
//
// if_statement: if_head
//      |        if_head 'else' compound_statement
//      ;
//
// if_head: 'if' '(' true_false_expression ')' compound_statement  ;
//
// identifier: T_IDENT ;

static struct ASTNode *printStatement() {
    struct ASTNode *tree;
    int reg;

    match(T_PRINT, "print");

    // Parse the following expression
    tree = binexpr(0);
    // Make a print AST tree
    tree = makeASTUnary(A_PRINT, tree, 0);


    // Match the following semicolon
    semi();
    return tree;
}

static struct ASTNode *assignmentStatement() {
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
    match(T_ASSIGN, "=");

    // Parse the following expression
    left = binexpr(0);

    // Make an assignment AST Tree
    tree = makeASTNode(A_ASSIGN, left, NULL, right, 0);

    // Match the following semicolon
    semi();
    return tree;
}

struct ASTNode *ifStatement() {
    struct ASTNode *condAST, *trueAST, *falseAST = NULL;

    // Ensure we have 'if' '('
    match(T_IF, "if");
    lParen();


    // Parse the following expression and the ')' following. Ensure the tree's operation is a comparison.
    condAST = binexpr(0);
    if (condAST->op_ < A_EQ || condAST->op_ > A_GE) {
        fatal("Bad comparison operator");
    }
    rParen();

    // Get the AST for the compound statement
    trueAST = compoundStatement();

    // If we have an 'else' clause, skip it and get the AST for the compound statement
    if (Token_.token_ == T_ELSE) {
        scan(&Token_);
        falseAST = compoundStatement();
    }

    // Build and return the AST for this statement
    return makeASTNode(A_IF, condAST, trueAST, falseAST, 0);
}

// Parse a compound statement and return its AST
struct ASTNode *compoundStatement() {
    struct ASTNode *left = NULL;
    struct ASTNode *tree;

    // Require a left curly brace
    lBrace();

    while (1) {
        switch (Token_.token_) {
            case T_PRINT:
                tree = printStatement();
                break;
            case T_INT:
                varDeclaration();
                tree = NULL;
                break;
            case T_IDENT:
                tree = assignmentStatement();
                break;
            case T_IF:
                tree = ifStatement();
                break;
            case T_RBRACE:
                // When we hit a right curly brace, skip past it and retrun the AST
                rBrace();
                return left;

            default:
                fatald("Syntax error, token", Token_.token_);
        }

        // For each new tree, either save it in left
        // if left is empty, or glue the left and the
        // new tree together
        if (tree) {
            if (left == NULL)
                left = tree;
            else
                left = makeASTNode(A_GLUE, left, NULL, tree, 0);
        }
    }
}