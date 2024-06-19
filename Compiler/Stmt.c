//
// Created by jewoo on 2024-06-14.
//

#include "Defs.h"
#include "Data.h"
#include "Decl.h"

// prototypes
static struct ASTNode *singleStatement();

// compound_statement:          // empty, i.e. no statement
//      |      statement
//      |      statement statements
//      ;
//
// statement: print_statement
//      |     declaration
//      |     assignment_statement
//      |     if_statement
//      |     while_statement
//      ;

// print_statement: 'print' expression ';'  ;
static struct ASTNode *printStatement() {
    struct ASTNode *tree;

    match(T_PRINT, "print");

    // Parse the following expression
    tree = binexpr(0);
    // Make a print AST tree
    tree = makeASTUnary(A_PRINT, tree, 0);


    // Match the following semicolon
    semi();
    return tree;
}

// assignment_statement: identifier '=' expression ';'   ;
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

// if_statement: if_head
//      |        if_head 'else' compound_statement
//      ;
//
// if_head: 'if' '(' true_false_expression ')' compound_statement  ;
//
// Parse an IF statement including
// any optional ELSE clause
// and return its AST
static struct ASTNode *ifStatement() {
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

// while_statement: 'while' '(' true_false_expression ')' compound_statement  ;
//
// Parse a WHILE statement
// and return its AST
static struct ASTNode *whileStatement() {
    struct ASTNode *condAST, *bodyAST;

    // Ensure we have 'while' '('
    match(T_WHILE, "while");
    lParen();

    // Parse the following expression and the ')' following. Ensure the tree's operation is a comparison.
    condAST = binexpr(0);
    if (condAST->op_ < A_EQ || condAST->op_ > A_GE) {
        fatal("Bad comparison operator");
    }
    rParen();

    // Get the AST for the compound statement
    bodyAST = compoundStatement();

    return makeASTNode(A_WHILE, condAST, NULL, bodyAST, 0);
}

// for_statement: 'for' '(' preop_statement ';'
//                          true_false_expression ';'
//                          postop_statement ')' compound_statement  ;
//
// preop_statement:  statement          (for now)
// postop_statement: statement          (for now)
//
// Parse a FOR statement
// and return its AST
static struct ASTNode *forStatement() {
    struct ASTNode *condAST, *bodyAST;
    struct ASTNode *preopAST, *postopAST;
    struct ASTNode *tree;

    // Ensure we have 'for' '('
    match(T_FOR, "for");
    lParen();

    // Get the pre_op statement and the ';'
    preopAST = singleStatement();
    semi();

    // Get the condition and the ';'
    condAST = binexpr(0);
    if (condAST->op_ < A_EQ || condAST->op_ > A_GE) {
        fatal("Bad comparison operator");
    }
    semi();

    // Get the post_op statement and the ')'
    postopAST = singleStatement();
    rParen();

    // Get the AST for the compound statement
    bodyAST = compoundStatement();


    // For now, all four sub-trees have to be non-NULL.
    // Later on, we'll change the semantics for when some are missing

    // Glue the compound statement and the postop tree
    tree = makeASTNode(A_GLUE, bodyAST, NULL, postopAST, 0);

    // Make a WHILE loop with the condition and this new body
    tree = makeASTNode(A_WHILE, condAST, NULL, tree, 0);

    return makeASTNode(A_GLUE, preopAST, NULL, tree, 0);
}

// Parse a single statement and return its AST
static struct ASTNode *singleStatement() {
    switch (Token_.token_) {
        case T_PRINT :
            return printStatement();
        case T_INT:
            varDeclaration();
            return NULL;
        case T_IDENT:
            return assignmentStatement();
        case T_IF:
            return ifStatement();
        case T_WHILE:
            return whileStatement();
        case T_FOR:
            return forStatement();
        default:
            fatald("Syntax error, token", Token_.token_);
    }
}

// Parse a compound statement and return its AST
struct ASTNode *compoundStatement() {
    struct ASTNode *left = NULL;
    struct ASTNode *tree;

    // Require a left curly brace
    lBrace();

    while (1) {
        tree = singleStatement();

        // Some statements must be followed by a semicolon
        if (tree != NULL && (tree->op_ == A_PRINT || tree->op_ == A_ASSIGN))
            semi();


        // For each new tree, either save it in left
        // if left is empty, or glue the left and the
        // new tree together
        if (tree != NULL) {
            if (left == NULL)
                left = tree;
            else
                left = makeASTNode(A_GLUE, left, NULL, tree, 0);
        }
        // When we hit a right curly bracket,
        // skip past it and return the AST
        if (Token_.token_ == T_RBRACE) {
            rBrace();
            return left;
        }
    }
}