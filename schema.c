//
// Created by jewoo on 2024-03-05.
//

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "cdata.h"

#define SCHEMA

#define MXCAT 3
#define NAMLEN 31
#define REMAKRK ';'
#define MAXLINE 120

struct dict {
    char dename[NAMLEN + 1];
    char detype;
    int delen;
    char *demask;
} dc[MXELE];

int dectr = 0;
int fctr = 0;
char filename[MXFILS][NAMLEN + 1];
int fileele[MXFILS][MXELE];
int ndxele[MXFILS][MXINDEX][MXCAT];

char word[NAMLEN + 1];
int lnctr = 0;
char line[160];


char *ers[] = {
        "invalid name",
        "invalid length",
        "comma missing",
        "invalid data type",
        "quote missing",
        "#schema missing",
        "#<command> missing",
        "unexpected end of file",
        "duplicate file name",
        "unknown data element",
        "too many data elements",
        "out of memory",
        "unknown file name",
        "too many indices in file",
        "too many elements in index",
        "duplicate data element",
        "too many files",
        "invalid command line switch"
};
