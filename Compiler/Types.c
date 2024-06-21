//
// Created by jewoo on 2024-06-21.
//

#include "Defs.h"
#include "Data.h"
#include "Decl.h"


int typeCompatible(int *left_, int *right_, int onlyRight_) {
    // Voids not compatible with anything
    if ((*left_ == P_VOID) || (*right_ == P_VOID)) {
        return 0;
    }

    // If both are of the same type, they are compatible
    if (*left_ == *right_) {
        *left_ = *right_ = 0;
        return 1;
    }

    // Widen P_CHARs to P_INTs as required
    if ((*left_ == P_CHAR) && (*right_ == P_INT)) {
        *left_ = A_WIDEN;
        *right_ = 0;
        return 1;
    }

    if ((*right_ == P_CHAR) && (*left_ == P_INT)) {
        if (onlyRight_)
            return 0;
        *left_ = 0;
        *right_ = A_WIDEN;
        return 1;
    }

    // If only the right operand is of type A_WIDEN, it is compatible
    *left_ = *right_ = 0;
    return 1;

}