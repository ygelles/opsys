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
CMAKE_SOURCE_DIR = /mnt/f/Workspace/opersys/Hw/Hw4/skeleton

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /mnt/f/Workspace/opersys/Hw/Hw4/skeleton/cmake-build-debug

# Include any dependencies generated for this target.
include CMakeFiles/skeleton.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/skeleton.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/skeleton.dir/flags.make

CMakeFiles/skeleton.dir/main.cpp.o: CMakeFiles/skeleton.dir/flags.make
CMakeFiles/skeleton.dir/main.cpp.o: ../main.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/mnt/f/Workspace/opersys/Hw/Hw4/skeleton/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/skeleton.dir/main.cpp.o"
	/usr/bin/g++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/skeleton.dir/main.cpp.o -c /mnt/f/Workspace/opersys/Hw/Hw4/skeleton/main.cpp

CMakeFiles/skeleton.dir/main.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/skeleton.dir/main.cpp.i"
	/usr/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /mnt/f/Workspace/opersys/Hw/Hw4/skeleton/main.cpp > CMakeFiles/skeleton.dir/main.cpp.i

CMakeFiles/skeleton.dir/main.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/skeleton.dir/main.cpp.s"
	/usr/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /mnt/f/Workspace/opersys/Hw/Hw4/skeleton/main.cpp -o CMakeFiles/skeleton.dir/main.cpp.s

CMakeFiles/skeleton.dir/main.cpp.o.requires:

.PHONY : CMakeFiles/skeleton.dir/main.cpp.o.requires

CMakeFiles/skeleton.dir/main.cpp.o.provides: CMakeFiles/skeleton.dir/main.cpp.o.requires
	$(MAKE) -f CMakeFiles/skeleton.dir/build.make CMakeFiles/skeleton.dir/main.cpp.o.provides.build
.PHONY : CMakeFiles/skeleton.dir/main.cpp.o.provides

CMakeFiles/skeleton.dir/main.cpp.o.provides.build: CMakeFiles/skeleton.dir/main.cpp.o


CMakeFiles/skeleton.dir/Barrier.cpp.o: CMakeFiles/skeleton.dir/flags.make
CMakeFiles/skeleton.dir/Barrier.cpp.o: ../Barrier.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/mnt/f/Workspace/opersys/Hw/Hw4/skeleton/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object CMakeFiles/skeleton.dir/Barrier.cpp.o"
	/usr/bin/g++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/skeleton.dir/Barrier.cpp.o -c /mnt/f/Workspace/opersys/Hw/Hw4/skeleton/Barrier.cpp

CMakeFiles/skeleton.dir/Barrier.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/skeleton.dir/Barrier.cpp.i"
	/usr/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /mnt/f/Workspace/opersys/Hw/Hw4/skeleton/Barrier.cpp > CMakeFiles/skeleton.dir/Barrier.cpp.i

CMakeFiles/skeleton.dir/Barrier.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/skeleton.dir/Barrier.cpp.s"
	/usr/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /mnt/f/Workspace/opersys/Hw/Hw4/skeleton/Barrier.cpp -o CMakeFiles/skeleton.dir/Barrier.cpp.s

CMakeFiles/skeleton.dir/Barrier.cpp.o.requires:

.PHONY : CMakeFiles/skeleton.dir/Barrier.cpp.o.requires

CMakeFiles/skeleton.dir/Barrier.cpp.o.provides: CMakeFiles/skeleton.dir/Barrier.cpp.o.requires
	$(MAKE) -f CMakeFiles/skeleton.dir/build.make CMakeFiles/skeleton.dir/Barrier.cpp.o.provides.build
.PHONY : CMakeFiles/skeleton.dir/Barrier.cpp.o.provides

CMakeFiles/skeleton.dir/Barrier.cpp.o.provides.build: CMakeFiles/skeleton.dir/Barrier.cpp.o


# Object files for target skeleton
skeleton_OBJECTS = \
"CMakeFiles/skeleton.dir/main.cpp.o" \
"CMakeFiles/skeleton.dir/Barrier.cpp.o"

# External object files for target skeleton
skeleton_EXTERNAL_OBJECTS =

skeleton: CMakeFiles/skeleton.dir/main.cpp.o
skeleton: CMakeFiles/skeleton.dir/Barrier.cpp.o
skeleton: CMakeFiles/skeleton.dir/build.make
skeleton: CMakeFiles/skeleton.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/mnt/f/Workspace/opersys/Hw/Hw4/skeleton/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Linking CXX executable skeleton"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/skeleton.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/skeleton.dir/build: skeleton

.PHONY : CMakeFiles/skeleton.dir/build

CMakeFiles/skeleton.dir/requires: CMakeFiles/skeleton.dir/main.cpp.o.requires
CMakeFiles/skeleton.dir/requires: CMakeFiles/skeleton.dir/Barrier.cpp.o.requires

.PHONY : CMakeFiles/skeleton.dir/requires

CMakeFiles/skeleton.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/skeleton.dir/cmake_clean.cmake
.PHONY : CMakeFiles/skeleton.dir/clean

CMakeFiles/skeleton.dir/depend:
	cd /mnt/f/Workspace/opersys/Hw/Hw4/skeleton/cmake-build-debug && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /mnt/f/Workspace/opersys/Hw/Hw4/skeleton /mnt/f/Workspace/opersys/Hw/Hw4/skeleton /mnt/f/Workspace/opersys/Hw/Hw4/skeleton/cmake-build-debug /mnt/f/Workspace/opersys/Hw/Hw4/skeleton/cmake-build-debug /mnt/f/Workspace/opersys/Hw/Hw4/skeleton/cmake-build-debug/CMakeFiles/skeleton.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/skeleton.dir/depend

