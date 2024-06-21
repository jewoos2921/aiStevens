//
// Created by jewoo on 2024-06-14.
//

#include "Defs.h"
#include "Data.h"
#include "Decl.h"


// List of available registers and their names
static int freeReg_[4];
static char *regList_[4] = {"r8", "r9",
                            "r10", "r11"};
static char *breList[4] = {"r8b", "r9b",
                           "r10b", "r11b"};

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
    fatal("Out of registers");
}

// Return a register to the list of available registers.
// Check to see if it's not already there.
static void freeRegister(int reg) {
    if (freeReg_[reg]) {
        fatald("Error trying to free register", reg);
    }
    freeReg_[reg] = 1;
}

// Print out the assembly preamble
void cgPreamble() {
    freeAllRegisters();
    fputs(
            "\tglobal\tmain\n"
            "\textern\tprintf\n"
            "\tsection\t.text\n"
            "LC0:\tdb\t\"%d\",10,0\n"
            "printint:\n"
            "\tpush\trbp\n"
            "\tmov\trbp, rsp\n"
            "\tsub\trsp, 16\n"
            "\tmov\t[rbp-4], edi\n"
            "\tmov\teax, [rbp-4]\n"
            "\tmov\tesi, eax\n"
            "\tlea	rdi, [rel LC0]\n"
            "\tmov	eax, 0\n"
            "\tcall	printf\n"
            "\tnop\n"
            "\tleave\n"
            "\tret\n"
            "\n",
            OutFile_);
}

// Print out a function preamble
void cgFuncPreamble(char *name_) {
    fprintf(OutFile_,
            "\tsection\t.text\n"
            "\tglobal\t%s\n"
            "%s:\n"
            "\tpush\trbp\n"
            "\tmov\trbp, rsp\n", name_, name_);
}

// Print out a function postamble
void cgFuncPostamble() {
    fputs("\tmov   eax, 0\n"
          "\tpop   rbp\n"
          "\tret\n", OutFile_);
}

// Load an integer literal value into a register. Return the number of the register.
// For x86-64, we don't need to worry about the type.
int cgLoadInt(int value_, int type_) {
    int r = allocRegister();
    fprintf(OutFile_, "\tmov\t%s, %d\n", regList_[r], value_);
    return r;
}

int cgLoadGlobal(int id_) {
    int r = allocRegister();

    // Print out the code to initialise it: P_CHAR or P_INT
    if (Gsym_[id_].type_ == P_INT)
        fprintf(OutFile_, "\tmov\t%s, [%s]\n", regList_[r], Gsym_[id_].name_);
    else
        fprintf(OutFile_, "\tmovzx\t%s, byte [%s]\n",
                regList_[r], Gsym_[id_].name_);
    return r;
}


int cgAdd(int r1_, int r2_) {
    fprintf(OutFile_, "\tadd\t%s, %s\n", regList_[r2_], regList_[r1_]);
    freeRegister(r1_);
    return r2_;
}

int cgSub(int r1_, int r2_) {
    fprintf(OutFile_, "\tsub\t%s, %s\n", regList_[r1_], regList_[r2_]);
    freeRegister(r2_);
    return r1_;
}

int cgMul(int r1_, int r2_) {
    fprintf(OutFile_, "\timul\t%s, %s\n",
            regList_[r2_], regList_[r1_]);
    freeRegister(r1_);
    return r2_;
}

int cgDiv(int r1_, int r2_) {
    fprintf(OutFile_, "\tmov\trax, %s\n", regList_[r1_]);
    fprintf(OutFile_, "\tcqo\n");
    fprintf(OutFile_, "\tidiv\t%s\n", regList_[r2_]);
    fprintf(OutFile_, "\tmov\t%s, rax\n", regList_[r1_]);
    freeRegister(r2_);
    return r1_;
}

void cgPrintInt(int r_) {
    fprintf(OutFile_, "\tmov\trdi, %s\n", regList_[r_]);
    fprintf(OutFile_, "\tcall\tprintint\n");
    freeRegister(r_);
}

// Store a register's value into a variable
int cgStorGlob(int r_, int id_) {

    // Choose P_INT or P_CHAR
    if (Gsym_[id_].type_ == P_INT)
        fprintf(OutFile_, "\tmov\t[%s], %s\n", Gsym_[id_].name_, regList_[r_]);
    else
        fprintf(OutFile_, "\tmov\t[%s], %s\n",
                Gsym_[id_].name_, breList[r_]);
    return r_;
}

// Generate a global symbol
void cgGlobSym(int id_) {
    // Choose P_INT or P_CHAR
    if (Gsym_[id_].type_ == P_INT)
        fprintf(OutFile_, "\tcommon\t%s 8:8\n", Gsym_[id_].name_);
    else
        fprintf(OutFile_, "\tcommon\t%s 1:1\n", Gsym_[id_].name_);
}

// List of comparison operators, indexed by ASTop values
static char *cmpList[] = {"sete", "setne", "setl", "setg", "setle", "setge"};

// Compare two registersand set if true.
int cgCompareAndSet(int ASTop_, int r1_, int r2_) {

    // Check the range of the AST operation
    if (ASTop_ < A_EQ || ASTop_ > A_GE) {
        fatal("Bad ASTop in cgCompareAndSet");
    }

    fprintf(OutFile_, "\tcmp\t%s, %s\n", regList_[r1_], regList_[r2_]);
    fprintf(OutFile_, "\t%s\t%s\n", cmpList[ASTop_ - A_EQ], breList[r2_]);
    fprintf(OutFile_, "\tmovzb\t%s, %s\n", regList_[r2_], breList[r2_]);
    freeRegister(r1_);
    return r2_;
}

void cgLabel(int label_) {
    fprintf(OutFile_, "L%d:\n", label_);
}

void cgJump(int label_) {
    fprintf(OutFile_, "\tjmp\tL%d\n", label_);
}

// List of inverted jump instruction in AST opder: A_EQ, A_NE, A_LT, A_GT, A_LE, A_GE
static char *jumpList[] = {"jne", "je", "jge", "jle", "jg", "jl"};

int cgCompareAndJump(int ASTop_, int r1_, int r2_, int label_) {

    // Check the range of the AST operation
    if (ASTop_ < A_EQ || ASTop_ > A_GE) {
        fatal("Bad ASTop in cgCompareAndJump");
    }

    fprintf(OutFile_, "\tcmp\t%s, %s\n", regList_[r1_], regList_[r2_]);
    fprintf(OutFile_, "\t%s\tL%d\n", jumpList[ASTop_ - A_EQ], label_);
    freeAllRegisters();
    return NOREG;
}

// Widen the value in the register from the old to the new type. and return a register with this new value.
int cgWiden(int r_, int oldType_, int newType_) {
    // Nothing to do
    return r_;
}