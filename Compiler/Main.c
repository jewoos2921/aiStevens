#include "Defs.h"

#define extern_

#include "Data.h"

#undef extern_

#include "Decl.h"
#include <errno.h>

static void init() {
    Line_ = 1;
    PutBack_ = '\n';
}

static void usage(char *prog) {
    fprintf(stderr, "Usage: %s [options] <infile> [outfile]\n", prog);
    fprintf(stderr, "\t-a\t\tprint abstract syntax tree\n");
    fprintf(stderr, "\t-o\t\tprint object code\n");
    fprintf(stderr, "\t-s\t\tprint symbol table\n");
    fprintf(stderr, "\t-h\t\tprint this help message\n");
    exit(EXIT_FAILURE);
}


int main(int argc, char *argv[]) {
    struct ASTNode *node;


    if (argc != 2)
        usage(argv[0]);

    init();

    if ((InFile_ = fopen(argv[1], "r")) == NULL) {
        fprintf(stderr, "Unable to open %s: %s\n", argv[1], strerror(errno));
        exit(EXIT_FAILURE);
    }

    // Create the output file
    if ((OutFile_ = fopen("out.s", "w")) == NULL) {
        fprintf(stderr, "Unable to create out.s: %s\n", strerror(errno));
        exit(EXIT_FAILURE);
    }

    scan(&Token_);
    node = binexpr(0);
    printf("%d\n", interpretAST(node));
    generateCode(node);

    fclose(OutFile_);
    return 0;
}
