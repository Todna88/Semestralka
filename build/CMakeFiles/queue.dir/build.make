# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.25

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
CMAKE_COMMAND = /usr/bin/cmake

# The command to remove a file.
RM = /usr/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /mnt/d/Škola/FAV/ZS_02/pc/Semestralka

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /mnt/d/Škola/FAV/ZS_02/pc/Semestralka/build

# Include any dependencies generated for this target.
include CMakeFiles/queue.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include CMakeFiles/queue.dir/compiler_depend.make

# Include the progress variables for this target.
include CMakeFiles/queue.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/queue.dir/flags.make

CMakeFiles/queue.dir/src/queue/queue_check.c.o: CMakeFiles/queue.dir/flags.make
CMakeFiles/queue.dir/src/queue/queue_check.c.o: /mnt/d/Škola/FAV/ZS_02/pc/Semestralka/src/queue/queue_check.c
CMakeFiles/queue.dir/src/queue/queue_check.c.o: CMakeFiles/queue.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/mnt/d/Škola/FAV/ZS_02/pc/Semestralka/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building C object CMakeFiles/queue.dir/src/queue/queue_check.c.o"
	/usr/bin/gcc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -MD -MT CMakeFiles/queue.dir/src/queue/queue_check.c.o -MF CMakeFiles/queue.dir/src/queue/queue_check.c.o.d -o CMakeFiles/queue.dir/src/queue/queue_check.c.o -c /mnt/d/Škola/FAV/ZS_02/pc/Semestralka/src/queue/queue_check.c

CMakeFiles/queue.dir/src/queue/queue_check.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/queue.dir/src/queue/queue_check.c.i"
	/usr/bin/gcc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /mnt/d/Škola/FAV/ZS_02/pc/Semestralka/src/queue/queue_check.c > CMakeFiles/queue.dir/src/queue/queue_check.c.i

CMakeFiles/queue.dir/src/queue/queue_check.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/queue.dir/src/queue/queue_check.c.s"
	/usr/bin/gcc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /mnt/d/Škola/FAV/ZS_02/pc/Semestralka/src/queue/queue_check.c -o CMakeFiles/queue.dir/src/queue/queue_check.c.s

CMakeFiles/queue.dir/src/queue/queue.c.o: CMakeFiles/queue.dir/flags.make
CMakeFiles/queue.dir/src/queue/queue.c.o: /mnt/d/Škola/FAV/ZS_02/pc/Semestralka/src/queue/queue.c
CMakeFiles/queue.dir/src/queue/queue.c.o: CMakeFiles/queue.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/mnt/d/Škola/FAV/ZS_02/pc/Semestralka/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building C object CMakeFiles/queue.dir/src/queue/queue.c.o"
	/usr/bin/gcc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -MD -MT CMakeFiles/queue.dir/src/queue/queue.c.o -MF CMakeFiles/queue.dir/src/queue/queue.c.o.d -o CMakeFiles/queue.dir/src/queue/queue.c.o -c /mnt/d/Škola/FAV/ZS_02/pc/Semestralka/src/queue/queue.c

CMakeFiles/queue.dir/src/queue/queue.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/queue.dir/src/queue/queue.c.i"
	/usr/bin/gcc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /mnt/d/Škola/FAV/ZS_02/pc/Semestralka/src/queue/queue.c > CMakeFiles/queue.dir/src/queue/queue.c.i

CMakeFiles/queue.dir/src/queue/queue.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/queue.dir/src/queue/queue.c.s"
	/usr/bin/gcc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /mnt/d/Škola/FAV/ZS_02/pc/Semestralka/src/queue/queue.c -o CMakeFiles/queue.dir/src/queue/queue.c.s

CMakeFiles/queue.dir/src/node/node.c.o: CMakeFiles/queue.dir/flags.make
CMakeFiles/queue.dir/src/node/node.c.o: /mnt/d/Škola/FAV/ZS_02/pc/Semestralka/src/node/node.c
CMakeFiles/queue.dir/src/node/node.c.o: CMakeFiles/queue.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/mnt/d/Škola/FAV/ZS_02/pc/Semestralka/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building C object CMakeFiles/queue.dir/src/node/node.c.o"
	/usr/bin/gcc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -MD -MT CMakeFiles/queue.dir/src/node/node.c.o -MF CMakeFiles/queue.dir/src/node/node.c.o.d -o CMakeFiles/queue.dir/src/node/node.c.o -c /mnt/d/Škola/FAV/ZS_02/pc/Semestralka/src/node/node.c

CMakeFiles/queue.dir/src/node/node.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/queue.dir/src/node/node.c.i"
	/usr/bin/gcc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /mnt/d/Škola/FAV/ZS_02/pc/Semestralka/src/node/node.c > CMakeFiles/queue.dir/src/node/node.c.i

CMakeFiles/queue.dir/src/node/node.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/queue.dir/src/node/node.c.s"
	/usr/bin/gcc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /mnt/d/Škola/FAV/ZS_02/pc/Semestralka/src/node/node.c -o CMakeFiles/queue.dir/src/node/node.c.s

# Object files for target queue
queue_OBJECTS = \
"CMakeFiles/queue.dir/src/queue/queue_check.c.o" \
"CMakeFiles/queue.dir/src/queue/queue.c.o" \
"CMakeFiles/queue.dir/src/node/node.c.o"

# External object files for target queue
queue_EXTERNAL_OBJECTS =

queue: CMakeFiles/queue.dir/src/queue/queue_check.c.o
queue: CMakeFiles/queue.dir/src/queue/queue.c.o
queue: CMakeFiles/queue.dir/src/node/node.c.o
queue: CMakeFiles/queue.dir/build.make
queue: CMakeFiles/queue.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/mnt/d/Škola/FAV/ZS_02/pc/Semestralka/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Linking C executable queue"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/queue.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/queue.dir/build: queue
.PHONY : CMakeFiles/queue.dir/build

CMakeFiles/queue.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/queue.dir/cmake_clean.cmake
.PHONY : CMakeFiles/queue.dir/clean

CMakeFiles/queue.dir/depend:
	cd /mnt/d/Škola/FAV/ZS_02/pc/Semestralka/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /mnt/d/Škola/FAV/ZS_02/pc/Semestralka /mnt/d/Škola/FAV/ZS_02/pc/Semestralka /mnt/d/Škola/FAV/ZS_02/pc/Semestralka/build /mnt/d/Škola/FAV/ZS_02/pc/Semestralka/build /mnt/d/Škola/FAV/ZS_02/pc/Semestralka/build/CMakeFiles/queue.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/queue.dir/depend

