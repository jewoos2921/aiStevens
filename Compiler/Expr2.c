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

// convert a binary operator token into an AST operation
int arithop(int tokentype) {
    switch (tokentype) {
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

struct ASTNode *additiveExpr();

struct ASTNode *multiplicativeExpr() {
    struct ASTNode *left, *right;
    int token_type;

    // get the integer literal on the left
    // Fetch the next token at the same time.
    left = primary();


    // If no tokens left, return just the left node
    token_type = Token_.token_;
    if (token_type == T_EOF)
        return left;

    // while the token is a '*' or '/'
    while (token_type == T_STAR || token_type == T_SLASH) {

        // Fetch in the next integer literal
        scan(&Token_);
        right = primary();

        // Make the AST node for the binary operator.
        left = makeASTNode(arithop(token_type), left, right, 0);

        // Update the token type
        token_type = Token_.token_;
        if (token_type == T_EOF)
            return left;
    }


    // Return whaterverr tree we have created.
    return left;
}

struct ASTNode *additiveExpr() {
    struct ASTNode *left, *right;
    int token_type;

    // Get the left sub-tree at a higer precedence
    left = multiplicativeExpr();

    token_type = Token_.token_;
    if (token_type == T_EOF)
        return left;

    // Loop working on token at our level of precedence
    while (1) {
        // Fetch in the next integer literal
        scan(&Token_);

        // Get the right sub-tree at a higer precedence
        right = multiplicativeExpr();

        // Join the two sub-trees
        left = makeASTNode(arithop(token_type), left, right, 0);

        token_type = Token_.token_;
        if (token_type == T_EOF)
            break;
    }

    return left;
}

// return an AST tree whose root is a binary operator
// Paramete ptp is the previous token's precedence.
struct ASTNode *binexpr(int ptp) {
    return additiveExpr();
}

