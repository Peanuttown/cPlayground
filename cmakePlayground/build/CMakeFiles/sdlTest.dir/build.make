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
include CMakeFiles/sdlTest.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/sdlTest.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/sdlTest.dir/flags.make

CMakeFiles/sdlTest.dir/main.o: CMakeFiles/sdlTest.dir/flags.make
CMakeFiles/sdlTest.dir/main.o: ../main.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/tzz/projects/cPlayground/cmakePlayground/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building C object CMakeFiles/sdlTest.dir/main.o"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/sdlTest.dir/main.o   -c /home/tzz/projects/cPlayground/cmakePlayground/main.c

CMakeFiles/sdlTest.dir/main.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/sdlTest.dir/main.i"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/tzz/projects/cPlayground/cmakePlayground/main.c > CMakeFiles/sdlTest.dir/main.i

CMakeFiles/sdlTest.dir/main.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/sdlTest.dir/main.s"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/tzz/projects/cPlayground/cmakePlayground/main.c -o CMakeFiles/sdlTest.dir/main.s

CMakeFiles/sdlTest.dir/main.o.requires:

.PHONY : CMakeFiles/sdlTest.dir/main.o.requires

CMakeFiles/sdlTest.dir/main.o.provides: CMakeFiles/sdlTest.dir/main.o.requires
	$(MAKE) -f CMakeFiles/sdlTest.dir/build.make CMakeFiles/sdlTest.dir/main.o.provides.build
.PHONY : CMakeFiles/sdlTest.dir/main.o.provides

CMakeFiles/sdlTest.dir/main.o.provides.build: CMakeFiles/sdlTest.dir/main.o


# Object files for target sdlTest
sdlTest_OBJECTS = \
"CMakeFiles/sdlTest.dir/main.o"

# External object files for target sdlTest
sdlTest_EXTERNAL_OBJECTS =

sdlTest: CMakeFiles/sdlTest.dir/main.o
sdlTest: CMakeFiles/sdlTest.dir/build.make
sdlTest: libtest/libtestTzz.so
sdlTest: libtest/libmod2/libmod2.a
sdlTest: CMakeFiles/sdlTest.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/tzz/projects/cPlayground/cmakePlayground/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking C executable sdlTest"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/sdlTest.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/sdlTest.dir/build: sdlTest

.PHONY : CMakeFiles/sdlTest.dir/build

CMakeFiles/sdlTest.dir/requires: CMakeFiles/sdlTest.dir/main.o.requires

.PHONY : CMakeFiles/sdlTest.dir/requires

CMakeFiles/sdlTest.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/sdlTest.dir/cmake_clean.cmake
.PHONY : CMakeFiles/sdlTest.dir/clean

CMakeFiles/sdlTest.dir/depend:
	cd /home/tzz/projects/cPlayground/cmakePlayground/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/tzz/projects/cPlayground/cmakePlayground /home/tzz/projects/cPlayground/cmakePlayground /home/tzz/projects/cPlayground/cmakePlayground/build /home/tzz/projects/cPlayground/cmakePlayground/build /home/tzz/projects/cPlayground/cmakePlayground/build/CMakeFiles/sdlTest.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/sdlTest.dir/depend
