//
// Created by jewoo on 2024-03-04.
//

#include "data_element.h"

int ellen[] = {
        5, 25, 25, 25, 2, 5, 10, 8, 5, 25,
        9, 5, 25, 5, 9, 9, 2
};

char *denames[] = {
        "CLIENT_NO",
        "CLIENT_NAME",
        "ADDRESS",
        "CITY",
        "STATE",
        "ZIP",
        "PHONE",
        "AMT_DUE",
        "PROJECT_NO",
        "PROJECT_NAME",
        "CONSULTANT_NO",
        "CONSULTANT_NAME",
        "RATE",
        "PAYMENT",
        "EXPENSE",
        "HOURS",
        (char *) 0
};

char eltype[] = "ZAAAANNCZACZACCCN";
char *elmask[] = {
        "_____",
        "_________________________",
        "_________________________",
        "_________________________",
        "__",
        "_____",
        "(___)___-____",
        "$______.__",
        "_____",
        "_________________________",
        "$_______.__",
        "_____",
        "_________________________",
        "$___.__",_
        "$_______.__",
        "$_______.__",
        "__"
};