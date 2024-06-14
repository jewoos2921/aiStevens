//
// Created by jewoo on 2024-06-14.
//

#pragma once

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

// Tokens
enum {
    T_EOF,
    T_PLUS,
    T_MINUS,
    T_STAR,
    T_SLASH,
    T_INTLIT
};

struct Token {
    int token_;
    int int_value_;
};

// AST node type
enum {
    A_ADD,
    A_SUBTRACT,
    A_MULTIPLY,
    A_DIVIDE,
    A_INTLIT
};

// Abstract Syntax Tree structure
struct ASTNode {
    int op_; // operaiton to be performed on this tree
    struct ASTNode *left_;
    struct ASTNode *right_;
    int int_value_;
};
