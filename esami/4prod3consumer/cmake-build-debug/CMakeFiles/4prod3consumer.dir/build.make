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
CMAKE_COMMAND = /Applications/CLion.app/Contents/bin/cmake/mac/bin/cmake

# The command to remove a file.
RM = /Applications/CLion.app/Contents/bin/cmake/mac/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /Users/giuliodegiorgi/Documents/GitHub/lab_PdS_2020/esami/4prod3consumer

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /Users/giuliodegiorgi/Documents/GitHub/lab_PdS_2020/esami/4prod3consumer/cmake-build-debug

# Include any dependencies generated for this target.
include CMakeFiles/4prod3consumer.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/4prod3consumer.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/4prod3consumer.dir/flags.make

CMakeFiles/4prod3consumer.dir/main.cpp.o: CMakeFiles/4prod3consumer.dir/flags.make
CMakeFiles/4prod3consumer.dir/main.cpp.o: ../main.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/giuliodegiorgi/Documents/GitHub/lab_PdS_2020/esami/4prod3consumer/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/4prod3consumer.dir/main.cpp.o"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/4prod3consumer.dir/main.cpp.o -c /Users/giuliodegiorgi/Documents/GitHub/lab_PdS_2020/esami/4prod3consumer/main.cpp

CMakeFiles/4prod3consumer.dir/main.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/4prod3consumer.dir/main.cpp.i"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/giuliodegiorgi/Documents/GitHub/lab_PdS_2020/esami/4prod3consumer/main.cpp > CMakeFiles/4prod3consumer.dir/main.cpp.i

CMakeFiles/4prod3consumer.dir/main.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/4prod3consumer.dir/main.cpp.s"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/giuliodegiorgi/Documents/GitHub/lab_PdS_2020/esami/4prod3consumer/main.cpp -o CMakeFiles/4prod3consumer.dir/main.cpp.s

# Object files for target 4prod3consumer
4prod3consumer_OBJECTS = \
"CMakeFiles/4prod3consumer.dir/main.cpp.o"

# External object files for target 4prod3consumer
4prod3consumer_EXTERNAL_OBJECTS =

4prod3consumer: CMakeFiles/4prod3consumer.dir/main.cpp.o
4prod3consumer: CMakeFiles/4prod3consumer.dir/build.make
4prod3consumer: CMakeFiles/4prod3consumer.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/Users/giuliodegiorgi/Documents/GitHub/lab_PdS_2020/esami/4prod3consumer/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable 4prod3consumer"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/4prod3consumer.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/4prod3consumer.dir/build: 4prod3consumer

.PHONY : CMakeFiles/4prod3consumer.dir/build

CMakeFiles/4prod3consumer.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/4prod3consumer.dir/cmake_clean.cmake
.PHONY : CMakeFiles/4prod3consumer.dir/clean

CMakeFiles/4prod3consumer.dir/depend:
	cd /Users/giuliodegiorgi/Documents/GitHub/lab_PdS_2020/esami/4prod3consumer/cmake-build-debug && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /Users/giuliodegiorgi/Documents/GitHub/lab_PdS_2020/esami/4prod3consumer /Users/giuliodegiorgi/Documents/GitHub/lab_PdS_2020/esami/4prod3consumer /Users/giuliodegiorgi/Documents/GitHub/lab_PdS_2020/esami/4prod3consumer/cmake-build-debug /Users/giuliodegiorgi/Documents/GitHub/lab_PdS_2020/esami/4prod3consumer/cmake-build-debug /Users/giuliodegiorgi/Documents/GitHub/lab_PdS_2020/esami/4prod3consumer/cmake-build-debug/CMakeFiles/4prod3consumer.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/4prod3consumer.dir/depend
