# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.10

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
CMAKE_SOURCE_DIR = /home/tzz/projects/cPlayground/cmakePlayground

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/tzz/projects/cPlayground/cmakePlayground/build

# Include any dependencies generated for this target.
include CMakeFiles/tzz.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/tzz.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/tzz.dir/flags.make

CMakeFiles/tzz.dir/main.o: CMakeFiles/tzz.dir/flags.make
CMakeFiles/tzz.dir/main.o: ../main.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/tzz/projects/cPlayground/cmakePlayground/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building C object CMakeFiles/tzz.dir/main.o"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/tzz.dir/main.o   -c /home/tzz/projects/cPlayground/cmakePlayground/main.c

CMakeFiles/tzz.dir/main.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/tzz.dir/main.i"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/tzz/projects/cPlayground/cmakePlayground/main.c > CMakeFiles/tzz.dir/main.i

CMakeFiles/tzz.dir/main.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/tzz.dir/main.s"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/tzz/projects/cPlayground/cmakePlayground/main.c -o CMakeFiles/tzz.dir/main.s

CMakeFiles/tzz.dir/main.o.requires:

.PHONY : CMakeFiles/tzz.dir/main.o.requires

CMakeFiles/tzz.dir/main.o.provides: CMakeFiles/tzz.dir/main.o.requires
	$(MAKE) -f CMakeFiles/tzz.dir/build.make CMakeFiles/tzz.dir/main.o.provides.build
.PHONY : CMakeFiles/tzz.dir/main.o.provides

CMakeFiles/tzz.dir/main.o.provides.build: CMakeFiles/tzz.dir/main.o


# Object files for target tzz
tzz_OBJECTS = \
"CMakeFiles/tzz.dir/main.o"

# External object files for target tzz
tzz_EXTERNAL_OBJECTS =

tzz: CMakeFiles/tzz.dir/main.o
tzz: CMakeFiles/tzz.dir/build.make
tzz: mod1Dir/libtzzMod1.so
tzz: CMakeFiles/tzz.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/tzz/projects/cPlayground/cmakePlayground/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking C executable tzz"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/tzz.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/tzz.dir/build: tzz

.PHONY : CMakeFiles/tzz.dir/build

CMakeFiles/tzz.dir/requires: CMakeFiles/tzz.dir/main.o.requires

.PHONY : CMakeFiles/tzz.dir/requires

CMakeFiles/tzz.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/tzz.dir/cmake_clean.cmake
.PHONY : CMakeFiles/tzz.dir/clean

CMakeFiles/tzz.dir/depend:
	cd /home/tzz/projects/cPlayground/cmakePlayground/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/tzz/projects/cPlayground/cmakePlayground /home/tzz/projects/cPlayground/cmakePlayground /home/tzz/projects/cPlayground/cmakePlayground/build /home/tzz/projects/cPlayground/cmakePlayground/build /home/tzz/projects/cPlayground/cmakePlayground/build/CMakeFiles/tzz.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/tzz.dir/depend
