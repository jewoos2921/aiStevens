//
// Created by jewoo on 2024-06-14.
//

#include "Defs.h"
#include "Data.h"
#include "Decl.h"


// Lexical Scanning



// Return the lookahead character and
// fetch the next character
// from the input stream
static int chrpos(char *s, char c) {
    char *p;

    p = strchr(s, c);
    return p ? p - s : -1;
}

static int next() {
    int c;
    if (PutBack_) {
        c = PutBack_;
        PutBack_ = 0;
        return c;
    }

    c = getc(InFile_);
    if ('\n' == c) Line_++;
    return c;
}

static void putback(int c) {
    PutBack_ = c;
}

static int skip() {
    int c;

    c = next();
    while (' ' == c ||
           '\t' == c ||
           '\n' == c ||
           '\r' == c ||
           '\f' == c) {
        c = next();
    }

    return c;
}

static int scanint(int c) {
    int k, val = 0;

    // convert each character into an int value
    while ((k = chrpos("0123456789", c)) >= 0) {
        val = val * 10 + k;
        c = next();
    }

    putback(c);
    return val;
}

int scan(struct Token *t_) {
    int c;
    c = skip();


    switch (c) {
        case EOF:
            return 0;
        case '+':
            t_->token_ = T_PLUS;
            break;
        case '-':
            t_->token_ = T_MINUS;
            break;
        case '*':
            t_->token_ = T_STAR;
            break;
        case '/':
            t_->token_ = T_SLASH;
            break;
        default:

            if (isdigit(c)) {
                t_->int_value_ = scanint(c);
                t_->token_ = T_INTLIT;
                break;
            } else {
                printf("Bad character '%c'\n", c);
                exit(1);
            }
    }

    // We found a token
    return 1;
}