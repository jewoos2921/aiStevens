# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.27

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:

#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:

# Disable VCS-based implicit rules.
% : %,v

# Disable VCS-based implicit rules.
% : RCS/%

# Disable VCS-based implicit rules.
% : RCS/%,v

# Disable VCS-based implicit rules.
% : SCCS/s.%

# Disable VCS-based implicit rules.
% : s.%

.SUFFIXES: .hpux_make_needs_suffix_list

# Command-line flag to silence nested $(MAKE).
$(VERBOSE)MAKESILENT = -s

#Suppress display of executed commands.
$(VERBOSE).SILENT:

# A target that is always out of date.
cmake_force:
.PHONY : cmake_force

#=============================================================================
# Set environment variables for the build.

# The shell in which to execute make rules.
SHELL = /bin/sh

# The CMake executable.
CMAKE_COMMAND = /usr/local/bin/cmake

# The command to remove a file.
RM = /usr/local/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /mnt/c/Users/jewoo/CLionProjects/aiStevens

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /mnt/c/Users/jewoo/CLionProjects/aiStevens/cmake-build-debug

# Include any dependencies generated for this target.
include CMakeFiles/aiStevens.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include CMakeFiles/aiStevens.dir/compiler_depend.make

# Include the progress variables for this target.
include CMakeFiles/aiStevens.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/aiStevens.dir/flags.make

CMakeFiles/aiStevens.dir/Compiler/Main.c.o: CMakeFiles/aiStevens.dir/flags.make
CMakeFiles/aiStevens.dir/Compiler/Main.c.o: /mnt/c/Users/jewoo/CLionProjects/aiStevens/Compiler/Main.c
CMakeFiles/aiStevens.dir/Compiler/Main.c.o: CMakeFiles/aiStevens.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/mnt/c/Users/jewoo/CLionProjects/aiStevens/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building C object CMakeFiles/aiStevens.dir/Compiler/Main.c.o"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -MD -MT CMakeFiles/aiStevens.dir/Compiler/Main.c.o -MF CMakeFiles/aiStevens.dir/Compiler/Main.c.o.d -o CMakeFiles/aiStevens.dir/Compiler/Main.c.o -c /mnt/c/Users/jewoo/CLionProjects/aiStevens/Compiler/Main.c

CMakeFiles/aiStevens.dir/Compiler/Main.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing C source to CMakeFiles/aiStevens.dir/Compiler/Main.c.i"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /mnt/c/Users/jewoo/CLionProjects/aiStevens/Compiler/Main.c > CMakeFiles/aiStevens.dir/Compiler/Main.c.i

CMakeFiles/aiStevens.dir/Compiler/Main.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling C source to assembly CMakeFiles/aiStevens.dir/Compiler/Main.c.s"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /mnt/c/Users/jewoo/CLionProjects/aiStevens/Compiler/Main.c -o CMakeFiles/aiStevens.dir/Compiler/Main.c.s

CMakeFiles/aiStevens.dir/Compiler/Scan.c.o: CMakeFiles/aiStevens.dir/flags.make
CMakeFiles/aiStevens.dir/Compiler/Scan.c.o: /mnt/c/Users/jewoo/CLionProjects/aiStevens/Compiler/Scan.c
CMakeFiles/aiStevens.dir/Compiler/Scan.c.o: CMakeFiles/aiStevens.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/mnt/c/Users/jewoo/CLionProjects/aiStevens/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building C object CMakeFiles/aiStevens.dir/Compiler/Scan.c.o"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -MD -MT CMakeFiles/aiStevens.dir/Compiler/Scan.c.o -MF CMakeFiles/aiStevens.dir/Compiler/Scan.c.o.d -o CMakeFiles/aiStevens.dir/Compiler/Scan.c.o -c /mnt/c/Users/jewoo/CLionProjects/aiStevens/Compiler/Scan.c

CMakeFiles/aiStevens.dir/Compiler/Scan.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing C source to CMakeFiles/aiStevens.dir/Compiler/Scan.c.i"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /mnt/c/Users/jewoo/CLionProjects/aiStevens/Compiler/Scan.c > CMakeFiles/aiStevens.dir/Compiler/Scan.c.i

CMakeFiles/aiStevens.dir/Compiler/Scan.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling C source to assembly CMakeFiles/aiStevens.dir/Compiler/Scan.c.s"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /mnt/c/Users/jewoo/CLionProjects/aiStevens/Compiler/Scan.c -o CMakeFiles/aiStevens.dir/Compiler/Scan.c.s

CMakeFiles/aiStevens.dir/Compiler/Expr.c.o: CMakeFiles/aiStevens.dir/flags.make
CMakeFiles/aiStevens.dir/Compiler/Expr.c.o: /mnt/c/Users/jewoo/CLionProjects/aiStevens/Compiler/Expr.c
CMakeFiles/aiStevens.dir/Compiler/Expr.c.o: CMakeFiles/aiStevens.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/mnt/c/Users/jewoo/CLionProjects/aiStevens/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building C object CMakeFiles/aiStevens.dir/Compiler/Expr.c.o"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -MD -MT CMakeFiles/aiStevens.dir/Compiler/Expr.c.o -MF CMakeFiles/aiStevens.dir/Compiler/Expr.c.o.d -o CMakeFiles/aiStevens.dir/Compiler/Expr.c.o -c /mnt/c/Users/jewoo/CLionProjects/aiStevens/Compiler/Expr.c

CMakeFiles/aiStevens.dir/Compiler/Expr.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing C source to CMakeFiles/aiStevens.dir/Compiler/Expr.c.i"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /mnt/c/Users/jewoo/CLionProjects/aiStevens/Compiler/Expr.c > CMakeFiles/aiStevens.dir/Compiler/Expr.c.i

CMakeFiles/aiStevens.dir/Compiler/Expr.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling C source to assembly CMakeFiles/aiStevens.dir/Compiler/Expr.c.s"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /mnt/c/Users/jewoo/CLionProjects/aiStevens/Compiler/Expr.c -o CMakeFiles/aiStevens.dir/Compiler/Expr.c.s

CMakeFiles/aiStevens.dir/Compiler/Tree.c.o: CMakeFiles/aiStevens.dir/flags.make
CMakeFiles/aiStevens.dir/Compiler/Tree.c.o: /mnt/c/Users/jewoo/CLionProjects/aiStevens/Compiler/Tree.c
CMakeFiles/aiStevens.dir/Compiler/Tree.c.o: CMakeFiles/aiStevens.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/mnt/c/Users/jewoo/CLionProjects/aiStevens/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Building C object CMakeFiles/aiStevens.dir/Compiler/Tree.c.o"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -MD -MT CMakeFiles/aiStevens.dir/Compiler/Tree.c.o -MF CMakeFiles/aiStevens.dir/Compiler/Tree.c.o.d -o CMakeFiles/aiStevens.dir/Compiler/Tree.c.o -c /mnt/c/Users/jewoo/CLionProjects/aiStevens/Compiler/Tree.c

CMakeFiles/aiStevens.dir/Compiler/Tree.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing C source to CMakeFiles/aiStevens.dir/Compiler/Tree.c.i"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /mnt/c/Users/jewoo/CLionProjects/aiStevens/Compiler/Tree.c > CMakeFiles/aiStevens.dir/Compiler/Tree.c.i

CMakeFiles/aiStevens.dir/Compiler/Tree.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling C source to assembly CMakeFiles/aiStevens.dir/Compiler/Tree.c.s"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /mnt/c/Users/jewoo/CLionProjects/aiStevens/Compiler/Tree.c -o CMakeFiles/aiStevens.dir/Compiler/Tree.c.s

CMakeFiles/aiStevens.dir/Compiler/Cg_x86_64.c.o: CMakeFiles/aiStevens.dir/flags.make
CMakeFiles/aiStevens.dir/Compiler/Cg_x86_64.c.o: /mnt/c/Users/jewoo/CLionProjects/aiStevens/Compiler/Cg_x86_64.c
CMakeFiles/aiStevens.dir/Compiler/Cg_x86_64.c.o: CMakeFiles/aiStevens.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/mnt/c/Users/jewoo/CLionProjects/aiStevens/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_5) "Building C object CMakeFiles/aiStevens.dir/Compiler/Cg_x86_64.c.o"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -MD -MT CMakeFiles/aiStevens.dir/Compiler/Cg_x86_64.c.o -MF CMakeFiles/aiStevens.dir/Compiler/Cg_x86_64.c.o.d -o CMakeFiles/aiStevens.dir/Compiler/Cg_x86_64.c.o -c /mnt/c/Users/jewoo/CLionProjects/aiStevens/Compiler/Cg_x86_64.c

CMakeFiles/aiStevens.dir/Compiler/Cg_x86_64.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing C source to CMakeFiles/aiStevens.dir/Compiler/Cg_x86_64.c.i"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /mnt/c/Users/jewoo/CLionProjects/aiStevens/Compiler/Cg_x86_64.c > CMakeFiles/aiStevens.dir/Compiler/Cg_x86_64.c.i

CMakeFiles/aiStevens.dir/Compiler/Cg_x86_64.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling C source to assembly CMakeFiles/aiStevens.dir/Compiler/Cg_x86_64.c.s"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /mnt/c/Users/jewoo/CLionProjects/aiStevens/Compiler/Cg_x86_64.c -o CMakeFiles/aiStevens.dir/Compiler/Cg_x86_64.c.s

CMakeFiles/aiStevens.dir/Compiler/Gen.c.o: CMakeFiles/aiStevens.dir/flags.make
CMakeFiles/aiStevens.dir/Compiler/Gen.c.o: /mnt/c/Users/jewoo/CLionProjects/aiStevens/Compiler/Gen.c
CMakeFiles/aiStevens.dir/Compiler/Gen.c.o: CMakeFiles/aiStevens.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/mnt/c/Users/jewoo/CLionProjects/aiStevens/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_6) "Building C object CMakeFiles/aiStevens.dir/Compiler/Gen.c.o"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -MD -MT CMakeFiles/aiStevens.dir/Compiler/Gen.c.o -MF CMakeFiles/aiStevens.dir/Compiler/Gen.c.o.d -o CMakeFiles/aiStevens.dir/Compiler/Gen.c.o -c /mnt/c/Users/jewoo/CLionProjects/aiStevens/Compiler/Gen.c

CMakeFiles/aiStevens.dir/Compiler/Gen.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing C source to CMakeFiles/aiStevens.dir/Compiler/Gen.c.i"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /mnt/c/Users/jewoo/CLionProjects/aiStevens/Compiler/Gen.c > CMakeFiles/aiStevens.dir/Compiler/Gen.c.i

CMakeFiles/aiStevens.dir/Compiler/Gen.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling C source to assembly CMakeFiles/aiStevens.dir/Compiler/Gen.c.s"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /mnt/c/Users/jewoo/CLionProjects/aiStevens/Compiler/Gen.c -o CMakeFiles/aiStevens.dir/Compiler/Gen.c.s

CMakeFiles/aiStevens.dir/Compiler/Stmt.c.o: CMakeFiles/aiStevens.dir/flags.make
CMakeFiles/aiStevens.dir/Compiler/Stmt.c.o: /mnt/c/Users/jewoo/CLionProjects/aiStevens/Compiler/Stmt.c
CMakeFiles/aiStevens.dir/Compiler/Stmt.c.o: CMakeFiles/aiStevens.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/mnt/c/Users/jewoo/CLionProjects/aiStevens/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_7) "Building C object CMakeFiles/aiStevens.dir/Compiler/Stmt.c.o"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -MD -MT CMakeFiles/aiStevens.dir/Compiler/Stmt.c.o -MF CMakeFiles/aiStevens.dir/Compiler/Stmt.c.o.d -o CMakeFiles/aiStevens.dir/Compiler/Stmt.c.o -c /mnt/c/Users/jewoo/CLionProjects/aiStevens/Compiler/Stmt.c

CMakeFiles/aiStevens.dir/Compiler/Stmt.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing C source to CMakeFiles/aiStevens.dir/Compiler/Stmt.c.i"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /mnt/c/Users/jewoo/CLionProjects/aiStevens/Compiler/Stmt.c > CMakeFiles/aiStevens.dir/Compiler/Stmt.c.i

CMakeFiles/aiStevens.dir/Compiler/Stmt.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling C source to assembly CMakeFiles/aiStevens.dir/Compiler/Stmt.c.s"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /mnt/c/Users/jewoo/CLionProjects/aiStevens/Compiler/Stmt.c -o CMakeFiles/aiStevens.dir/Compiler/Stmt.c.s

CMakeFiles/aiStevens.dir/Compiler/Misc.c.o: CMakeFiles/aiStevens.dir/flags.make
CMakeFiles/aiStevens.dir/Compiler/Misc.c.o: /mnt/c/Users/jewoo/CLionProjects/aiStevens/Compiler/Misc.c
CMakeFiles/aiStevens.dir/Compiler/Misc.c.o: CMakeFiles/aiStevens.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/mnt/c/Users/jewoo/CLionProjects/aiStevens/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_8) "Building C object CMakeFiles/aiStevens.dir/Compiler/Misc.c.o"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -MD -MT CMakeFiles/aiStevens.dir/Compiler/Misc.c.o -MF CMakeFiles/aiStevens.dir/Compiler/Misc.c.o.d -o CMakeFiles/aiStevens.dir/Compiler/Misc.c.o -c /mnt/c/Users/jewoo/CLionProjects/aiStevens/Compiler/Misc.c

CMakeFiles/aiStevens.dir/Compiler/Misc.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing C source to CMakeFiles/aiStevens.dir/Compiler/Misc.c.i"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /mnt/c/Users/jewoo/CLionProjects/aiStevens/Compiler/Misc.c > CMakeFiles/aiStevens.dir/Compiler/Misc.c.i

CMakeFiles/aiStevens.dir/Compiler/Misc.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling C source to assembly CMakeFiles/aiStevens.dir/Compiler/Misc.c.s"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /mnt/c/Users/jewoo/CLionProjects/aiStevens/Compiler/Misc.c -o CMakeFiles/aiStevens.dir/Compiler/Misc.c.s

# Object files for target aiStevens
aiStevens_OBJECTS = \
"CMakeFiles/aiStevens.dir/Compiler/Main.c.o" \
"CMakeFiles/aiStevens.dir/Compiler/Scan.c.o" \
"CMakeFiles/aiStevens.dir/Compiler/Expr.c.o" \
"CMakeFiles/aiStevens.dir/Compiler/Tree.c.o" \
"CMakeFiles/aiStevens.dir/Compiler/Cg_x86_64.c.o" \
"CMakeFiles/aiStevens.dir/Compiler/Gen.c.o" \
"CMakeFiles/aiStevens.dir/Compiler/Stmt.c.o" \
"CMakeFiles/aiStevens.dir/Compiler/Misc.c.o"

# External object files for target aiStevens
aiStevens_EXTERNAL_OBJECTS =

aiStevens: CMakeFiles/aiStevens.dir/Compiler/Main.c.o
aiStevens: CMakeFiles/aiStevens.dir/Compiler/Scan.c.o
aiStevens: CMakeFiles/aiStevens.dir/Compiler/Expr.c.o
aiStevens: CMakeFiles/aiStevens.dir/Compiler/Tree.c.o
aiStevens: CMakeFiles/aiStevens.dir/Compiler/Cg_x86_64.c.o
aiStevens: CMakeFiles/aiStevens.dir/Compiler/Gen.c.o
aiStevens: CMakeFiles/aiStevens.dir/Compiler/Stmt.c.o
aiStevens: CMakeFiles/aiStevens.dir/Compiler/Misc.c.o
aiStevens: CMakeFiles/aiStevens.dir/build.make
aiStevens: CMakeFiles/aiStevens.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --bold --progress-dir=/mnt/c/Users/jewoo/CLionProjects/aiStevens/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_9) "Linking C executable aiStevens"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/aiStevens.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/aiStevens.dir/build: aiStevens
.PHONY : CMakeFiles/aiStevens.dir/build

CMakeFiles/aiStevens.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/aiStevens.dir/cmake_clean.cmake
.PHONY : CMakeFiles/aiStevens.dir/clean

CMakeFiles/aiStevens.dir/depend:
	cd /mnt/c/Users/jewoo/CLionProjects/aiStevens/cmake-build-debug && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /mnt/c/Users/jewoo/CLionProjects/aiStevens /mnt/c/Users/jewoo/CLionProjects/aiStevens /mnt/c/Users/jewoo/CLionProjects/aiStevens/cmake-build-debug /mnt/c/Users/jewoo/CLionProjects/aiStevens/cmake-build-debug /mnt/c/Users/jewoo/CLionProjects/aiStevens/cmake-build-debug/CMakeFiles/aiStevens.dir/DependInfo.cmake "--color=$(COLOR)"
.PHONY : CMakeFiles/aiStevens.dir/depend

