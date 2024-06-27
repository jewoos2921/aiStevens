//
// Created by jewoo on 2024-06-27.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#ifndef TRUE
#define TRUE 1
#endif
#ifndef FALSE
#define FALSE 0
#endif

#define IADDR_SIZE 1024 /* increase for large programs */
#define DADDR_SIZE 1024 /* decrease for large programs */
#define NO_REGS 8
#define PC_REG 7
