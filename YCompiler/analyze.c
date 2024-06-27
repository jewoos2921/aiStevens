//
// Created by jewoo on 2024-06-27.
//

#include "globals.h"
#include "symtab.h"
#include "analyze.h"

// counter for variable memory locations
static int location = 0;

static void traverse(TreeNode *t, void (*preProc)(TreeNode *), void (*postProc)(TreeNode *)) {
    if (t != NULL) {
        preProc(t);
        {
            int i;
            for (i = 0; i < MAXCHILDREN; i++)
                traverse(t->children[i], preProc, postProc);
        }
        postProc(t);
        traverse(t->sibling, preProc, postProc);
    }
}

static void nullProc(TreeNode *t) {
    if (t == NULL) return;
    else return;
}

static void insertNode(TreeNode *t) {
    switch (t->nodekind) {
        case StmtK:
            switch (t->kind.stmt) {
                case AssignK:
                case ReadK:
                    // not yet in table, so treate as new definition
                    if (st_lookup(t->attr.name) == -1)
                        st_insert(t->attr.name, t->lineno, location++);
                    else
                        /* already in table, so ignore location, add line number of use only */
                        st_insert(t->attr.name, t->lineno, 0);
                    break;
                default:
                    break;
            }

        case ExpK:
            switch (t->kind.exp) {
                case IdK:
                    // Not yet in table, so treat as new definition
                    if (st_lookup(t->attr.name) == -1)
                        st_insert(t->attr.name, t->lineno, location++);
                    else
                        /* already in table, so ignore location, add line number of use only */
                        st_insert(t->attr.name, t->lineno, 0);
                    break;
                default:
                    break;
            }
        default:
            break;
    }
}

/* Function buildSymtab constructs the symbol
 * table by preorder traversal of the syntax tree
 */
void buildSymtab(TreeNode *syntaxTree) {
    traverse(syntaxTree, insertNode, nullProc);
    if (TraceAnalyze) {
        fprintf(listing, "<symbol table>\n");
        printSymTab(listing);
    }
}


static void typeError(TreeNode *t, char *message) {
    fprintf(listing, "Type error at line %d: %s\n", t->lineno, message);
    Error = TRUE;
}

// Procedure checkNode performs type checking on a single tree node
static void checkNode(TreeNode *t) {
    switch (t->nodekind) {
        case ExpK:
            switch (t->kind.exp) {
                case OpK:
                    if (t->children[0]->type != Integer || t->children[1]->type != Integer)
                        typeError(t, "Op applied to non-integer");
                    if ((t->attr.op == EQ) || (t->attr.op == LT))
                        t->type = Boolean;
                    else
                        t->type = Integer;
                    break;
                case ConstK:
                    t->type = Integer;
                    break;
                default:
                    break;
            }
            break;
        case StmtK:
            switch (t->kind.stmt) {
                case IfK:
                    if (t->children[0]->type == Integer)
                        typeError(t->children[0], "if test is not Boolean");
                    break;
                case AssignK:
                    if (t->children[0]->type != Integer)
                        typeError(t->children[0], "assignment of non-integer value");
                    break;
                case WriteK:
                    if (t->children[0]->type != Integer)
                        typeError(t->children[0], "write of non-integer value");
                    break;
                case RepeatK:
                    if (t->children[0]->type == Integer)
                        typeError(t->children[0], "repeat test is not Boolean");
                    break;
                default:
                    break;
            }
            break;
        default:
            break;
    }
}

/* Procedure typeCheck performs type checking
 * by a postorder traversal of the syntax tree
 */
void typeCheck(TreeNode *syntaxTree) {
    traverse(syntaxTree, nullProc, checkNode);
}