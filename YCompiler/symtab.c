//
// Created by jewoo on 2024-06-27.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "symtab.h"


// SIZE is the size of the hash table
#define SIZE 211

// SHIFT is the power of two used as multiplier in hash function
#define SHIFT 4

// the hash function
static int hash(char *key) {
    int temp = 0;
    int i = 0;
    while (key[i] != '\0') {
        temp = ((temp << SHIFT) + key[i]) % SIZE;
        i++;
    }
    return temp;
}

// The list of line numbers of the source code in which a variable is referenced
typedef struct LinListRec {
    int lineno;
    struct LinListRec *next;
} *LineList;

/* The record in the bucket lists for each variable, including name, assigned
 * memory location, and the list of line numberrs in which it appears in the source code*/
typedef struct BucketListRec {
    char *name;
    LineList lines;
    int memloc; // memory location for the variable
    struct BucketListRec *next;
} *BucketList;

/* The hash table */
static BucketList hashTable[SIZE];

// Procedure st_insert inserts line numbers and memory locations into the symbol table loc =
// memory locations is inserted only the first time, otherwise ignored.
void st_insert(char *name, int lineno, int loc) {
    int h = hash(name);
    BucketList l = hashTable[h];
    while ((l != NULL) && (strcmp(name, l->name) != 0))
        l = l->next;
    if (l == NULL) { // variable not yet in table
        l = (BucketList) malloc(sizeof(struct BucketListRec));
        l->name = name;
        l->lines = (LineList) malloc(sizeof(struct LinListRec));
        l->lines->lineno = lineno;
        l->memloc = loc;
        l->next = hashTable[h];
        hashTable[h] = l;
    } else { // found in table, so just add line number
        LineList t = l->lines;
        while (t->next != NULL) t = t->next;
        t->next = (LineList) malloc(sizeof(struct LinListRec));
        t->next->lineno = lineno;
        t->next->next = NULL;
    }
}

// Function st_lookup returns the memory location of variable or -1 if not found.
int st_lookup(char *name) {
    int h = hash(name);
    BucketList l = hashTable[h];
    while ((l != NULL) && (strcmp(name, l->name) != 0))
        l = l->next;
    if (l == NULL) return -1;
    else return l->memloc;
}

/* Procedure prints a formatted listing of the symbol table contents to the listing file */
void printSymTab(FILE *listing) {
    int i;
    fprintf(listing, "Variable Name Location Line Numbers\n");
    fprintf(listing, "------------- -------- ---------------\n");
    for (i = 0; i < SIZE; ++i) {
        if (hashTable[i] != NULL) {
            BucketList  l = hashTable[i];
            while (l != NULL) {
                LineList t = l->lines;
                fprintf(listing, "%-1s ", l->name);
                fprintf(listing, "%-8d  ", l->memloc);
                while (t != NULL) {
                    fprintf(listing, "%4d ", t->lineno);
                    t = t->next;
                }
                fprintf(listing, "\n");
                l = l->next;
            }
        }
    }
}
