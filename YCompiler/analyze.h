//
// Created by jewoo on 2024-06-26.
//

#ifndef AISTEVENS_ANALYZE_H
#define AISTEVENS_ANALYZE_H

// Function buildSymtab constructs the symbol table by preorder traversal of the syntax tree
void buildSymtab(TreeNode *);

// Procedure typeCheck performs type checking by a postorder syntax tree traversal
void typeCheck(TreeNode *);

#endif //AISTEVENS_ANALYZE_H
