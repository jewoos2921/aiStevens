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
        fatals("Expected", what_);
    }
}

void semi() {
    match(T_SEMI, ";");
}


void ident() {
    match(T_IDENT, "identifier");
}

void fatal(char *s_) {
    fprintf(stderr, "%s on line %d\n", s_, Line_);
    exit(1);
}

void fatals(char *s1_, char *s2_) {
    fprintf(stderr, "%s:%s on line %d\n", s1_, s2_, Line_);
    exit(1);
}

void fatald(char *s_, int d_) {
    fprintf(stderr, "%s%d on line %d\n", s_, d_, Line_);
    exit(1);
}

void fatalc(char *s_, int c_) {
    fprintf(stderr, "%s:%c on line %d\n", s_, c_, Line_);
    exit(1);
}