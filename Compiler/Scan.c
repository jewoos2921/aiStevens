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

    c = fgetc(InFile_);
    if ('\n' == c)
        Line_++;

    return c;
}

static void putback(int c) {
    PutBack_ = c;
}

static int skip() {
    int c;

    c = next();
    while (' ' == c || '\t' == c || '\n' == c || '\r' == c || '\f' == c) {
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


// Scan an identifier from the input file and
// store it in buf[]. Return the identifier's length
static int scanident(int c, char *buf, int lim) {
    int i = 0;

    // Allow digits, alpha and underscores
    while (isalpha(c) || isdigit(c) || '_' == c) {
        if (lim - 1 == i) {
            printf("Error: Identifier too long on line %d\n", Line_);
            exit(1);
        } else if (i < lim - 1) {
            buf[i++] = c;
        }
        c = next();
    }

    putback(c);
    buf[i] = '\0';
    return i;
}


static int keyword(char *s) {
    switch (*s) {
        case 'i':
            if (!strcmp(s, "int"))
                return T_INT;
            break;

        case 'p':
            if (!strcmp(s, "print"))
                return T_PRINT;
            break;
    }
    return 0;
}

// Scan and return the next token
int scan(struct Token *t_) {
    int c;
    int token_type;

    // skip whitespace
    c = skip();

    switch (c) {
        case EOF:
            t_->token_ = T_EOF;
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
        case ';':
            t_->token_ = T_SEMI;
            break;
        case '=':
            if ((c = next()) == '=') {
                t_->token_ = T_EQ;
            } else {
                putback(c);
                t_->token_ = T_ASSIGN;
            }
            break;
        case '!':
            if ((c = next()) == '=') {
                t_->token_ = T_NE;
            } else {
                fatalc("Unrecognised token", c);
            }
            break;
        case '<':
            if ((c = next()) == '=') {
                t_->token_ = T_LE;
            } else {
                putback(c);
                t_->token_ = T_LT;
            }
            break;
        case '>':
            if ((c = next()) == '=') {
                t_->token_ = T_GE;
            } else {
                putback(c);
                t_->token_ = T_GT;
            }
            break;
        default:
            if (isdigit(c)) {
                t_->int_value_ = scanint(c);
                t_->token_ = T_INTLIT;
                break;
            } else if (isalpha(c) || '_' == c) {
                // Read in a keyword or identifier
                scanident(c, Text_, TEXTLEN);

                // If it's a recognised keyword, return that token
                token_type = keyword(Text_);
                if (token_type) {
                    t_->token_ = token_type;
                    break;
                }

                t_->token_ = T_IDENT;
                break;
            }

            fatalc("Unrecognised character", c);
    }
    // We found a token
    return 1;
}