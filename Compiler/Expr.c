//
// Created by jewoo on 2024-06-14.
//
#include "Defs.h"
#include "Data.h"
#include "Decl.h"


// Parse a function call with a single expression argument and return its AST.
struct ASTNode *funcCall() {
    struct ASTNode *tree;
    int id;

    if ((id = findGlob(Text_)) == -1) {
        fatals("undeclared function", Text_);
    }

    // Get the '('
    lParen();

    // Parse the following expression
    tree = binexpr(0);

    tree = makeASTUnary(A_FUNCCALL, Gsym_[id].type_, tree, id);

    // Ensure we have a ')'
    rParen();
    return tree;
}

// Parse a primary factor and return an AST node representing it.
static struct ASTNode *primary() {
    struct ASTNode *node = NULL;
    int id;

    switch (Token_.token_) {
        case T_INTLIT:
            if ((Token_.int_value_) >= 0 && (Token_.int_value_ < 256))
                node = makeASTLeaf(A_INTLIT, P_CHAR, Token_.int_value_);
            else
                node = makeASTLeaf(A_INTLIT, P_INT, Token_.int_value_);
            break;

        case T_IDENT:

            // This could be a variable or a function call.
            // Scan in the next token to find out
            scan(&Token_);

            // It's a '(' so a function call
            if (Token_.token_ == T_LPAREN)
                return funcCall();

            // Not a function call, so reject the new token
            rejectToken(&Token_);

            // Check that this identifier exists
            id = findGlob(Text_);
            if (id == -1)
                fatals("undeclared variable", Text_);

            // Make a leaf AST node for it
            node = makeASTLeaf(A_IDENT, Gsym_[id].type_, id);
            break;

        default:
            fatald("Syntax error, token", Token_.token_);
    }

    // Scan in the next token and return the leaf node
    scan(&Token_);
    return node;
}

// convert a binary operator token into an AST operation
static int arithop(int tokentype_) {
    if (tokentype_ > T_EOF && tokentype_ < T_INTLIT)
        return tokentype_;
    fatald("Syntax error, token", tokentype_);
}

// Operator precedence for each token.
static int OpPrec_[] = {
        0, 10, 10,                                  // T_EOF, T_PLUS, T_MINUS
        20, 20,                                         // T_STAR, T_SLASH
        30, 30,                                         // T_EQ, T_NE
        40, 40, 40, 40                         // T_LT, T_GT, T_LE, T_GE
};

// Check that we have a binary operator where we need to.
// Return its precedence.
static int op_precedence(int tokentype_) {
    int prec = OpPrec_[tokentype_];
    if (prec == 0) {
        fatald("Syntax error, token", tokentype_);
    }
    return prec;
}

// return an AST tree whose root is a binary operator
// Paramete ptp is the previous token's precedence.
struct ASTNode *binexpr(int ptp_) {
    struct ASTNode *left, *right;
    int leftType, rightType;
    int tokentype;

    // get the primary tree on the left.
    // Fetch the next token at the same time.
    left = primary();

    // If we hit a semicolon or ')', return just the left node
    tokentype = Token_.token_;
    if (tokentype == T_SEMI || tokentype == T_RPAREN)
        return left;


    // While the precedence of this token is
    // more than that of the previous token precedence.
    while (op_precedence(tokentype) > ptp_) {

        // Fetch in the next integer literal
        scan(&Token_);

        // Recursively cll binexpr() with the
        // precedence of our token to build a sub-tree
        right = binexpr(OpPrec_[tokentype]);

        // Ensure the two types are compatible
        leftType = left->type_;
        rightType = right->type_;
        if (!typeCompatible(&leftType, &rightType, 0))
            fatal("Incompatible types");


        // Widen either side if required. type vars are A_WIDEN now
        if (leftType)
            left = makeASTUnary(leftType, right->type_, left, 0);
        if (rightType)
            right = makeASTUnary(rightType, left->type_, right, 0);

        // Join that sub-tree with ours. Convert the token into an AST operation at the same time.
        left = makeASTNode(arithop(tokentype), left->type_, left, NULL, right, 0);

        // Update the details of the current token
        // If we hit a semicolon or ')', return just the left node.
        tokentype = Token_.token_;
        if (tokentype == T_SEMI || tokentype == T_RPAREN)
            return left;
    }

    // Return the tree we have when the precedence
    // is the same or lower.
    return left;
}

