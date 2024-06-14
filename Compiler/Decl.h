//
// Created by jewoo on 2024-06-14.
//

#pragma once

int scan(struct Token *t_);

struct ASTNode *makeASTNode(int op_,
                            struct ASTNode *left,
                            struct ASTNode *right_,
                            int int_value_);

struct ASTNode *makeASTLeaf(int op_, int int_value_);

struct ASTNode *makeASTUnary(int op_,
                             struct ASTNode *left_,
                             int int_value_);

struct ASTNode *binexpr(int rbp_);

int interpretAST(struct ASTNode *node_);