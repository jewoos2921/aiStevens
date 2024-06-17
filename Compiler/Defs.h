//
// Created by jewoo on 2024-06-14.
//

#pragma once

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>


#define TEXTLEN         512                     // length of symbols in input
#define NSYMBOLS        1024                    // Number of symbol table entries


// Tokens
enum {
    T_EOF,
    T_PLUS,
    T_MINUS,
    T_STAR,
    T_SLASH,
    T_EQ, T_NE,
    T_LT, T_GT, T_LE, T_GE,
    T_INTLIT,
    T_SEMI,
    T_ASSIGN,
    T_IDENT,
    // Keywords
    T_PRINT, T_INT

};

struct Token {
    int token_;
    int int_value_;
};

// AST node type
enum {
    A_ADD = 1,
    A_SUBTRACT,
    A_MULTIPLY,
    A_DIVIDE,
    A_EQ, A_NE, A_LT, A_GT, A_LE, A_GE,
    A_INTLIT,
    A_IDENT,
    A_LVIDENT,
    A_ASSIGN,
};

// Abstract Syntax Tree structure
struct ASTNode {
    int op_; // operaiton to be performed on this tree
    struct ASTNode *left_;
    struct ASTNode *right_;
    union {
        int int_value_; // for A_INTLIT
        int id_; // for A_IDENT
    } v_;
};

struct symtable {
    char *name_;
};