//
// Created by jewoo on 2024-06-27.
//

#ifndef AISTEVENS_CODE_H
#define AISTEVENS_CODE_H

/* pc = program counter */
#define pc 7

/* mp = "memory pointer" points to top of memory */
#define mp 6

/* gp = "global pointer" points to bottom of memory for (global) variable storage */
#define gp 5

/* ac = "accumulator" */
#define ac 0

/* ac1 = "second accumulator" */
#define ac1 1



/* code emitting utilities */

/* Procedure emitRO emits a registerr-only TM instruction */
/* op = the opcode */
/* r = target register */
/* s = 1st source register */
/* t = 2nd source register */
/* c = a comment to be printed if TraceCode is TRUE */
void emitRO(char *op, int r, int s, int t, char *c);

/* Procedure emitRM emits a register-to-memory TM instruction */
/* op = the opcode */
/* r = target register */
/* d = the offset register */
/* s = the base register */
/* c = a comment to be printed if TraceCode is TRUE */
void emitRM(char *op, int r, int d, int s, char *c);

/* Function emitSkip skips "howMany" code locations for later backpatch. It also returns the current code position */
int emitSkip(int loc);

// Procedure emitBackup backs up to loc = a previously skipped location
void emitBackup(int loc);

/* Procedure emitRestore restores the current code position to the highest previously unemitted position */
void emitRestore(void);

/* Procedure emitComment prints a comment line with comment c in the code file */
void emitComment(char *c);

/* Procedure emitRM_Abs converts an absolute reference to a pc-relative reference when emitting a register-to-memory TM instruction */
/* op = the opcode */
/* r = target register */
/* a = the absolute memory address */
/* c = a comment to be printed if TraceCode is TRUE */
void emitRM_Abs(char *op, int r, int a, char *c);

#endif //AISTEVENS_CODE_H
