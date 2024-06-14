//
// Created by jewoo on 2024-06-14.
//
#include "Defs.h"
#include "Data.h"
#include "Decl.h"


static struct ASTNode *primary() {
    struct ASTNode *node;

    switch (Token_.token_) {
        case T_INTLIT:
            node = makeASTLeaf(A_INTLIT, Token_.int_value_);
            scan(&Token_);
            return node;

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

// Operator precedence for each token.
static int OpPrec[] = {0, 10, 10, 20, 20, 0};

// Check that we have a binary operator where we need to.
// Return its precedence.
static int op_precedence(int tokentype) {
    int prec = OpPrec[tokentype];
    if (prec == 0) {
        fprintf(stderr, "syntax error, token %d\n", tokentype);
        exit(1);
    }
    return prec;
}

// return an AST tree whose root is a binary operator
// Paramete ptp is the previous token's precedence.
struct ASTNode *binexpr(int ptp) {
    struct ASTNode *left, *right;
    int token_type;

    // get the left hand side
    left = primary();

    // If no tokens left, return just the left node
    token_type = Token_.token_;
    if (token_type == T_EOF)
        return left;
    // While the precedence of this token is
    // more than that of the previous token precedence.
    while (op_precedence(token_type) > ptp) {

        // Fetch in the next integer literal
        scan(&Token_);

        // Recursively cll binexpr() with the
        // precedence of our token to build a sub-tree
        right = binexpr(OpPrec[token_type]);

        // Make the AST node for the binary operator.
        left = makeASTNode(arithop(token_type), left, right, 0);

        // Update the token type
        token_type = Token_.token_;
        if (token_type == T_EOF)
            return left;
    }

    // Return the tree we have when the precedence
    // is the same or lower.
    return left;
}

