//
// Created by jewoo on 2024-06-26.
//

#ifndef AISTEVENS_UTIL_H
#define AISTEVENS_UTIL_H

// Procedure printToken prints a token and its lexeme to the listing file
void printToken(TokenType, const char *);

// Function newStmtNode creates a new statement node
TreeNode *newStmtNode(StmtKind);

// Function newExpNode creates a new expression node for syntax tree construction
TreeNode *newExpNode(ExpKind);

// Function copyString allocates  and makes a new copy of an existing string
char *copyString(char *);

// Procedure printTree prints a syntax tree to the listing file using indentation to indicate subtrees
void printTree(TreeNode *);

#endif //AISTEVENS_UTIL_H
