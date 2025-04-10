//
// Created by jewoo on 2024-06-14.
//

#include "Defs.h"
#include "Data.h"
#include "Decl.h"


// Lexical Scanning


// Return the position of character c in string s, or -1 if c not found
static int chrpos(char *s, char c) {
    char *p;

    p = strchr(s, c);
    return p ? p - s : -1;
}

// Get the next character from the input file.
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

// Put back an unwanted character.
static void putback(int c) {
    PutBack_ = c;
}

// Skip past input that we don't need to deal with,
// i.e. whitespace, newlines. Return the first
// character we do need to deal with.
static int skip() {
    int c;

    c = next();
    while (' ' == c || '\t' == c || '\n' == c || '\r' == c || '\f' == c) {
        c = next();
    }

    return c;
}

// Scan and return an integer literal value from the input file.
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
            fatal("identifier too long");
        } else if (i < lim - 1) {
            buf[i++] = c;
        }
        c = next();
    }

    putback(c);
    buf[i] = '\0';
    return i;
}

// Given a word from the input, return the matching
// keyword token number or 0 if it's not a keyword.
// Switch on the first letter so that we don't have
// to waste time strcmp()ing against all the keywords.
static int keyword(char *s) {
    switch (*s) {

        case 'c':
            if (!strcmp(s, "char"))
                return T_CHAR;
            break;

        case 'e':
            if (!strcmp(s, "else"))
                return T_ELSE;
            break;

        case 'f':
            if (!strcmp(s, "for"))
                return T_FOR;
            break;

        case 'i':
            if (!strcmp(s, "if"))
                return T_IF;
            if (!strcmp(s, "int"))
                return T_INT;
            break;

        case 'l':
            if (!strcmp(s, "long"))
                return T_LONG;
            break;

        case 'p':
            if (!strcmp(s, "print"))
                return T_PRINT;
            break;

        case 'r':
            if (!strcmp(s, "return"))
                return T_RETURN;
            break;

        case 'w':
            if (!strcmp(s, "while"))
                return T_WHILE;
            break;

        case 'v':
            if (!strcmp(s, "void"))
                return T_VOID;
            break;
    }
    return 0;
}

static struct Token *RejectedToken = NULL;

// A pointer to a rejected token
void rejectToken(struct Token *t_) {
    if (RejectedToken != NULL)
        fatal("only one token can be rejected");
    RejectedToken = t_;
}

// Scan and return the next token found in the input.
// Return 1 if token valid, 0 if no tokens left.
int scan(struct Token *t_) {
    int c;
    int token_type;

    if (RejectedToken != NULL) {
        t_ = RejectedToken;
        RejectedToken = NULL;
        return 1;
    }

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
        case '{':
            t_->token_ = T_LBRACE;
            break;
        case '}':
            t_->token_ = T_RBRACE;
            break;
        case '(':
            t_->token_ = T_LPAREN;
            break;
        case ')':
            t_->token_ = T_RPAREN;
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
            // If it's a digit, scan the literal integer value in
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
