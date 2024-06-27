//
// Created by jewoo on 2024-06-27.
//

#ifndef AISTEVENS_SYMTAB_H
#define AISTEVENS_SYMTAB_H

// Procedure st_insert inserts line numbers and memory locations into the symbol table loc =
// memory locations is inserted only the first time, otherwise ignored.
void st_insert(char *name, int lineno, int loc);

// Function st_lookup returns the memory location of variable or -1 if not found.
int st_lookup(char *name);

/* Procedure prints a formatted listing of the symbol table contents to the listing file */
void printSymTab(FILE *listing);

#endif //AISTEVENS_SYMTAB_H
