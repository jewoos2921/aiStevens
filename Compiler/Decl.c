//
// Created by jewoo on 2024-06-16.
//

#include "Defs.h"
#include "Data.h"
#include "Decl.h"

// Parsing of declarations

// declaration: 'int' identifier ';'  ;
//
// Parse the declaration of a variable
void varDeclaration() {

    match(T_INT, "int");
    ident();
    addGlob(Text_);
    genGlobalSymbols(Text_);
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
    nameSlot = addGlob(Text_);
    lParen();
    lParen();

    // Get the AST tree for the compound statement
    tree = compoundStatement();

    // Return an A_FUNCTION node which has the function's nameslot and the compound statement sub-tree
    return makeASTUnary(A_FUNCTION, tree, nameSlot);
}