//
// Created by jewoo on 2024-06-14.
//

#include "Defs.h"
#include "Data.h"
#include "Decl.h"


void match(int expected_, char *what_) {
    if (Token_.token_ == expected_) {
        scan(&Token_);
    } else {
        printf("ERROR: Unexpected token -> %s\n", what_);
        exit(1);
    }
}

void semi() {
    match(T_SEMI, ";");
}