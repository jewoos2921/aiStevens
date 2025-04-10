//
// Created by jewoo on 2024-06-28.
//

#include "globals.h"
#include "code.h"

/* TM location number for current instruction emission */
static int emitLoc = 0;

/* Highest TM location emitted so far FOR use in conjunction with emitSkip ,
 * emit Backup, and emitRestore */
static int highEmitLoc = 0;

/* Procedure emitcomment prints a comment line with comment c in the code file */
void emitComment(char *comment) {
    if (TraceCode) fprintf(code, "* %s\n", comment);
}

/* Procedure emitRO emits a registerr-only TM instruction */
/* op = the opcode */
/* r = target register */
/* s = 1st source register */
/* t = 2nd source register */
/* c = a comment to be printed if TraceCode is TRUE */
void emitRO(char *op, int r, int s, int t, char *c) {
    fprintf(code, "%3d: %5s %d %d %d ", emitLoc++, op, r, s, t);
    if (TraceCode) fprintf(code, "\t%s", c);
    fprintf(code, "\n");
    if (highEmitLoc < emitLoc) highEmitLoc = emitLoc;
}

/* Procedure emitRM emits a register-to-memory TM instruction */
/* op = the opcode */
/* r = target register */
/* d = the offset register */
/* s = the base register */
/* c = a comment to be printed if TraceCode is TRUE */
void emitRM(char *op, int r, int d, int s, char *c) {
    fprintf(code, "%3d: %5s %d %d(%d) ", emitLoc++, op, r, d, s);
    if (TraceCode) fprintf(code, "\t%s", c);
    fprintf(code, "\n");
    if (highEmitLoc < emitLoc) highEmitLoc = emitLoc;
}


/* Function emitSkip skips "howMany" code locations for later backpatch. It also returns the current code position */
int emitSkip(int loc) {
    int i = emitLoc;
    emitLoc += loc;
    if (highEmitLoc < emitLoc) highEmitLoc = emitLoc;
    return i;
}

// Procedure emitBackup backs up to loc = a previously skipped location
void emitBackup(int loc) {
    if (loc > highEmitLoc) emitComment("BUG in emitBackup");
    emitLoc = loc;
}

/* Procedure emitRestore restores the current code position to the highest previously unemitted position */
void emitRestore(void) {
    emitLoc = highEmitLoc;
}


/* Procedure emitRM_Abs converts an absolute reference to a pc-relative reference when emitting a register-to-memory TM instruction */
/* op = the opcode */
/* r = target register */
/* a = the absolute memory address */
/* c = a comment to be printed if TraceCode is TRUE */
void emitRM_Abs(char *op, int r, int a, char *c) {
    fprintf(code, "3d: %5d %s,%d(%d) ",
            emitLoc, op, r, a - (emitLoc + 1), pc);
    ++emitLoc;
    if (TraceCode) fprintf(code, "\t%s", c);
    fprintf(code, "\n");
    if (highEmitLoc < emitLoc) highEmitLoc = emitLoc;
}