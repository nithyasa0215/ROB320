# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 4.0

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
CMAKE_COMMAND = /opt/homebrew/bin/cmake

# The command to remove a file.
RM = /opt/homebrew/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /Users/nithyasamba/Desktop/Rob-320/Project1

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /Users/nithyasamba/Desktop/Rob-320/Project1/debug

# Include any dependencies generated for this target.
include CMakeFiles/cmd_receiver.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include CMakeFiles/cmd_receiver.dir/compiler_depend.make

# Include the progress variables for this target.
include CMakeFiles/cmd_receiver.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/cmd_receiver.dir/flags.make

CMakeFiles/cmd_receiver.dir/codegen:
.PHONY : CMakeFiles/cmd_receiver.dir/codegen

CMakeFiles/cmd_receiver.dir/src/cmd_receiver.c.o: CMakeFiles/cmd_receiver.dir/flags.make
CMakeFiles/cmd_receiver.dir/src/cmd_receiver.c.o: /Users/nithyasamba/Desktop/Rob-320/Project1/src/cmd_receiver.c
CMakeFiles/cmd_receiver.dir/src/cmd_receiver.c.o: CMakeFiles/cmd_receiver.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/Users/nithyasamba/Desktop/Rob-320/Project1/debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building C object CMakeFiles/cmd_receiver.dir/src/cmd_receiver.c.o"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -MD -MT CMakeFiles/cmd_receiver.dir/src/cmd_receiver.c.o -MF CMakeFiles/cmd_receiver.dir/src/cmd_receiver.c.o.d -o CMakeFiles/cmd_receiver.dir/src/cmd_receiver.c.o -c /Users/nithyasamba/Desktop/Rob-320/Project1/src/cmd_receiver.c

CMakeFiles/cmd_receiver.dir/src/cmd_receiver.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing C source to CMakeFiles/cmd_receiver.dir/src/cmd_receiver.c.i"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /Users/nithyasamba/Desktop/Rob-320/Project1/src/cmd_receiver.c > CMakeFiles/cmd_receiver.dir/src/cmd_receiver.c.i

CMakeFiles/cmd_receiver.dir/src/cmd_receiver.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling C source to assembly CMakeFiles/cmd_receiver.dir/src/cmd_receiver.c.s"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /Users/nithyasamba/Desktop/Rob-320/Project1/src/cmd_receiver.c -o CMakeFiles/cmd_receiver.dir/src/cmd_receiver.c.s

CMakeFiles/cmd_receiver.dir/src/util.c.o: CMakeFiles/cmd_receiver.dir/flags.make
CMakeFiles/cmd_receiver.dir/src/util.c.o: /Users/nithyasamba/Desktop/Rob-320/Project1/src/util.c
CMakeFiles/cmd_receiver.dir/src/util.c.o: CMakeFiles/cmd_receiver.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/Users/nithyasamba/Desktop/Rob-320/Project1/debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building C object CMakeFiles/cmd_receiver.dir/src/util.c.o"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -MD -MT CMakeFiles/cmd_receiver.dir/src/util.c.o -MF CMakeFiles/cmd_receiver.dir/src/util.c.o.d -o CMakeFiles/cmd_receiver.dir/src/util.c.o -c /Users/nithyasamba/Desktop/Rob-320/Project1/src/util.c

CMakeFiles/cmd_receiver.dir/src/util.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing C source to CMakeFiles/cmd_receiver.dir/src/util.c.i"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /Users/nithyasamba/Desktop/Rob-320/Project1/src/util.c > CMakeFiles/cmd_receiver.dir/src/util.c.i

CMakeFiles/cmd_receiver.dir/src/util.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling C source to assembly CMakeFiles/cmd_receiver.dir/src/util.c.s"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /Users/nithyasamba/Desktop/Rob-320/Project1/src/util.c -o CMakeFiles/cmd_receiver.dir/src/util.c.s

# Object files for target cmd_receiver
cmd_receiver_OBJECTS = \
"CMakeFiles/cmd_receiver.dir/src/cmd_receiver.c.o" \
"CMakeFiles/cmd_receiver.dir/src/util.c.o"

# External object files for target cmd_receiver
cmd_receiver_EXTERNAL_OBJECTS =

cmd_receiver: CMakeFiles/cmd_receiver.dir/src/cmd_receiver.c.o
cmd_receiver: CMakeFiles/cmd_receiver.dir/src/util.c.o
cmd_receiver: CMakeFiles/cmd_receiver.dir/build.make
cmd_receiver: librobot.a
cmd_receiver: CMakeFiles/cmd_receiver.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --bold --progress-dir=/Users/nithyasamba/Desktop/Rob-320/Project1/debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Linking C executable cmd_receiver"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/cmd_receiver.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/cmd_receiver.dir/build: cmd_receiver
.PHONY : CMakeFiles/cmd_receiver.dir/build

CMakeFiles/cmd_receiver.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/cmd_receiver.dir/cmake_clean.cmake
.PHONY : CMakeFiles/cmd_receiver.dir/clean

CMakeFiles/cmd_receiver.dir/depend:
	cd /Users/nithyasamba/Desktop/Rob-320/Project1/debug && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /Users/nithyasamba/Desktop/Rob-320/Project1 /Users/nithyasamba/Desktop/Rob-320/Project1 /Users/nithyasamba/Desktop/Rob-320/Project1/debug /Users/nithyasamba/Desktop/Rob-320/Project1/debug /Users/nithyasamba/Desktop/Rob-320/Project1/debug/CMakeFiles/cmd_receiver.dir/DependInfo.cmake "--color=$(COLOR)"
.PHONY : CMakeFiles/cmd_receiver.dir/depend

