cmake_minimum_required(VERSION 3.27)
project(aiStevens C)

set(CMAKE_C_STANDARD 11)

add_executable(aiStevens
#        main.c
        Compiler/Main.c
        Compiler/Scan.c
        Compiler/Expr.c
        Compiler/Tree.c
#        Compiler/cg.c
        Compiler/Cg_x86_64.c
        Compiler/Gen.c
        Compiler/Stmt.c
        Compiler/Misc.c
        Compiler/Decl.c
        Compiler/Sym.c
        Compiler/DB.c
        Compiler/Types.c
        YCompiler/main.c
        YCompiler/util.c
        YCompiler/scan.c
        YCompiler/cgen.c
        YCompiler/parse.c
        YCompiler/symtab.c
        YCompiler/tm.c
        YCompiler/analyze.c
        #        Compiler/Expr2.c
        #        data_element.c
        #        database.c
        #        schema.c
        #        sys.c
        #        file_spec.c
        #        index_spec.c
        #        bree.c
#        gpt_train.c
)
