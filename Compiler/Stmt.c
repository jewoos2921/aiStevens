//
// Created by jewoo on 2024-06-14.
//

#include "Defs.h"
#include "Data.h"
#include "Decl.h"


void statements() {
    struct ASTNode *tree;
    int reg;

    while (1) {
        match(T_PRINT, "print");

        // Parse the following expression and
        // generate the assembly code
        tree = binexpr(0);
        reg = genAST(tree);
        genPrintInt(reg);
        genFreeRegs();

        semi();
        if (Token_.token_ == T_EOF) {
            return;
        }
    }
}