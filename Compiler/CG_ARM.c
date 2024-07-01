//
// Created by jewoo on 2024-07-01.
//

#include "Defs.h"
#include "Data.h"
#include "Decl.h"

// code generator for ARMv6 on Rasberry PI

// List of available registers and their names
static int freeRegs_[4];
static char *regList_[4] = {"r4", "r5", "r6", "r7"};


// Set all registers as available
void freeAllRegs() {
    freeRegs_[0] = 1;
    freeRegs_[1] = 1;
    freeRegs_[2] = 1;
    freeRegs_[3] = 1;
}

// Allocate a free register. Return the number of the register.
// Die if no available registers.
static int allocRegister() {
    for (int i = 0; i < 4; ++i) {
        if (freeRegs_[i]) {
            freeRegs_[i] = 0;
            return i;
        }
    }
    fatal("Out of registers");
    return NOREG;
}

// Return a register to the list of available registers.
// Check to see if it's not already there.
static void freeRegister(int reg) {
    if (freeRegs_[reg] != 0) {
        fatald("Error trying to free register", reg);
    }
    freeRegs_[reg] = 1;
}

// We have to store large integer literal values in memory.
// keep a list of them which will be output in the postamble
#define MAXINTS 1024
int IntList_[MAXINTS];
static int IntSlot_ = 0;

