# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.16

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:


#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:


# Remove some rules from gmake that .SUFFIXES does not remove.
SUFFIXES =

.SUFFIXES: .hpux_make_needs_suffix_list


# Suppress display of executed commands.
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
RM = /usr/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/corentin/Documents/sepc/ensimag-shell

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/corentin/Documents/sepc/ensimag-shell/build

# Include any dependencies generated for this target.
include CMakeFiles/ensishell.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/ensishell.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/ensishell.dir/flags.make

CMakeFiles/ensishell.dir/src/readcmd.c.o: CMakeFiles/ensishell.dir/flags.make
CMakeFiles/ensishell.dir/src/readcmd.c.o: ../src/readcmd.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/corentin/Documents/sepc/ensimag-shell/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building C object CMakeFiles/ensishell.dir/src/readcmd.c.o"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/ensishell.dir/src/readcmd.c.o   -c /home/corentin/Documents/sepc/ensimag-shell/src/readcmd.c

CMakeFiles/ensishell.dir/src/readcmd.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/ensishell.dir/src/readcmd.c.i"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/corentin/Documents/sepc/ensimag-shell/src/readcmd.c > CMakeFiles/ensishell.dir/src/readcmd.c.i

CMakeFiles/ensishell.dir/src/readcmd.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/ensishell.dir/src/readcmd.c.s"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/corentin/Documents/sepc/ensimag-shell/src/readcmd.c -o CMakeFiles/ensishell.dir/src/readcmd.c.s

CMakeFiles/ensishell.dir/src/ensishell.c.o: CMakeFiles/ensishell.dir/flags.make
CMakeFiles/ensishell.dir/src/ensishell.c.o: ../src/ensishell.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/corentin/Documents/sepc/ensimag-shell/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building C object CMakeFiles/ensishell.dir/src/ensishell.c.o"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/ensishell.dir/src/ensishell.c.o   -c /home/corentin/Documents/sepc/ensimag-shell/src/ensishell.c

CMakeFiles/ensishell.dir/src/ensishell.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/ensishell.dir/src/ensishell.c.i"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/corentin/Documents/sepc/ensimag-shell/src/ensishell.c > CMakeFiles/ensishell.dir/src/ensishell.c.i

CMakeFiles/ensishell.dir/src/ensishell.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/ensishell.dir/src/ensishell.c.s"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/corentin/Documents/sepc/ensimag-shell/src/ensishell.c -o CMakeFiles/ensishell.dir/src/ensishell.c.s

# Object files for target ensishell
ensishell_OBJECTS = \
"CMakeFiles/ensishell.dir/src/readcmd.c.o" \
"CMakeFiles/ensishell.dir/src/ensishell.c.o"

# External object files for target ensishell
ensishell_EXTERNAL_OBJECTS =

ensishell: CMakeFiles/ensishell.dir/src/readcmd.c.o
ensishell: CMakeFiles/ensishell.dir/src/ensishell.c.o
ensishell: CMakeFiles/ensishell.dir/build.make
ensishell: CMakeFiles/ensishell.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/corentin/Documents/sepc/ensimag-shell/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Linking C executable ensishell"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/ensishell.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/ensishell.dir/build: ensishell

.PHONY : CMakeFiles/ensishell.dir/build

CMakeFiles/ensishell.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/ensishell.dir/cmake_clean.cmake
.PHONY : CMakeFiles/ensishell.dir/clean

CMakeFiles/ensishell.dir/depend:
	cd /home/corentin/Documents/sepc/ensimag-shell/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/corentin/Documents/sepc/ensimag-shell /home/corentin/Documents/sepc/ensimag-shell /home/corentin/Documents/sepc/ensimag-shell/build /home/corentin/Documents/sepc/ensimag-shell/build /home/corentin/Documents/sepc/ensimag-shell/build/CMakeFiles/ensishell.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/ensishell.dir/depend

