//
// Created by jewoo on 2024-06-26.
//

#include "globals.h"

#define NO_PARSE FALSE

// set NO_ANALYZE to TRUE to get a parser-only compiler
#define NO_ANALYZE FALSE

// set NO_CODE to TRUE to disable code generation
#define NO_CODE FALSE

#include "util.h"

#if NO_PARSE
#include "scan.h"
#else

#include "parse.h"

#if !NO_ANALYZE

#include "analyze.h"

#if !NO_CODE

#include "cgen.h"

#endif
#endif
#endif

/* allocate global variables */
int lineno = 0;
FILE *source;
FILE *listing;
FILE *code;

/* allocate and set tracing flags */
int EchoSource = TRUE;
int TraceScan = TRUE;
int TraceParse = TRUE;
int TraceAnalyze = TRUE;
int TraceCode = TRUE;

int Error = FALSE;

int main(int argc, char *argv[]) {

}