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
CMAKE_SOURCE_DIR = /mnt/c/Users/barney/System/projetos/msx-tools/source-doc/tvision

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /mnt/c/Users/barney/System/projetos/msx-tools/source-doc/tvision/build

# Include any dependencies generated for this target.
include examples/tvdir/CMakeFiles/tvdir.dir/depend.make

# Include the progress variables for this target.
include examples/tvdir/CMakeFiles/tvdir.dir/progress.make

# Include the compile flags for this target's objects.
include examples/tvdir/CMakeFiles/tvdir.dir/flags.make

examples/tvdir/CMakeFiles/tvdir.dir/Unity/unity_0_cxx.cxx.o: examples/tvdir/CMakeFiles/tvdir.dir/flags.make
examples/tvdir/CMakeFiles/tvdir.dir/Unity/unity_0_cxx.cxx.o: examples/tvdir/CMakeFiles/tvdir.dir/Unity/unity_0_cxx.cxx
examples/tvdir/CMakeFiles/tvdir.dir/Unity/unity_0_cxx.cxx.o: examples/CMakeFiles/hello.dir/cmake_pch.hxx
examples/tvdir/CMakeFiles/tvdir.dir/Unity/unity_0_cxx.cxx.o: examples/CMakeFiles/hello.dir/cmake_pch.hxx.gch
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/mnt/c/Users/barney/System/projetos/msx-tools/source-doc/tvision/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object examples/tvdir/CMakeFiles/tvdir.dir/Unity/unity_0_cxx.cxx.o"
	cd /mnt/c/Users/barney/System/projetos/msx-tools/source-doc/tvision/build/examples/tvdir && /usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -Winvalid-pch -include /mnt/c/Users/barney/System/projetos/msx-tools/source-doc/tvision/build/examples/CMakeFiles/hello.dir/cmake_pch.hxx -o CMakeFiles/tvdir.dir/Unity/unity_0_cxx.cxx.o -c /mnt/c/Users/barney/System/projetos/msx-tools/source-doc/tvision/build/examples/tvdir/CMakeFiles/tvdir.dir/Unity/unity_0_cxx.cxx

examples/tvdir/CMakeFiles/tvdir.dir/Unity/unity_0_cxx.cxx.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/tvdir.dir/Unity/unity_0_cxx.cxx.i"
	cd /mnt/c/Users/barney/System/projetos/msx-tools/source-doc/tvision/build/examples/tvdir && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -Winvalid-pch -include /mnt/c/Users/barney/System/projetos/msx-tools/source-doc/tvision/build/examples/CMakeFiles/hello.dir/cmake_pch.hxx -E /mnt/c/Users/barney/System/projetos/msx-tools/source-doc/tvision/build/examples/tvdir/CMakeFiles/tvdir.dir/Unity/unity_0_cxx.cxx > CMakeFiles/tvdir.dir/Unity/unity_0_cxx.cxx.i

examples/tvdir/CMakeFiles/tvdir.dir/Unity/unity_0_cxx.cxx.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/tvdir.dir/Unity/unity_0_cxx.cxx.s"
	cd /mnt/c/Users/barney/System/projetos/msx-tools/source-doc/tvision/build/examples/tvdir && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -Winvalid-pch -include /mnt/c/Users/barney/System/projetos/msx-tools/source-doc/tvision/build/examples/CMakeFiles/hello.dir/cmake_pch.hxx -S /mnt/c/Users/barney/System/projetos/msx-tools/source-doc/tvision/build/examples/tvdir/CMakeFiles/tvdir.dir/Unity/unity_0_cxx.cxx -o CMakeFiles/tvdir.dir/Unity/unity_0_cxx.cxx.s

# Object files for target tvdir
tvdir_OBJECTS = \
"CMakeFiles/tvdir.dir/Unity/unity_0_cxx.cxx.o"

# External object files for target tvdir
tvdir_EXTERNAL_OBJECTS =

tvdir: examples/tvdir/CMakeFiles/tvdir.dir/Unity/unity_0_cxx.cxx.o
tvdir: examples/tvdir/CMakeFiles/tvdir.dir/build.make
tvdir: libtvision.a
tvdir: /usr/lib/x86_64-linux-gnu/libncursesw.so
tvdir: examples/tvdir/CMakeFiles/tvdir.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/mnt/c/Users/barney/System/projetos/msx-tools/source-doc/tvision/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable ../../tvdir"
	cd /mnt/c/Users/barney/System/projetos/msx-tools/source-doc/tvision/build/examples/tvdir && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/tvdir.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
examples/tvdir/CMakeFiles/tvdir.dir/build: tvdir

.PHONY : examples/tvdir/CMakeFiles/tvdir.dir/build

examples/tvdir/CMakeFiles/tvdir.dir/clean:
	cd /mnt/c/Users/barney/System/projetos/msx-tools/source-doc/tvision/build/examples/tvdir && $(CMAKE_COMMAND) -P CMakeFiles/tvdir.dir/cmake_clean.cmake
.PHONY : examples/tvdir/CMakeFiles/tvdir.dir/clean

examples/tvdir/CMakeFiles/tvdir.dir/depend:
	cd /mnt/c/Users/barney/System/projetos/msx-tools/source-doc/tvision/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /mnt/c/Users/barney/System/projetos/msx-tools/source-doc/tvision /mnt/c/Users/barney/System/projetos/msx-tools/source-doc/tvision/examples/tvdir /mnt/c/Users/barney/System/projetos/msx-tools/source-doc/tvision/build /mnt/c/Users/barney/System/projetos/msx-tools/source-doc/tvision/build/examples/tvdir /mnt/c/Users/barney/System/projetos/msx-tools/source-doc/tvision/build/examples/tvdir/CMakeFiles/tvdir.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : examples/tvdir/CMakeFiles/tvdir.dir/depend

