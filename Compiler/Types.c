//
// Created by jewoo on 2024-06-21.
//

#include "Defs.h"
#include "Data.h"
#include "Decl.h"

// Given two primitive types,
// return true if they are compatible,
// false otherwise. Also return either
// zero or an A_WIDEN operation if one
// has to be widened to match the other.
// If onlyright is true, only widen left to right.
int typeCompatible(int *left_, int *right_, int onlyRight_) {
    int leftSize, rightSize;


    // If both are of the same type, they are compatible
    if (*left_ == *right_) {
        *left_ = *right_ = 0;
        return 1;
    }

    // Get the sizes for each type
    leftSize = genPrimSize(*left_);
    rightSize = genPrimSize(*right_);

    // Types with zero size are not compatible with anything
    if ((leftSize == 0) || (rightSize == 0)) {
        return 0;
    }


    // Widen types as required
    if (leftSize < rightSize) {
        *left_ = A_WIDEN;
        *right_ = 0;
        return 1;
    }

    if (rightSize < leftSize) {
        if (onlyRight_)
            return 0;
        *left_ = 0;
        *right_ = A_WIDEN;
        return 1;
    }

    // Anything remaining is the same size and thus compatible.
    *left_ = *right_ = 0;
    return 1;

}