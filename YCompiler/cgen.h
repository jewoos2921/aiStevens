//
// Created by jewoo on 2024-06-26.
//

#ifndef AISTEVENS_CGEN_H
#define AISTEVENS_CGEN_H

// Procedure codeGen generate code to a code file by traversal of the syntax tree. The second
// parameter (codefile) is the file name of the code file, and is used to print the file name
// as a comment in the code file
void codeGen(TreeNode *syntaxTree, char *codeFile);

#endif //AISTEVENS_CGEN_H
