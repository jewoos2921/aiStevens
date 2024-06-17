//
// Created by jewoo on 2024-06-16.
//

#include "Defs.h"
#include "Data.h"
#include "Decl.h"

// Parsing of declarations

// Parse the declaration of a variable
void varDeclaration() {

    match(T_INT, "int");
    ident();
    addGlob(Text_);
    genGlobalSymbols(Text_);
    semi();
}