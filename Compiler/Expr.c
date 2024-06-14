//
// Created by jewoo on 2024-06-14.
//
#include "Defs.h"
#include "Data.h"
#include "Decl.h"


static struct ASTNode *primary() {
    struct ASTNode *n;

    switch (Token_.token_) {
        case T_INTLIT:
            n = makeASTLeaf(A_INTLIT, Token_.int_value_);
            scan(&Token_);
            return n;

        default:
            fprintf(stderr, "syntax error on line %d\n", Line_);
            exit(1);
    }
}

// converta token into ana AST operation
int arithop(int tok) {
    switch (tok) {
        case T_PLUS:
            return A_ADD;
        case T_MINUS:
            return A_SUBTRACT;
        case T_STAR:
            return A_MULTIPLY;
        case T_SLASH:
            return A_DIVIDE;
        default:
            fprintf(stderr, "syntax error on line %d\n", Line_);
            exit(1);
    }
}

// return an AST tree whose root is a binary operator
struct ASTNode *binexpr() {
    struct ASTNode *n, *left, *right;
    int node_type;

    // get the left hand side
    left = primary();

    // If no tokens left, return just the left node
    if (Token_.token_ == T_EOF)
        return left;

    // convert the token into a node type
    node_type = arithop(Token_.token_);

    // Get the next token in
    scan(&Token_);

    // Recursively call binexpr to get the right hand side
    right = binexpr();

    // not build a tree with both sub-trees
    n = makeASTNode(node_type, left, right, 0);

    return n;
}

