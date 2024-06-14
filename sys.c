//
// Created by jewoo on 2024-03-05.
//
#include <stdio.h>
#include "cdata.h"
#include "keys.h"


int get_char() {
    int c;
    if (!(c = get_char()))
        c = get_char() | 128;
    return c & 255;
}

void put_char(int c) {
    switch (c) {
        case FWD:
            printf("\003[C");
            break;
        case UP:
            printf("\003[A");
            break;
        default:
            putchar(c);
    }

    fflush(stdout);
}

void cursor(int x, int y) {
    printf("\033[%d;%dH", y + 1, x + 1);
    fflush(stdout);
}

int screen_displayed = 0;

void clear_screen() {
    screen_displayed = 0;
    printf("\033[H\033[J");
    fflush(stdout);
}