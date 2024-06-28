//
// Created by jewoo on 2024-06-27.
//

#include "globals.h"
#include "symtab.h"
#include "code.h"
#include "cgen.h"


/* tmpOffset is the memory offset fo temps It is decremented each time a temp is
 * stored, and incremented when loaded again */
static int tmpOffset = 0;

/* prototype for internal recursive code generator */
static void cGen(TreeNode *tree);

/* Procedure genStmt generates code at a statement node */
static void genStmt(TreeNode *tree) {
    int savedLoc1, savedLoc2, currentLoc;
    TreeNode *p1, *p2, *p3;
    int loc;
    switch (tree->kind.stmt) {
        case IfK:
            if (TraceCode) emitComment("-> if");
            p1 = tree->children[0];
            p2 = tree->children[1];
            p3 = tree->children[2];
            /* generate code for test expression*/
            cGen(p1);
            savedLoc1 = emitSkip(1);
            emitComment("if: jump to else after condition");
            /* recurse on then part */
            cGen(p2);
            savedLoc2 = emitSkip(1);
            emitComment("if: jump to end belongs here");
            currentLoc = emitSkip(0);
            emitBackup(savedLoc1);
            emitRM_Abs("JEQ", ac, currentLoc, "jump to end");
            emitRestore();
            /* recurse on the else part */
            cGen(p3);
            currentLoc = emitSkip(0);
            emitBackup(savedLoc2);
            emitRM_Abs("LDA", pc, currentLoc, "jump to end");
            emitRestore();
            if (TraceCode) emitComment("<- if");
            break;

        case RepeatK:
            if (TraceCode) emitComment("-> repeat");
            p1 = tree->children[0];
            p2 = tree->children[1];
            savedLoc1 = emitSkip(0);
            emitComment("repeat: jump after body comes back here");
            /* generate code for body */
            cGen(p1);
            /* generate code for test */
            cGen(p2);
            emitRM_Abs("JEQ", ac, savedLoc1, "repeat: jmp back to body");
            if (TraceCode) emitComment("<- repeat");
            break;

        case AssignK:
            if (TraceCode) emitComment("-> assign");
            /* generate code for rhs */
            cGen(tree->children[0]);
            /* new store value */
            loc = st_lookup(tree->attr.name);
            emitRM("ST", ac, loc, gp, "assign: store value");
            if (TraceCode) emitComment("<- assign");
            break;

        case ReadK:
            emitRO("IN", ac, 0, 0, "read integer value");
            loc = st_lookup(tree->attr.name);
            emitRM("ST", ac, loc, gp, "read: store value");
            break;

        case WriteK:
            /* generate code for expression to write */
            cGen(tree->children[0]);
            /* now ouptut it */
            emitRO("OUT", ac, 0, 0, "write ac");
            break;
        default:
            break;
    }
}

/* Procedure genExp generates code at an expression node */
static void genExp(TreeNode *tree) {
    int loc;
    TreeNode *p1, *p2;
    switch (tree->kind.exp) {
        case ConstK:
            if (TraceCode) emitComment("-> Const");
            /* gen code to load integer constant using LDC */
            emitRM("LDC", ac, tree->attr.val, 0, "load const");
            if (TraceCode) emitComment("<- Const");
            break;

        case IdK:
            if (TraceCode) emitComment("-> Id");
            loc = st_lookup(tree->attr.name);
            emitRM("LD", ac, loc, gp, "load id value");
            if (TraceCode) emitComment("<- Id");
            break;

        case OpK:
            if (TraceCode) emitComment("-> Op");
            p1 = tree->children[0];
            p2 = tree->children[1];
            /* gen code for ac = left arg */
            cGen(p1);
            /* gen code to push left operand */
            emitRM("ST", ac, tmpOffset--, mp, "op: push left");
            /* gen code for ac = right operand */
            cGen(p2);
            emitRM("LD", ac1, ++tmpOffset, mp, "op: load left");
            switch (tree->attr.op) {
                case PLUS:
                    emitRO("ADD", ac, ac1, ac, "op +");
                    break;
                case MINUS:
                    emitRO("SUB", ac, ac1, ac, "op -");
                    break;
                case TIMES:
                    emitRO("MUL", ac, ac1, ac, "op *");
                    break;
                case OVER:
                    emitRO("DIV", ac, ac1, ac, "op /");
                    break;
                case LT:
                    emitRO("SUB", ac, ac1, ac, "op <");
                    emitRM("JLT", ac, 2, pc, "br if true");
                    emitRM("LDC", ac, 0, ac, "false case");
                    emitRM("LDA", pc, 1, pc, "unconditional jmp");
                    emitRM("LDC", ac, 1, ac, "true case");
                    break;
                case EQ:
                    emitRO("SUB", ac, ac1, ac, "op ==");
                    emitRM("JEQ", ac, 2, pc, "br if true");
                    emitRM("LDC", ac, 0, ac, "false case");
                    emitRM("LDA", pc, 1, pc, "unconditional jmp");
                    emitRM("LDC", ac, 1, ac, "true case");
                    break;
                default:
                    emitComment("BUG: Unknown operator");
                    break;
            }
            if (TraceCode) emitComment("<- Op");
            break;
        default:
            break;
    }
}

/* prototype for internal recursive code generator */
static void cGen(TreeNode *tree) {
    if (tree != NULL) {
        switch (tree->nodekind) {
            case StmtK:
                genStmt(tree);
                break;
            case ExpK:
                genExp(tree);
                break;
            default:
                break;
        }
        cGen(tree->sibling);
    }
}

// Procedure codeGen generate code to a code file by traversal of the syntax tree. The second
// parameter (codefile) is the file name of the code file, and is used to print the file name
// as a comment in the code file
void codeGen(TreeNode *syntaxTree, char *codeFile) {
    char *s = malloc(strlen(codeFile) + 7);
    strcpy(s, "File: ");
    strcat(s, codeFile);
    emitComment("TINY Compilation to TM code");
    emitComment(s);
    /* generate standard prelude */
    emitComment("Standard prelude:");
    emitRM("LD", mp, 0, ac, "load maxaddress from location 0");
    emitRM("ST", ac, 0, ac, "clear location 0");
    emitComment("End of standard prelude.");
    /* Generate code for TINY program */
    cGen(syntaxTree);
    /* finish */
    emitComment("End of execution.");
    emitRO("HALT", 0, 0, 0, "");
}