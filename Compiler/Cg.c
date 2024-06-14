//
// Created by jewoo on 2024-06-14.
//

#include "Defs.h"
#include "Data.h"
#include "Decl.h"


// List of available registers and their names
static int freeReg_[4];
static char *regList_[4] = {"%r8", "%r9",
                            "%r10", "%r11"};

// set all registers as available
void freeAllRegisters() {
    for (int i = 0; i < 4; ++i) {
        freeReg_[i] = 1;
    }
}

// Allocate a free register. Return the number of the register.
// Die if no available registers.
static int allocRegister() {
    for (int i = 0; i < 4; ++i) {
        if (freeReg_[i]) {
            freeReg_[i] = 0;
            return i;
        }
    }
    fprintf(stderr, "Error: out of registers\n");
    exit(1);
}

// Return a register to the list of available registers.
// Check to see if it's not already there.
static void freeRegister(int reg) {
    if (freeReg_[reg]) {
        fprintf(stderr, "Error: double free of register %d\n", reg);
        exit(1);
    }
    freeReg_[reg] = 1;
}

// Print out the assembly preamble
void cgPreamble() {
    freeAllRegisters();
    fputs(
            "\t.text\n"
            ".LC0:\n"
            "\t.string\t\"%d\\n\"\n"
            "printint:\n"
            "\tpushq\t%rbp\n"
            "\tmovq\t%rsp, %rbp\n"
            "\tsubq\t$16, %rsp\n"
            "\tmovl\t%edi, -4(%rbp)\n"
            "\tmovl\t-4(%rbp), %eax\n"
            "\tmovl\t%eax, %esi\n"
            "\tleaq	.LC0(%rip), %rdi\n"
            "\tmovl	$0, %eax\n"
            "\tcall	printf@PLT\n"
            "\tnop\n"
            "\tleave\n"
            "\tret\n"
            "\n"
            "\t.globl\tmain\n"
            "\t.type\tmain, @function\n"
            "main:\n"
            "\tpushq\t%rbp\n"
            "\tmovq	%rsp, %rbp\n",
            OutFile_);
}

// Print out the assembly postamble
void cgPostamble() {
    fputs(
            "\tmovl	$0\n"
            "\tpopq rbp\n"
            "\tret\n",
            OutFile_);
}

int cgLoad(int value_) {
    int r = allocRegister();
    fprintf(OutFile_, "\tmovl\t$%d, %s\n", value_, regList_[r]);
    return r;
}

int cgAdd(int r1_, int r2_) {
    fprintf(OutFile_, "\taddq\t%s, %s\n", regList_[r2_], regList_[r1_]);
    freeRegister(r1_);
    return r2_;
}

int cgSub(int r1_, int r2_) {
    fprintf(OutFile_, "\tsubq\t%s, %s\n", regList_[r1_], regList_[r2_]);
    freeRegister(r2_);
    return r1_;
}

int cgMul(int r1_, int r2_) {
    fprintf(OutFile_, "\timulq\t%s, %s\n", regList_[r2_], regList_[r1_]);
    freeRegister(r1_);
    return r2_;
}

int cgDiv(int r1_, int r2_) {
    fprintf(OutFile_, "\tmov\trax, %s\n", regList_[r1_]);
    fprintf(OutFile_, "\tcqo\n");
    fprintf(OutFile_, "\tidivq\t%s\n", regList_[r2_]);
    fprintf(OutFile_, "\tmovq\t%%rax, %s\n", regList_[r1_]);
    freeRegister(r2_);
    return r1_;
}

void cgPrintInt(int r_) {
    fprintf(OutFile_, "\tmovq\t%s, %%rdi\n", regList_[r_]);
    fprintf(OutFile_, "\tcall\tprintint\n");
    freeRegister(r_);
}

