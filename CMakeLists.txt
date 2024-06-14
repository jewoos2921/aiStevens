cmake_minimum_required(VERSION 3.27)
project(aiStevens C)

set(CMAKE_C_STANDARD 11)

add_executable(aiStevens
#        main.c
        Compiler/Main.c
        Compiler/Scan.c
        Compiler/Expr.c
        Compiler/Interp.c
        Compiler/Tree.c
        #        data_element.c
        #        database.c
        #        schema.c
        #        sys.c
        #        file_spec.c
        #        index_spec.c
        #        bree.c
#        gpt_train.c
)
