# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.22

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
CMAKE_SOURCE_DIR = /home/apolline/Documents/M1/C/Projet

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/apolline/Documents/M1/C/Projet/build

# Include any dependencies generated for this target.
include CMakeFiles/Projet.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include CMakeFiles/Projet.dir/compiler_depend.make

# Include the progress variables for this target.
include CMakeFiles/Projet.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/Projet.dir/flags.make

CMakeFiles/Projet.dir/src/main.cpp.o: CMakeFiles/Projet.dir/flags.make
CMakeFiles/Projet.dir/src/main.cpp.o: ../src/main.cpp
CMakeFiles/Projet.dir/src/main.cpp.o: CMakeFiles/Projet.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/apolline/Documents/M1/C/Projet/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/Projet.dir/src/main.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/Projet.dir/src/main.cpp.o -MF CMakeFiles/Projet.dir/src/main.cpp.o.d -o CMakeFiles/Projet.dir/src/main.cpp.o -c /home/apolline/Documents/M1/C/Projet/src/main.cpp

CMakeFiles/Projet.dir/src/main.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/Projet.dir/src/main.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/apolline/Documents/M1/C/Projet/src/main.cpp > CMakeFiles/Projet.dir/src/main.cpp.i

CMakeFiles/Projet.dir/src/main.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/Projet.dir/src/main.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/apolline/Documents/M1/C/Projet/src/main.cpp -o CMakeFiles/Projet.dir/src/main.cpp.s

# Object files for target Projet
Projet_OBJECTS = \
"CMakeFiles/Projet.dir/src/main.cpp.o"

# External object files for target Projet
Projet_EXTERNAL_OBJECTS =

Projet: CMakeFiles/Projet.dir/src/main.cpp.o
Projet: CMakeFiles/Projet.dir/build.make
Projet: CMakeFiles/Projet.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/apolline/Documents/M1/C/Projet/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable Projet"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/Projet.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/Projet.dir/build: Projet
.PHONY : CMakeFiles/Projet.dir/build

CMakeFiles/Projet.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/Projet.dir/cmake_clean.cmake
.PHONY : CMakeFiles/Projet.dir/clean

CMakeFiles/Projet.dir/depend:
	cd /home/apolline/Documents/M1/C/Projet/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/apolline/Documents/M1/C/Projet /home/apolline/Documents/M1/C/Projet /home/apolline/Documents/M1/C/Projet/build /home/apolline/Documents/M1/C/Projet/build /home/apolline/Documents/M1/C/Projet/build/CMakeFiles/Projet.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/Projet.dir/depend
