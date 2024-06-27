//
// Created by jewoo on 2024-06-26.
//

#ifndef AISTEVENS_SCAN_H
#define AISTEVENS_SCAN_H

/* MAXTOKENLEN is the maximum size of a token */
#define MAXTOKENLEN 40

/* tokenString array stores the lexeme of each token */
extern char tokenString[MAXTOKENLEN + 1];

/* funcion getToken returns the next token in source file */
TokenType getToken(void);

#endif //AISTEVENS_SCAN_H
