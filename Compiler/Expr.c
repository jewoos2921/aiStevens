//
// Created by jewoo on 2024-06-14.
//
#include "Defs.h"
#include "Data.h"
#include "Decl.h"


// Parse a primary factor and return an AST node representing it.
static struct ASTNode *primary() {
    struct ASTNode *node = NULL;
    int id;

    switch (Token_.token_) {
        case T_INTLIT:
            node = makeASTLeaf(A_INTLIT, Token_.int_value_);
            break;

        case T_IDENT:
            id = findGlob(Text_);
            if (id == -1)
                fatals("undeclared variable", Text_);

            node = makeASTLeaf(A_IDENT, id);
            break;

        default:
            fatald("Syntax error, token", Token_.token_);
    }
    scan(&Token_);
    return node;
}

// convert a binary operator token into an AST operation
static int arithop(int tokentype) {
    if (tokentype > T_EOF && tokentype < T_INTLIT)
        return tokentype;
    fatald("Syntax error, token", tokentype);
}

// Operator precedence for each token.
static int OpPrec[] = {
        0, 10, 10, // T_EOF, T_PLUS, T_MINUS
        20, 20,         // T_STAR, T_SLASH
        30, 30,             // T_EQ, T_NE
        40, 40, 40, 40 // T_LT, T_GT, T_LE, T_GE
};

// Check that we have a binary operator where we need to.
// Return its precedence.
static int op_precedence(int tokentype) {
    int prec = OpPrec[tokentype];
    if (prec == 0) {
        fatald("Syntax error, token", tokentype);
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
    if (token_type == T_SEMI || token_type == T_RPAREN)
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
        left = makeASTNode(arithop(token_type), left, NULL, right, 0);

        // Update the token type
        token_type = Token_.token_;
        if (token_type == T_EOF || token_type == T_RPAREN)
            return left;
    }

    // Return the tree we have when the precedence
    // is the same or lower.
    return left;
}

