# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.9

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
CMAKE_COMMAND = /usr/local/Cellar/cmake/3.9.0/bin/cmake

# The command to remove a file.
RM = /usr/local/Cellar/cmake/3.9.0/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /Users/pippoc/Z64C

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /Users/pippoc/Z64C/target/Debug

# Include any dependencies generated for this target.
include CMakeFiles/HIREDIS.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/HIREDIS.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/HIREDIS.dir/flags.make

../../lib/hiredis/libhiredis.a:
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --blue --bold --progress-dir=/Users/pippoc/Z64C/target/Debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Generating ../../lib/hiredis/libhiredis.a"
	cd /Users/pippoc/Z64C/lib/hiredis && make

# Object files for target HIREDIS
HIREDIS_OBJECTS =

# External object files for target HIREDIS
HIREDIS_EXTERNAL_OBJECTS =

libHIREDIS.a: CMakeFiles/HIREDIS.dir/build.make
libHIREDIS.a: CMakeFiles/HIREDIS.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/Users/pippoc/Z64C/target/Debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX static library libHIREDIS.a"
	$(CMAKE_COMMAND) -P CMakeFiles/HIREDIS.dir/cmake_clean_target.cmake
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/HIREDIS.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/HIREDIS.dir/build: libHIREDIS.a

.PHONY : CMakeFiles/HIREDIS.dir/build

CMakeFiles/HIREDIS.dir/requires:

.PHONY : CMakeFiles/HIREDIS.dir/requires

CMakeFiles/HIREDIS.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/HIREDIS.dir/cmake_clean.cmake
.PHONY : CMakeFiles/HIREDIS.dir/clean

CMakeFiles/HIREDIS.dir/depend: ../../lib/hiredis/libhiredis.a
	cd /Users/pippoc/Z64C/target/Debug && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /Users/pippoc/Z64C /Users/pippoc/Z64C /Users/pippoc/Z64C/target/Debug /Users/pippoc/Z64C/target/Debug /Users/pippoc/Z64C/target/Debug/CMakeFiles/HIREDIS.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/HIREDIS.dir/depend

