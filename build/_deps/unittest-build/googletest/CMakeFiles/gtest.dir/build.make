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
CMAKE_COMMAND = /usr/local/bin/cmake

# The command to remove a file.
RM = /usr/local/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/triccyx/Documents/fake-strain2

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/triccyx/Documents/fake-strain2/build

# Include any dependencies generated for this target.
include _deps/unittest-build/googletest/CMakeFiles/gtest.dir/depend.make

# Include the progress variables for this target.
include _deps/unittest-build/googletest/CMakeFiles/gtest.dir/progress.make

# Include the compile flags for this target's objects.
include _deps/unittest-build/googletest/CMakeFiles/gtest.dir/flags.make

_deps/unittest-build/googletest/CMakeFiles/gtest.dir/src/gtest-all.cc.o: _deps/unittest-build/googletest/CMakeFiles/gtest.dir/flags.make
_deps/unittest-build/googletest/CMakeFiles/gtest.dir/src/gtest-all.cc.o: _deps/unittest-src/googletest/src/gtest-all.cc
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/triccyx/Documents/fake-strain2/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object _deps/unittest-build/googletest/CMakeFiles/gtest.dir/src/gtest-all.cc.o"
	cd /home/triccyx/Documents/fake-strain2/build/_deps/unittest-build/googletest && /usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/gtest.dir/src/gtest-all.cc.o -c /home/triccyx/Documents/fake-strain2/build/_deps/unittest-src/googletest/src/gtest-all.cc

_deps/unittest-build/googletest/CMakeFiles/gtest.dir/src/gtest-all.cc.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/gtest.dir/src/gtest-all.cc.i"
	cd /home/triccyx/Documents/fake-strain2/build/_deps/unittest-build/googletest && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/triccyx/Documents/fake-strain2/build/_deps/unittest-src/googletest/src/gtest-all.cc > CMakeFiles/gtest.dir/src/gtest-all.cc.i

_deps/unittest-build/googletest/CMakeFiles/gtest.dir/src/gtest-all.cc.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/gtest.dir/src/gtest-all.cc.s"
	cd /home/triccyx/Documents/fake-strain2/build/_deps/unittest-build/googletest && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/triccyx/Documents/fake-strain2/build/_deps/unittest-src/googletest/src/gtest-all.cc -o CMakeFiles/gtest.dir/src/gtest-all.cc.s

# Object files for target gtest
gtest_OBJECTS = \
"CMakeFiles/gtest.dir/src/gtest-all.cc.o"

# External object files for target gtest
gtest_EXTERNAL_OBJECTS =

lib/libgtestd.a: _deps/unittest-build/googletest/CMakeFiles/gtest.dir/src/gtest-all.cc.o
lib/libgtestd.a: _deps/unittest-build/googletest/CMakeFiles/gtest.dir/build.make
lib/libgtestd.a: _deps/unittest-build/googletest/CMakeFiles/gtest.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/triccyx/Documents/fake-strain2/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX static library ../../../lib/libgtestd.a"
	cd /home/triccyx/Documents/fake-strain2/build/_deps/unittest-build/googletest && $(CMAKE_COMMAND) -P CMakeFiles/gtest.dir/cmake_clean_target.cmake
	cd /home/triccyx/Documents/fake-strain2/build/_deps/unittest-build/googletest && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/gtest.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
_deps/unittest-build/googletest/CMakeFiles/gtest.dir/build: lib/libgtestd.a

.PHONY : _deps/unittest-build/googletest/CMakeFiles/gtest.dir/build

_deps/unittest-build/googletest/CMakeFiles/gtest.dir/clean:
	cd /home/triccyx/Documents/fake-strain2/build/_deps/unittest-build/googletest && $(CMAKE_COMMAND) -P CMakeFiles/gtest.dir/cmake_clean.cmake
.PHONY : _deps/unittest-build/googletest/CMakeFiles/gtest.dir/clean

_deps/unittest-build/googletest/CMakeFiles/gtest.dir/depend:
	cd /home/triccyx/Documents/fake-strain2/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/triccyx/Documents/fake-strain2 /home/triccyx/Documents/fake-strain2/build/_deps/unittest-src/googletest /home/triccyx/Documents/fake-strain2/build /home/triccyx/Documents/fake-strain2/build/_deps/unittest-build/googletest /home/triccyx/Documents/fake-strain2/build/_deps/unittest-build/googletest/CMakeFiles/gtest.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : _deps/unittest-build/googletest/CMakeFiles/gtest.dir/depend
