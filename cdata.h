//
// Created by jewoo on 2024-03-04.
//

#ifndef AISTEVENS_CDATA_H
#define AISTEVENS_CDATA_H

#define ERROR -1
#define OK 0

#ifdef TRUE
#define TRUE 1
#define FALSE 0
#endif

#include <errno.h>
#include <fcntl.h>
#include <ctype.h>

#define CMODE 0666
#define OPENMODE O_RDWR

#define NODE 512
#define RPTR long
#define MXFILS 11
#define MXELE 100
#define MXINDEX 5

#define D_NF 1
#define D_PRIOR 2
#define D_EOF 3
#define D_BOF 4
#define D_DUPL 5
#define D_OM 6
#define D_INDXC 7
#define D_IOERR 8

#define MXKEYLEN 80

#ifndef SCHEMA
extern char *dbfiles[];
extern char *denames[];
extern char *elmask[];
extern char eltype[];
extern int ellen[];
extern int *file_ele[];
extern int **index_ele[];

// =====================================================================
void mov_mem(char *s, char *d, int i);

// =====================================================================
void build_b(char *name, int len);

void put_char(int c);

void clear_screen();

void cursor();

void error_message();

void clear_notice();

void post_notice();

// =====================================================================
void db_open(char *, int *);

void db_cls();

void dberror();

void init_rcd(int, char *);

void clrrcd(char *, int *);

void rcd_fill(char *, char *, int *, int *);

void build_index();

// =====================================================================
void init_screen();

void protect();

void edit();

void display_template();

void tally();

void put_field();

#endif

typedef struct fhdr {
    RPTR first_record;
    RPTR next_record;
    int record_length;
} FHEADER;

extern FHEADER fh[];

#endif //AISTEVENS_CDATA_H
