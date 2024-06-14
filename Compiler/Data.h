//
// Created by jewoo on 2024-06-14.
//

#pragma once

#ifndef extern_
#define extern_ extern
#endif

extern_ int Line_;
extern_ int PutBack_;
extern_ FILE *InFile_;
extern_ FILE *OutFile_;
extern_ struct Token Token_;
extern_ char Text_[TEXTLEN + 1];