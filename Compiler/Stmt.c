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
//      |     function_call
//      |     if_statement
//      |     while_statement
//      |     for_statement
//      |     return_statement
//      ;


// print_statement: 'print' expression ';'  ;
static struct ASTNode *printStatement() {
    struct ASTNode *tree;
    int leftType, rightType;


    match(T_PRINT, "print");

    // Parse the following expression
    tree = binexpr(0);

    // Ensure the two types are compatible
    leftType = P_INT;
    rightType = tree->type_;
    if (!typeCompatible(&leftType, &rightType, 0)) {
        fatal("Incompatible types");
    }

    if (rightType)
        tree = makeASTUnary(rightType, P_INT, tree, 0);

    // Make a print AST tree
    tree = makeASTUnary(A_PRINT, P_NONE, tree, 0);


    // Return the AST.
    return tree;
}

// assignment_statement: identifier '=' expression ';'   ;
//
static struct ASTNode *assignmentStatement() {
    struct ASTNode *left, *right, *tree;
    int id;
    int leftType, rightType;

    // Ensure we have an identifier
    ident();

    // This could be a variable or a function call.
    // If next token is '(', it's a function call
    if (Token_.token_ == T_LPAREN)
        return funcCall();

    // Check it's been defined then make a leaf node for it
    if ((id = findGlob(Text_)) == -1) {
        fatals("Undeclared varaibles", Text_);
    }

    right = makeASTLeaf(A_LVIDENT, Gsym_[id].type_, id);

    // Ensure we have an equals sign
    match(T_ASSIGN, "=");

    // Parse the following expression
    left = binexpr(0);

    // Ensure the two types are compatible
    leftType = left->type_;
    rightType = right->type_;
    if (!typeCompatible(&leftType, &rightType, 1)) {
        fatal("Incompatible types");
    }

    // Widen the left if required
    if (leftType)
        left = makeASTUnary(leftType, right->type_, left, 0);

    // Make an assignment AST Tree
    tree = makeASTNode(A_ASSIGN, P_INT, left, NULL, right, 0);

    // Return the AST.
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
    return makeASTNode(A_IF, P_NONE, condAST, trueAST, falseAST, 0);
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

    return makeASTNode(A_WHILE, P_NONE, condAST, NULL, bodyAST, 0);
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
    tree = makeASTNode(A_GLUE, P_NONE, bodyAST, NULL, postopAST, 0);

    // Make a WHILE loop with the condition and this new body
    tree = makeASTNode(A_WHILE, P_NONE, condAST, NULL, tree, 0);

    return makeASTNode(A_GLUE, P_NONE, preopAST, NULL, tree, 0);
}

// return_statement: 'return' '(' expression ')'  ;
//
// Parse a return statement and return its AST
static struct ASTNode *returnStatement() {
    struct ASTNode *tree;
    int returnType, funcType;

    // Can't return a value if function returns P_VOIDS
    if (Gsym_[FunctionId_].type_ == P_VOID)
        fatal("Can't return from a void function");

    // Ensure we have 'return' '('
    match(T_RETURN, "return");
    lParen();

    // Parse the following expression
    tree = binexpr(0);

    // ensure this is compatible with the function's return type
    returnType = tree->type_;
    funcType = Gsym_[FunctionId_].type_;
    if (!typeCompatible(&funcType, &returnType, 1)) {
        fatal("Incompatible types");
    }

    // Widen the left if required
    if (returnType)
        tree = makeASTUnary(returnType, funcType, tree, 0);

    // Make a return AST tree
    tree = makeASTUnary(A_RETURN, P_NONE, tree, 0);

    // Get the ')'
    rParen();
    return tree;
}

// singleStatement: Parse a single statement and return its AST
// Parse a single statement and return its AST
static struct ASTNode *singleStatement() {
    switch (Token_.token_) {
        case T_PRINT :
            return printStatement();

        case T_CHAR:
        case T_INT:
        case T_LONG:
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
        case T_RETURN:
            return returnStatement();
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
        if (tree != NULL && (tree->op_ == A_PRINT || tree->op_ == A_ASSIGN ||
                             tree->op_ == A_RETURN || tree->op_ == A_FUNCCALL))
            semi();


        // For each new tree, either save it in left
        // if left is empty, or glue the left and the
        // new tree together
        if (tree != NULL) {
            if (left == NULL)
                left = tree;
            else
                left = makeASTNode(A_GLUE, P_NONE, left, NULL, tree, 0);
        }
        // When we hit a right curly bracket,
        // skip past it and return the AST
        if (Token_.token_ == T_RBRACE) {
            rBrace();
            return left;
        }
    }
}