# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.17

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
CMAKE_COMMAND = /home/ubuntu/clion-2020.1.1/bin/cmake/linux/bin/cmake

# The command to remove a file.
RM = /home/ubuntu/clion-2020.1.1/bin/cmake/linux/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/ubuntu/Scrivania/esercitazione4

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/ubuntu/Scrivania/esercitazione4/cmake-build-debug

# Include any dependencies generated for this target.
include CMakeFiles/esercitazione4.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/esercitazione4.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/esercitazione4.dir/flags.make

CMakeFiles/esercitazione4.dir/main.cpp.o: CMakeFiles/esercitazione4.dir/flags.make
CMakeFiles/esercitazione4.dir/main.cpp.o: ../main.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/ubuntu/Scrivania/esercitazione4/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/esercitazione4.dir/main.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/esercitazione4.dir/main.cpp.o -c /home/ubuntu/Scrivania/esercitazione4/main.cpp

CMakeFiles/esercitazione4.dir/main.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/esercitazione4.dir/main.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/ubuntu/Scrivania/esercitazione4/main.cpp > CMakeFiles/esercitazione4.dir/main.cpp.i

CMakeFiles/esercitazione4.dir/main.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/esercitazione4.dir/main.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/ubuntu/Scrivania/esercitazione4/main.cpp -o CMakeFiles/esercitazione4.dir/main.cpp.s

# Object files for target esercitazione4
esercitazione4_OBJECTS = \
"CMakeFiles/esercitazione4.dir/main.cpp.o"

# External object files for target esercitazione4
esercitazione4_EXTERNAL_OBJECTS =

esercitazione4: CMakeFiles/esercitazione4.dir/main.cpp.o
esercitazione4: CMakeFiles/esercitazione4.dir/build.make
esercitazione4: CMakeFiles/esercitazione4.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/ubuntu/Scrivania/esercitazione4/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable esercitazione4"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/esercitazione4.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/esercitazione4.dir/build: esercitazione4

.PHONY : CMakeFiles/esercitazione4.dir/build

CMakeFiles/esercitazione4.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/esercitazione4.dir/cmake_clean.cmake
.PHONY : CMakeFiles/esercitazione4.dir/clean

CMakeFiles/esercitazione4.dir/depend:
	cd /home/ubuntu/Scrivania/esercitazione4/cmake-build-debug && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/ubuntu/Scrivania/esercitazione4 /home/ubuntu/Scrivania/esercitazione4 /home/ubuntu/Scrivania/esercitazione4/cmake-build-debug /home/ubuntu/Scrivania/esercitazione4/cmake-build-debug /home/ubuntu/Scrivania/esercitazione4/cmake-build-debug/CMakeFiles/esercitazione4.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/esercitazione4.dir/depend

