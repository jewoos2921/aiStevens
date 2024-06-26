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
    if (t_ == T_LONG) return (P_LONG);
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
    id = addGlob(Text_, type, S_VARIABLE, 0);
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
    struct ASTNode *tree, *finalStmt;
    int nameSlot, type, endLabel;

    // Get the type of the variable, then the identifier
    type = parseType(Token_.token_);
    scan(&Token_);
    ident();


    // Get a label-id for the end label, add the function
    // to the symbol table, and set the Functionid global
    // to the function's symbol-id
    endLabel = genLabel();
    nameSlot = addGlob(Text_, type, P_VOID, S_FUNCTION);
    FunctionId_ = nameSlot;

    lParen();
    lParen();

    // Get the AST tree for the compound statement
    tree = compoundStatement();

    // If the function type isn't P_VOID, check that
    // the last AST operation in the compound statement
    // was a return statement
    if (type != P_VOID) {
        finalStmt = (tree->op_ == A_GLUE) ? tree->right_ : tree;
        if (finalStmt == NULL || finalStmt->op_ != A_RETURN) {
            fatal("No return statement for function");
        }
    }

    // Return an A_FUNCTION node which has the function's nameslot and the compound statement sub-tree
    return makeASTUnary(A_FUNCTION, P_VOID, tree, nameSlot);
}