//
// Created by jewoo on 2024-06-27.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#ifndef TRUE
#define TRUE 1
#endif
#ifndef FALSE
#define FALSE 0
#endif

#define IADDR_SIZE 1024 /* increase for large programs */
#define DADDR_SIZE 1024 /* decrease for large programs */
#define NO_REGS 8
#define PC_REG 7

#define LINESIZE 121
#define WORDSIZE 20

/* type */
typedef enum {
    opclRR,
    opclRM,
    opclRA,
} OPCLASS;

typedef enum {
    /* RR instructions */
    opHALT, /* RR halt, operands must be zero */
    opIN, /* RR read into reg(r); s and t are ignored */
    opOUT, /* RR write from reg(r); s and t are ignored */
    opADD, /* RR reg(r) = reg(s)+reg(t) */
    opSUB, /* RR reg(r) = reg(s)-reg(t) */
    opMUL, /* RR reg(r) = reg(s)*reg(t) */
    opDIV, /* RR reg(r) = reg(s)/reg(t) */
    opRRLIM, /* limit of RR opcodes */

    /* RM instructions */
    opLD, /* RM reg(r) = mem(d+reg(s)) */
    opST, /* RM mem(d+reg(s)) = reg(r) */
    opRMLIM, /* Limit of RM opcodes */

    /* RA instructions */
    opLDA,  /* RA reg(r) = d + reg(s) */
    opLDC,  /* RA reg(r) = d; reg(s) is ignored */
    opJLT,  /* RA if reg(r) < 0 then reg(7) = d + reg(s) */
    opJLE,  /* RA if reg(r) <= 0 then reg(7) = d + reg(s) */
    opJGT,  /* RA if reg(r) > 0 then reg(7) = d + reg(s) */
    opJGE,  /* RA if reg(r) >= 0 then reg(7) = d + reg(s) */
    opJEQ,  /* RA if reg(r) == 0 then reg(7) = d + reg(s) */
    opJNE,  /* RA if reg(r) != 0 then reg(7) =  d + reg(s) */
    opRALIM,    /* Limit of RA opcodes */
} OPCODE;

typedef enum {
    srOKAY,
    srHALT,
    srIMEM_ERR,
    srDMEM_ERR,
    srZERODIVIDE,
} STEPRESULT;

typedef struct {
    int iop;
    int iarg1;
    int iarg2;
    int iarg3;
} INSTRUCTION;

/************ VARS ************/
int iloc = 0;
int dloc = 0;
int traceflag = FALSE;
int icountflag = FALSE;

INSTRUCTION iMem[IADDR_SIZE];
int dMem[DADDR_SIZE];
int reg[NO_REGS];

char *opCodeTab[] = {"HALT", "IN", "OUT", "ADD", "SUB", "MUL", "DIV",
                     "????",
        /* RR opcodes */
                     "LD", "ST", "????",    /* RM opcodes */
                     "LDA", "LDC", "JLT", "JLE",
                     "JGT", "JGE", "JEQ", "JNE", "????",
        /* RA opcodes */
};

char *stepResultTab[] = {"OK", "Halted", "Instruction Memory Fault",
                         "Data Memory Fault", "Division by 0"};

char pgmName[20];
FILE *pgm;

char in_Line[LINESIZE];
int lineLen;
int inCol;
int num;
char word[WORDSIZE];
char ch;
int done;
