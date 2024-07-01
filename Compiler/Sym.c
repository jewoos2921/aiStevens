//
// Created by jewoo on 2024-06-17.
//

#include "Defs.h"
#include "Data.h"
#include "Decl.h"


// Determine if the symbol s is in the global symbol table.
// Return its slot position or -1 if not found.
int findGlob(char *s_) {
    int i;
    for (i = 0; i < Globs_; i++) {
        if (*s_ == *Gsym_[i].name_ && !strcmp(s_, Gsym_[i].name_)) {
            return i;
        }
    }
    return -1;
}

// Get the position of a new global symbol slot, or die if we've run out of positions.
static int newGlob() {
    int p;
    if ((p = Globs_++) >= NSYMBOLS) {
        fatal("too many global symbols");
    }
    return p;
}

// Add a global symbol to the symbol table.
// Also set up its type and structural type.
// Return the slot number in the symbol table
int addGlob(char *name_, int type_, int stype_, int endlabel_) {
    int y;

    // if this is already in the symbol table, return the existing slot
    if ((y = findGlob(name_)) != -1) {
        return y;
    }

    // Otherwise get a new slpt, fill it in and return the slot number
    y = newGlob();
    Gsym_[y].name_ = strdup(name_);
    Gsym_[y].type_ = type_;
    Gsym_[y].stype_ = stype_;
    Gsym_[y].endlabel_ = endlabel_;
    return y;
}