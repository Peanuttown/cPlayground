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
CMAKE_SOURCE_DIR = /home/tzz/projects/cPlayground/clox/lox

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/tzz/projects/cPlayground/clox/lox/build

# Include any dependencies generated for this target.
include CMakeFiles/lox.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/lox.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/lox.dir/flags.make

CMakeFiles/lox.dir/main.c.o: CMakeFiles/lox.dir/flags.make
CMakeFiles/lox.dir/main.c.o: ../main.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/tzz/projects/cPlayground/clox/lox/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building C object CMakeFiles/lox.dir/main.c.o"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/lox.dir/main.c.o   -c /home/tzz/projects/cPlayground/clox/lox/main.c

CMakeFiles/lox.dir/main.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/lox.dir/main.c.i"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/tzz/projects/cPlayground/clox/lox/main.c > CMakeFiles/lox.dir/main.c.i

CMakeFiles/lox.dir/main.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/lox.dir/main.c.s"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/tzz/projects/cPlayground/clox/lox/main.c -o CMakeFiles/lox.dir/main.c.s

CMakeFiles/lox.dir/main.c.o.requires:

.PHONY : CMakeFiles/lox.dir/main.c.o.requires

CMakeFiles/lox.dir/main.c.o.provides: CMakeFiles/lox.dir/main.c.o.requires
	$(MAKE) -f CMakeFiles/lox.dir/build.make CMakeFiles/lox.dir/main.c.o.provides.build
.PHONY : CMakeFiles/lox.dir/main.c.o.provides

CMakeFiles/lox.dir/main.c.o.provides.build: CMakeFiles/lox.dir/main.c.o


CMakeFiles/lox.dir/debug.c.o: CMakeFiles/lox.dir/flags.make
CMakeFiles/lox.dir/debug.c.o: ../debug.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/tzz/projects/cPlayground/clox/lox/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building C object CMakeFiles/lox.dir/debug.c.o"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/lox.dir/debug.c.o   -c /home/tzz/projects/cPlayground/clox/lox/debug.c

CMakeFiles/lox.dir/debug.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/lox.dir/debug.c.i"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/tzz/projects/cPlayground/clox/lox/debug.c > CMakeFiles/lox.dir/debug.c.i

CMakeFiles/lox.dir/debug.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/lox.dir/debug.c.s"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/tzz/projects/cPlayground/clox/lox/debug.c -o CMakeFiles/lox.dir/debug.c.s

CMakeFiles/lox.dir/debug.c.o.requires:

.PHONY : CMakeFiles/lox.dir/debug.c.o.requires

CMakeFiles/lox.dir/debug.c.o.provides: CMakeFiles/lox.dir/debug.c.o.requires
	$(MAKE) -f CMakeFiles/lox.dir/build.make CMakeFiles/lox.dir/debug.c.o.provides.build
.PHONY : CMakeFiles/lox.dir/debug.c.o.provides

CMakeFiles/lox.dir/debug.c.o.provides.build: CMakeFiles/lox.dir/debug.c.o


CMakeFiles/lox.dir/chunk.c.o: CMakeFiles/lox.dir/flags.make
CMakeFiles/lox.dir/chunk.c.o: ../chunk.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/tzz/projects/cPlayground/clox/lox/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building C object CMakeFiles/lox.dir/chunk.c.o"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/lox.dir/chunk.c.o   -c /home/tzz/projects/cPlayground/clox/lox/chunk.c

CMakeFiles/lox.dir/chunk.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/lox.dir/chunk.c.i"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/tzz/projects/cPlayground/clox/lox/chunk.c > CMakeFiles/lox.dir/chunk.c.i

CMakeFiles/lox.dir/chunk.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/lox.dir/chunk.c.s"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/tzz/projects/cPlayground/clox/lox/chunk.c -o CMakeFiles/lox.dir/chunk.c.s

CMakeFiles/lox.dir/chunk.c.o.requires:

.PHONY : CMakeFiles/lox.dir/chunk.c.o.requires

CMakeFiles/lox.dir/chunk.c.o.provides: CMakeFiles/lox.dir/chunk.c.o.requires
	$(MAKE) -f CMakeFiles/lox.dir/build.make CMakeFiles/lox.dir/chunk.c.o.provides.build
.PHONY : CMakeFiles/lox.dir/chunk.c.o.provides

CMakeFiles/lox.dir/chunk.c.o.provides.build: CMakeFiles/lox.dir/chunk.c.o


CMakeFiles/lox.dir/memory.c.o: CMakeFiles/lox.dir/flags.make
CMakeFiles/lox.dir/memory.c.o: ../memory.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/tzz/projects/cPlayground/clox/lox/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Building C object CMakeFiles/lox.dir/memory.c.o"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/lox.dir/memory.c.o   -c /home/tzz/projects/cPlayground/clox/lox/memory.c

CMakeFiles/lox.dir/memory.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/lox.dir/memory.c.i"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/tzz/projects/cPlayground/clox/lox/memory.c > CMakeFiles/lox.dir/memory.c.i

CMakeFiles/lox.dir/memory.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/lox.dir/memory.c.s"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/tzz/projects/cPlayground/clox/lox/memory.c -o CMakeFiles/lox.dir/memory.c.s

CMakeFiles/lox.dir/memory.c.o.requires:

.PHONY : CMakeFiles/lox.dir/memory.c.o.requires

CMakeFiles/lox.dir/memory.c.o.provides: CMakeFiles/lox.dir/memory.c.o.requires
	$(MAKE) -f CMakeFiles/lox.dir/build.make CMakeFiles/lox.dir/memory.c.o.provides.build
.PHONY : CMakeFiles/lox.dir/memory.c.o.provides

CMakeFiles/lox.dir/memory.c.o.provides.build: CMakeFiles/lox.dir/memory.c.o


CMakeFiles/lox.dir/value.c.o: CMakeFiles/lox.dir/flags.make
CMakeFiles/lox.dir/value.c.o: ../value.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/tzz/projects/cPlayground/clox/lox/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_5) "Building C object CMakeFiles/lox.dir/value.c.o"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/lox.dir/value.c.o   -c /home/tzz/projects/cPlayground/clox/lox/value.c

CMakeFiles/lox.dir/value.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/lox.dir/value.c.i"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/tzz/projects/cPlayground/clox/lox/value.c > CMakeFiles/lox.dir/value.c.i

CMakeFiles/lox.dir/value.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/lox.dir/value.c.s"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/tzz/projects/cPlayground/clox/lox/value.c -o CMakeFiles/lox.dir/value.c.s

CMakeFiles/lox.dir/value.c.o.requires:

.PHONY : CMakeFiles/lox.dir/value.c.o.requires

CMakeFiles/lox.dir/value.c.o.provides: CMakeFiles/lox.dir/value.c.o.requires
	$(MAKE) -f CMakeFiles/lox.dir/build.make CMakeFiles/lox.dir/value.c.o.provides.build
.PHONY : CMakeFiles/lox.dir/value.c.o.provides

CMakeFiles/lox.dir/value.c.o.provides.build: CMakeFiles/lox.dir/value.c.o


CMakeFiles/lox.dir/vm.c.o: CMakeFiles/lox.dir/flags.make
CMakeFiles/lox.dir/vm.c.o: ../vm.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/tzz/projects/cPlayground/clox/lox/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_6) "Building C object CMakeFiles/lox.dir/vm.c.o"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/lox.dir/vm.c.o   -c /home/tzz/projects/cPlayground/clox/lox/vm.c

CMakeFiles/lox.dir/vm.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/lox.dir/vm.c.i"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/tzz/projects/cPlayground/clox/lox/vm.c > CMakeFiles/lox.dir/vm.c.i

CMakeFiles/lox.dir/vm.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/lox.dir/vm.c.s"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/tzz/projects/cPlayground/clox/lox/vm.c -o CMakeFiles/lox.dir/vm.c.s

CMakeFiles/lox.dir/vm.c.o.requires:

.PHONY : CMakeFiles/lox.dir/vm.c.o.requires

CMakeFiles/lox.dir/vm.c.o.provides: CMakeFiles/lox.dir/vm.c.o.requires
	$(MAKE) -f CMakeFiles/lox.dir/build.make CMakeFiles/lox.dir/vm.c.o.provides.build
.PHONY : CMakeFiles/lox.dir/vm.c.o.provides

CMakeFiles/lox.dir/vm.c.o.provides.build: CMakeFiles/lox.dir/vm.c.o


CMakeFiles/lox.dir/compiler.c.o: CMakeFiles/lox.dir/flags.make
CMakeFiles/lox.dir/compiler.c.o: ../compiler.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/tzz/projects/cPlayground/clox/lox/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_7) "Building C object CMakeFiles/lox.dir/compiler.c.o"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/lox.dir/compiler.c.o   -c /home/tzz/projects/cPlayground/clox/lox/compiler.c

CMakeFiles/lox.dir/compiler.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/lox.dir/compiler.c.i"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/tzz/projects/cPlayground/clox/lox/compiler.c > CMakeFiles/lox.dir/compiler.c.i

CMakeFiles/lox.dir/compiler.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/lox.dir/compiler.c.s"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/tzz/projects/cPlayground/clox/lox/compiler.c -o CMakeFiles/lox.dir/compiler.c.s

CMakeFiles/lox.dir/compiler.c.o.requires:

.PHONY : CMakeFiles/lox.dir/compiler.c.o.requires

CMakeFiles/lox.dir/compiler.c.o.provides: CMakeFiles/lox.dir/compiler.c.o.requires
	$(MAKE) -f CMakeFiles/lox.dir/build.make CMakeFiles/lox.dir/compiler.c.o.provides.build
.PHONY : CMakeFiles/lox.dir/compiler.c.o.provides

CMakeFiles/lox.dir/compiler.c.o.provides.build: CMakeFiles/lox.dir/compiler.c.o


CMakeFiles/lox.dir/scanner.c.o: CMakeFiles/lox.dir/flags.make
CMakeFiles/lox.dir/scanner.c.o: ../scanner.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/tzz/projects/cPlayground/clox/lox/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_8) "Building C object CMakeFiles/lox.dir/scanner.c.o"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/lox.dir/scanner.c.o   -c /home/tzz/projects/cPlayground/clox/lox/scanner.c

CMakeFiles/lox.dir/scanner.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/lox.dir/scanner.c.i"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/tzz/projects/cPlayground/clox/lox/scanner.c > CMakeFiles/lox.dir/scanner.c.i

CMakeFiles/lox.dir/scanner.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/lox.dir/scanner.c.s"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/tzz/projects/cPlayground/clox/lox/scanner.c -o CMakeFiles/lox.dir/scanner.c.s

CMakeFiles/lox.dir/scanner.c.o.requires:

.PHONY : CMakeFiles/lox.dir/scanner.c.o.requires

CMakeFiles/lox.dir/scanner.c.o.provides: CMakeFiles/lox.dir/scanner.c.o.requires
	$(MAKE) -f CMakeFiles/lox.dir/build.make CMakeFiles/lox.dir/scanner.c.o.provides.build
.PHONY : CMakeFiles/lox.dir/scanner.c.o.provides

CMakeFiles/lox.dir/scanner.c.o.provides.build: CMakeFiles/lox.dir/scanner.c.o


CMakeFiles/lox.dir/table.c.o: CMakeFiles/lox.dir/flags.make
CMakeFiles/lox.dir/table.c.o: ../table.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/tzz/projects/cPlayground/clox/lox/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_9) "Building C object CMakeFiles/lox.dir/table.c.o"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/lox.dir/table.c.o   -c /home/tzz/projects/cPlayground/clox/lox/table.c

CMakeFiles/lox.dir/table.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/lox.dir/table.c.i"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/tzz/projects/cPlayground/clox/lox/table.c > CMakeFiles/lox.dir/table.c.i

CMakeFiles/lox.dir/table.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/lox.dir/table.c.s"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/tzz/projects/cPlayground/clox/lox/table.c -o CMakeFiles/lox.dir/table.c.s

CMakeFiles/lox.dir/table.c.o.requires:

.PHONY : CMakeFiles/lox.dir/table.c.o.requires

CMakeFiles/lox.dir/table.c.o.provides: CMakeFiles/lox.dir/table.c.o.requires
	$(MAKE) -f CMakeFiles/lox.dir/build.make CMakeFiles/lox.dir/table.c.o.provides.build
.PHONY : CMakeFiles/lox.dir/table.c.o.provides

CMakeFiles/lox.dir/table.c.o.provides.build: CMakeFiles/lox.dir/table.c.o


CMakeFiles/lox.dir/object.c.o: CMakeFiles/lox.dir/flags.make
CMakeFiles/lox.dir/object.c.o: ../object.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/tzz/projects/cPlayground/clox/lox/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_10) "Building C object CMakeFiles/lox.dir/object.c.o"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/lox.dir/object.c.o   -c /home/tzz/projects/cPlayground/clox/lox/object.c

CMakeFiles/lox.dir/object.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/lox.dir/object.c.i"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/tzz/projects/cPlayground/clox/lox/object.c > CMakeFiles/lox.dir/object.c.i

CMakeFiles/lox.dir/object.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/lox.dir/object.c.s"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/tzz/projects/cPlayground/clox/lox/object.c -o CMakeFiles/lox.dir/object.c.s

CMakeFiles/lox.dir/object.c.o.requires:

.PHONY : CMakeFiles/lox.dir/object.c.o.requires

CMakeFiles/lox.dir/object.c.o.provides: CMakeFiles/lox.dir/object.c.o.requires
	$(MAKE) -f CMakeFiles/lox.dir/build.make CMakeFiles/lox.dir/object.c.o.provides.build
.PHONY : CMakeFiles/lox.dir/object.c.o.provides

CMakeFiles/lox.dir/object.c.o.provides.build: CMakeFiles/lox.dir/object.c.o


# Object files for target lox
lox_OBJECTS = \
"CMakeFiles/lox.dir/main.c.o" \
"CMakeFiles/lox.dir/debug.c.o" \
"CMakeFiles/lox.dir/chunk.c.o" \
"CMakeFiles/lox.dir/memory.c.o" \
"CMakeFiles/lox.dir/value.c.o" \
"CMakeFiles/lox.dir/vm.c.o" \
"CMakeFiles/lox.dir/compiler.c.o" \
"CMakeFiles/lox.dir/scanner.c.o" \
"CMakeFiles/lox.dir/table.c.o" \
"CMakeFiles/lox.dir/object.c.o"

# External object files for target lox
lox_EXTERNAL_OBJECTS =

lox: CMakeFiles/lox.dir/main.c.o
lox: CMakeFiles/lox.dir/debug.c.o
lox: CMakeFiles/lox.dir/chunk.c.o
lox: CMakeFiles/lox.dir/memory.c.o
lox: CMakeFiles/lox.dir/value.c.o
lox: CMakeFiles/lox.dir/vm.c.o
lox: CMakeFiles/lox.dir/compiler.c.o
lox: CMakeFiles/lox.dir/scanner.c.o
lox: CMakeFiles/lox.dir/table.c.o
lox: CMakeFiles/lox.dir/object.c.o
lox: CMakeFiles/lox.dir/build.make
lox: CMakeFiles/lox.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/tzz/projects/cPlayground/clox/lox/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_11) "Linking C executable lox"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/lox.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/lox.dir/build: lox

.PHONY : CMakeFiles/lox.dir/build

CMakeFiles/lox.dir/requires: CMakeFiles/lox.dir/main.c.o.requires
CMakeFiles/lox.dir/requires: CMakeFiles/lox.dir/debug.c.o.requires
CMakeFiles/lox.dir/requires: CMakeFiles/lox.dir/chunk.c.o.requires
CMakeFiles/lox.dir/requires: CMakeFiles/lox.dir/memory.c.o.requires
CMakeFiles/lox.dir/requires: CMakeFiles/lox.dir/value.c.o.requires
CMakeFiles/lox.dir/requires: CMakeFiles/lox.dir/vm.c.o.requires
CMakeFiles/lox.dir/requires: CMakeFiles/lox.dir/compiler.c.o.requires
CMakeFiles/lox.dir/requires: CMakeFiles/lox.dir/scanner.c.o.requires
CMakeFiles/lox.dir/requires: CMakeFiles/lox.dir/table.c.o.requires
CMakeFiles/lox.dir/requires: CMakeFiles/lox.dir/object.c.o.requires

.PHONY : CMakeFiles/lox.dir/requires

CMakeFiles/lox.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/lox.dir/cmake_clean.cmake
.PHONY : CMakeFiles/lox.dir/clean

CMakeFiles/lox.dir/depend:
	cd /home/tzz/projects/cPlayground/clox/lox/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/tzz/projects/cPlayground/clox/lox /home/tzz/projects/cPlayground/clox/lox /home/tzz/projects/cPlayground/clox/lox/build /home/tzz/projects/cPlayground/clox/lox/build /home/tzz/projects/cPlayground/clox/lox/build/CMakeFiles/lox.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/lox.dir/depend

