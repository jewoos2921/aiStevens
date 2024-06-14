//
// Created by jewoo on 2024-03-05.
//

#include <stdio.h>
#include <unistd.h>
#include <unctrl.h>
#include "bree.h"

#define MXTREES 20
#define ADR sizeof(RPTR)
#define KLEN bheader[trx].keylength
#define ENTLN (KLEN + ADR)

typedef struct treenode {
    int nonleaf;
    RPTR prntnode;
    RPTR lfsib;
    RPTR rtsib;
    int keyct;
    RPTR key0;
    char keyspace[NODE - ((sizeof(int) * 2) + (ADR * 4))];
    char spil[MXKEYLEN];
} BTREE;

typedef struct treehdr {
    RPTR rootnode;
    int keylength;
    int m;
    RPTR rlsed_node;
    RPTR endnode;
    int locked;
    RPTR leftmost, rightmost;
} HEADER;

HEADER bheader[MXTREES];
BTREE trnode;

int handle[MXTREES];
int currkno[MXTREES];
RPTR currnode[MXTREES];
int trx;

int btree_init(char *ndx_name) {
    for (trx = 0; trx < MXTREES; trx++)
        if (handle[trx] == 0)
            break;
    if (trx == MXTREES)
        return ERROR;

    if ((handle[trx] = open(ndx_name, OPENMODE)) == ERROR)
        return ERROR;

    lseek(handle[trx], 0, SEEK_SET);
    read(handle[trx], (char *) &bheader[trx], sizeof(HEADER));
    if (bheader[trx].locked) {
        close(handle[trx]);
        handle[trx] = 0;
        return ERROR;
    }
    bheader[trx].locked = TRUE;
    lseek(handle[trx], 0, 0);
    write(handle[trx], (char *) &bheader[trx], sizeof(HEADER));
    currkno[trx] = 0;
    currnode[trx] = 0;
    return trx;
}

int btree_close(int tree);

void build_b(char *name, int len);

RPTR locate(int tree, char *k);

int deletekey(int tree, char *x, RPTR ad);

int insertkey(int tree, char *x, RPTR ad, int unique);

RPTR nextkey(int tree);

RPTR prevkey(int tree);

RPTR firstkey(int tree);

RPTR lastkey(int tree);

RPTR currkey(int tree);

void keyval(int tree, char *ky);
