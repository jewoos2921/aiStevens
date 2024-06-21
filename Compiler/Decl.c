//
// Created by jewoo on 2024-06-16.
//

#include "Defs.h"
#include "Data.h"
#include "Decl.h"

// Parsing of declarations

// Parse the current token and return a primitive type enum value
static int parseType(int t_) {
    if (t_ == T_CHAR) return (P_CHAR);
    if (t_ == T_INT) return (P_INT);
    if (t_ == T_VOID) return (P_VOID);
    fatald("Illegal type, token", t_);
}

// declaration: 'int' identifier ';'  ;
//
// Parse the declaration of a variable
void varDeclaration() {

    int type, id;
    // Get the type of the variable, then the identifier

    type = parseType(Token_.token_);
    scan(&Token_);
    ident();
    // Text now has the identifier's name
    // Add it as a known identifier and generate its space in assembly
    id = addGlob(Text_, type, S_VARIABLE);
    genGlobalSymbols(id);
    // Get the trailing semicolon
    semi();
}

// For now we have a very simplistic function definition grammar
//
// function_declaration: 'void' identifier '(' ')' compound_statement   ;
//
// Parse the declaration of a simplistic function
struct ASTNode *functionDeclaration() {
    struct ASTNode *tree;
    int nameSlot;

    // Find the 'void', identifier, and the '(' ')'.
    // For now, do nothing with them
    match(T_VOID, "void");
    ident();
    nameSlot = addGlob(Text_, P_VOID, S_FUNCTION);
    lParen();
    lParen();

    // Get the AST tree for the compound statement
    tree = compoundStatement();

    // Return an A_FUNCTION node which has the function's nameslot and the compound statement sub-tree
    return makeASTUnary(A_FUNCTION, P_VOID, tree, nameSlot);
}