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
CMAKE_BINARY_DIR = /Users/pippoc/Z64C/target/Release

# Include any dependencies generated for this target.
include CMakeFiles/Z64C_LIB.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/Z64C_LIB.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/Z64C_LIB.dir/flags.make

CMakeFiles/Z64C_LIB.dir/src/ann.c.o: CMakeFiles/Z64C_LIB.dir/flags.make
CMakeFiles/Z64C_LIB.dir/src/ann.c.o: ../../src/ann.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/pippoc/Z64C/target/Release/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building C object CMakeFiles/Z64C_LIB.dir/src/ann.c.o"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/Z64C_LIB.dir/src/ann.c.o   -c /Users/pippoc/Z64C/src/ann.c

CMakeFiles/Z64C_LIB.dir/src/ann.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/Z64C_LIB.dir/src/ann.c.i"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /Users/pippoc/Z64C/src/ann.c > CMakeFiles/Z64C_LIB.dir/src/ann.c.i

CMakeFiles/Z64C_LIB.dir/src/ann.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/Z64C_LIB.dir/src/ann.c.s"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /Users/pippoc/Z64C/src/ann.c -o CMakeFiles/Z64C_LIB.dir/src/ann.c.s

CMakeFiles/Z64C_LIB.dir/src/ann.c.o.requires:

.PHONY : CMakeFiles/Z64C_LIB.dir/src/ann.c.o.requires

CMakeFiles/Z64C_LIB.dir/src/ann.c.o.provides: CMakeFiles/Z64C_LIB.dir/src/ann.c.o.requires
	$(MAKE) -f CMakeFiles/Z64C_LIB.dir/build.make CMakeFiles/Z64C_LIB.dir/src/ann.c.o.provides.build
.PHONY : CMakeFiles/Z64C_LIB.dir/src/ann.c.o.provides

CMakeFiles/Z64C_LIB.dir/src/ann.c.o.provides.build: CMakeFiles/Z64C_LIB.dir/src/ann.c.o


CMakeFiles/Z64C_LIB.dir/src/board.c.o: CMakeFiles/Z64C_LIB.dir/flags.make
CMakeFiles/Z64C_LIB.dir/src/board.c.o: ../../src/board.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/pippoc/Z64C/target/Release/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building C object CMakeFiles/Z64C_LIB.dir/src/board.c.o"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/Z64C_LIB.dir/src/board.c.o   -c /Users/pippoc/Z64C/src/board.c

CMakeFiles/Z64C_LIB.dir/src/board.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/Z64C_LIB.dir/src/board.c.i"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /Users/pippoc/Z64C/src/board.c > CMakeFiles/Z64C_LIB.dir/src/board.c.i

CMakeFiles/Z64C_LIB.dir/src/board.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/Z64C_LIB.dir/src/board.c.s"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /Users/pippoc/Z64C/src/board.c -o CMakeFiles/Z64C_LIB.dir/src/board.c.s

CMakeFiles/Z64C_LIB.dir/src/board.c.o.requires:

.PHONY : CMakeFiles/Z64C_LIB.dir/src/board.c.o.requires

CMakeFiles/Z64C_LIB.dir/src/board.c.o.provides: CMakeFiles/Z64C_LIB.dir/src/board.c.o.requires
	$(MAKE) -f CMakeFiles/Z64C_LIB.dir/build.make CMakeFiles/Z64C_LIB.dir/src/board.c.o.provides.build
.PHONY : CMakeFiles/Z64C_LIB.dir/src/board.c.o.provides

CMakeFiles/Z64C_LIB.dir/src/board.c.o.provides.build: CMakeFiles/Z64C_LIB.dir/src/board.c.o


CMakeFiles/Z64C_LIB.dir/src/castling.c.o: CMakeFiles/Z64C_LIB.dir/flags.make
CMakeFiles/Z64C_LIB.dir/src/castling.c.o: ../../src/castling.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/pippoc/Z64C/target/Release/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building C object CMakeFiles/Z64C_LIB.dir/src/castling.c.o"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/Z64C_LIB.dir/src/castling.c.o   -c /Users/pippoc/Z64C/src/castling.c

CMakeFiles/Z64C_LIB.dir/src/castling.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/Z64C_LIB.dir/src/castling.c.i"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /Users/pippoc/Z64C/src/castling.c > CMakeFiles/Z64C_LIB.dir/src/castling.c.i

CMakeFiles/Z64C_LIB.dir/src/castling.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/Z64C_LIB.dir/src/castling.c.s"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /Users/pippoc/Z64C/src/castling.c -o CMakeFiles/Z64C_LIB.dir/src/castling.c.s

CMakeFiles/Z64C_LIB.dir/src/castling.c.o.requires:

.PHONY : CMakeFiles/Z64C_LIB.dir/src/castling.c.o.requires

CMakeFiles/Z64C_LIB.dir/src/castling.c.o.provides: CMakeFiles/Z64C_LIB.dir/src/castling.c.o.requires
	$(MAKE) -f CMakeFiles/Z64C_LIB.dir/build.make CMakeFiles/Z64C_LIB.dir/src/castling.c.o.provides.build
.PHONY : CMakeFiles/Z64C_LIB.dir/src/castling.c.o.provides

CMakeFiles/Z64C_LIB.dir/src/castling.c.o.provides.build: CMakeFiles/Z64C_LIB.dir/src/castling.c.o


CMakeFiles/Z64C_LIB.dir/src/clock.c.o: CMakeFiles/Z64C_LIB.dir/flags.make
CMakeFiles/Z64C_LIB.dir/src/clock.c.o: ../../src/clock.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/pippoc/Z64C/target/Release/CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Building C object CMakeFiles/Z64C_LIB.dir/src/clock.c.o"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/Z64C_LIB.dir/src/clock.c.o   -c /Users/pippoc/Z64C/src/clock.c

CMakeFiles/Z64C_LIB.dir/src/clock.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/Z64C_LIB.dir/src/clock.c.i"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /Users/pippoc/Z64C/src/clock.c > CMakeFiles/Z64C_LIB.dir/src/clock.c.i

CMakeFiles/Z64C_LIB.dir/src/clock.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/Z64C_LIB.dir/src/clock.c.s"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /Users/pippoc/Z64C/src/clock.c -o CMakeFiles/Z64C_LIB.dir/src/clock.c.s

CMakeFiles/Z64C_LIB.dir/src/clock.c.o.requires:

.PHONY : CMakeFiles/Z64C_LIB.dir/src/clock.c.o.requires

CMakeFiles/Z64C_LIB.dir/src/clock.c.o.provides: CMakeFiles/Z64C_LIB.dir/src/clock.c.o.requires
	$(MAKE) -f CMakeFiles/Z64C_LIB.dir/build.make CMakeFiles/Z64C_LIB.dir/src/clock.c.o.provides.build
.PHONY : CMakeFiles/Z64C_LIB.dir/src/clock.c.o.provides

CMakeFiles/Z64C_LIB.dir/src/clock.c.o.provides.build: CMakeFiles/Z64C_LIB.dir/src/clock.c.o


CMakeFiles/Z64C_LIB.dir/src/color.c.o: CMakeFiles/Z64C_LIB.dir/flags.make
CMakeFiles/Z64C_LIB.dir/src/color.c.o: ../../src/color.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/pippoc/Z64C/target/Release/CMakeFiles --progress-num=$(CMAKE_PROGRESS_5) "Building C object CMakeFiles/Z64C_LIB.dir/src/color.c.o"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/Z64C_LIB.dir/src/color.c.o   -c /Users/pippoc/Z64C/src/color.c

CMakeFiles/Z64C_LIB.dir/src/color.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/Z64C_LIB.dir/src/color.c.i"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /Users/pippoc/Z64C/src/color.c > CMakeFiles/Z64C_LIB.dir/src/color.c.i

CMakeFiles/Z64C_LIB.dir/src/color.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/Z64C_LIB.dir/src/color.c.s"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /Users/pippoc/Z64C/src/color.c -o CMakeFiles/Z64C_LIB.dir/src/color.c.s

CMakeFiles/Z64C_LIB.dir/src/color.c.o.requires:

.PHONY : CMakeFiles/Z64C_LIB.dir/src/color.c.o.requires

CMakeFiles/Z64C_LIB.dir/src/color.c.o.provides: CMakeFiles/Z64C_LIB.dir/src/color.c.o.requires
	$(MAKE) -f CMakeFiles/Z64C_LIB.dir/build.make CMakeFiles/Z64C_LIB.dir/src/color.c.o.provides.build
.PHONY : CMakeFiles/Z64C_LIB.dir/src/color.c.o.provides

CMakeFiles/Z64C_LIB.dir/src/color.c.o.provides.build: CMakeFiles/Z64C_LIB.dir/src/color.c.o


CMakeFiles/Z64C_LIB.dir/src/coord.c.o: CMakeFiles/Z64C_LIB.dir/flags.make
CMakeFiles/Z64C_LIB.dir/src/coord.c.o: ../../src/coord.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/pippoc/Z64C/target/Release/CMakeFiles --progress-num=$(CMAKE_PROGRESS_6) "Building C object CMakeFiles/Z64C_LIB.dir/src/coord.c.o"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/Z64C_LIB.dir/src/coord.c.o   -c /Users/pippoc/Z64C/src/coord.c

CMakeFiles/Z64C_LIB.dir/src/coord.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/Z64C_LIB.dir/src/coord.c.i"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /Users/pippoc/Z64C/src/coord.c > CMakeFiles/Z64C_LIB.dir/src/coord.c.i

CMakeFiles/Z64C_LIB.dir/src/coord.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/Z64C_LIB.dir/src/coord.c.s"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /Users/pippoc/Z64C/src/coord.c -o CMakeFiles/Z64C_LIB.dir/src/coord.c.s

CMakeFiles/Z64C_LIB.dir/src/coord.c.o.requires:

.PHONY : CMakeFiles/Z64C_LIB.dir/src/coord.c.o.requires

CMakeFiles/Z64C_LIB.dir/src/coord.c.o.provides: CMakeFiles/Z64C_LIB.dir/src/coord.c.o.requires
	$(MAKE) -f CMakeFiles/Z64C_LIB.dir/build.make CMakeFiles/Z64C_LIB.dir/src/coord.c.o.provides.build
.PHONY : CMakeFiles/Z64C_LIB.dir/src/coord.c.o.provides

CMakeFiles/Z64C_LIB.dir/src/coord.c.o.provides.build: CMakeFiles/Z64C_LIB.dir/src/coord.c.o


CMakeFiles/Z64C_LIB.dir/src/engine.c.o: CMakeFiles/Z64C_LIB.dir/flags.make
CMakeFiles/Z64C_LIB.dir/src/engine.c.o: ../../src/engine.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/pippoc/Z64C/target/Release/CMakeFiles --progress-num=$(CMAKE_PROGRESS_7) "Building C object CMakeFiles/Z64C_LIB.dir/src/engine.c.o"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/Z64C_LIB.dir/src/engine.c.o   -c /Users/pippoc/Z64C/src/engine.c

CMakeFiles/Z64C_LIB.dir/src/engine.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/Z64C_LIB.dir/src/engine.c.i"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /Users/pippoc/Z64C/src/engine.c > CMakeFiles/Z64C_LIB.dir/src/engine.c.i

CMakeFiles/Z64C_LIB.dir/src/engine.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/Z64C_LIB.dir/src/engine.c.s"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /Users/pippoc/Z64C/src/engine.c -o CMakeFiles/Z64C_LIB.dir/src/engine.c.s

CMakeFiles/Z64C_LIB.dir/src/engine.c.o.requires:

.PHONY : CMakeFiles/Z64C_LIB.dir/src/engine.c.o.requires

CMakeFiles/Z64C_LIB.dir/src/engine.c.o.provides: CMakeFiles/Z64C_LIB.dir/src/engine.c.o.requires
	$(MAKE) -f CMakeFiles/Z64C_LIB.dir/build.make CMakeFiles/Z64C_LIB.dir/src/engine.c.o.provides.build
.PHONY : CMakeFiles/Z64C_LIB.dir/src/engine.c.o.provides

CMakeFiles/Z64C_LIB.dir/src/engine.c.o.provides.build: CMakeFiles/Z64C_LIB.dir/src/engine.c.o


CMakeFiles/Z64C_LIB.dir/src/fen.c.o: CMakeFiles/Z64C_LIB.dir/flags.make
CMakeFiles/Z64C_LIB.dir/src/fen.c.o: ../../src/fen.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/pippoc/Z64C/target/Release/CMakeFiles --progress-num=$(CMAKE_PROGRESS_8) "Building C object CMakeFiles/Z64C_LIB.dir/src/fen.c.o"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/Z64C_LIB.dir/src/fen.c.o   -c /Users/pippoc/Z64C/src/fen.c

CMakeFiles/Z64C_LIB.dir/src/fen.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/Z64C_LIB.dir/src/fen.c.i"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /Users/pippoc/Z64C/src/fen.c > CMakeFiles/Z64C_LIB.dir/src/fen.c.i

CMakeFiles/Z64C_LIB.dir/src/fen.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/Z64C_LIB.dir/src/fen.c.s"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /Users/pippoc/Z64C/src/fen.c -o CMakeFiles/Z64C_LIB.dir/src/fen.c.s

CMakeFiles/Z64C_LIB.dir/src/fen.c.o.requires:

.PHONY : CMakeFiles/Z64C_LIB.dir/src/fen.c.o.requires

CMakeFiles/Z64C_LIB.dir/src/fen.c.o.provides: CMakeFiles/Z64C_LIB.dir/src/fen.c.o.requires
	$(MAKE) -f CMakeFiles/Z64C_LIB.dir/build.make CMakeFiles/Z64C_LIB.dir/src/fen.c.o.provides.build
.PHONY : CMakeFiles/Z64C_LIB.dir/src/fen.c.o.provides

CMakeFiles/Z64C_LIB.dir/src/fen.c.o.provides.build: CMakeFiles/Z64C_LIB.dir/src/fen.c.o


CMakeFiles/Z64C_LIB.dir/src/game.c.o: CMakeFiles/Z64C_LIB.dir/flags.make
CMakeFiles/Z64C_LIB.dir/src/game.c.o: ../../src/game.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/pippoc/Z64C/target/Release/CMakeFiles --progress-num=$(CMAKE_PROGRESS_9) "Building C object CMakeFiles/Z64C_LIB.dir/src/game.c.o"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/Z64C_LIB.dir/src/game.c.o   -c /Users/pippoc/Z64C/src/game.c

CMakeFiles/Z64C_LIB.dir/src/game.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/Z64C_LIB.dir/src/game.c.i"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /Users/pippoc/Z64C/src/game.c > CMakeFiles/Z64C_LIB.dir/src/game.c.i

CMakeFiles/Z64C_LIB.dir/src/game.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/Z64C_LIB.dir/src/game.c.s"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /Users/pippoc/Z64C/src/game.c -o CMakeFiles/Z64C_LIB.dir/src/game.c.s

CMakeFiles/Z64C_LIB.dir/src/game.c.o.requires:

.PHONY : CMakeFiles/Z64C_LIB.dir/src/game.c.o.requires

CMakeFiles/Z64C_LIB.dir/src/game.c.o.provides: CMakeFiles/Z64C_LIB.dir/src/game.c.o.requires
	$(MAKE) -f CMakeFiles/Z64C_LIB.dir/build.make CMakeFiles/Z64C_LIB.dir/src/game.c.o.provides.build
.PHONY : CMakeFiles/Z64C_LIB.dir/src/game.c.o.provides

CMakeFiles/Z64C_LIB.dir/src/game.c.o.provides.build: CMakeFiles/Z64C_LIB.dir/src/game.c.o


CMakeFiles/Z64C_LIB.dir/src/history.c.o: CMakeFiles/Z64C_LIB.dir/flags.make
CMakeFiles/Z64C_LIB.dir/src/history.c.o: ../../src/history.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/pippoc/Z64C/target/Release/CMakeFiles --progress-num=$(CMAKE_PROGRESS_10) "Building C object CMakeFiles/Z64C_LIB.dir/src/history.c.o"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/Z64C_LIB.dir/src/history.c.o   -c /Users/pippoc/Z64C/src/history.c

CMakeFiles/Z64C_LIB.dir/src/history.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/Z64C_LIB.dir/src/history.c.i"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /Users/pippoc/Z64C/src/history.c > CMakeFiles/Z64C_LIB.dir/src/history.c.i

CMakeFiles/Z64C_LIB.dir/src/history.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/Z64C_LIB.dir/src/history.c.s"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /Users/pippoc/Z64C/src/history.c -o CMakeFiles/Z64C_LIB.dir/src/history.c.s

CMakeFiles/Z64C_LIB.dir/src/history.c.o.requires:

.PHONY : CMakeFiles/Z64C_LIB.dir/src/history.c.o.requires

CMakeFiles/Z64C_LIB.dir/src/history.c.o.provides: CMakeFiles/Z64C_LIB.dir/src/history.c.o.requires
	$(MAKE) -f CMakeFiles/Z64C_LIB.dir/build.make CMakeFiles/Z64C_LIB.dir/src/history.c.o.provides.build
.PHONY : CMakeFiles/Z64C_LIB.dir/src/history.c.o.provides

CMakeFiles/Z64C_LIB.dir/src/history.c.o.provides.build: CMakeFiles/Z64C_LIB.dir/src/history.c.o


CMakeFiles/Z64C_LIB.dir/src/main.c.o: CMakeFiles/Z64C_LIB.dir/flags.make
CMakeFiles/Z64C_LIB.dir/src/main.c.o: ../../src/main.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/pippoc/Z64C/target/Release/CMakeFiles --progress-num=$(CMAKE_PROGRESS_11) "Building C object CMakeFiles/Z64C_LIB.dir/src/main.c.o"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/Z64C_LIB.dir/src/main.c.o   -c /Users/pippoc/Z64C/src/main.c

CMakeFiles/Z64C_LIB.dir/src/main.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/Z64C_LIB.dir/src/main.c.i"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /Users/pippoc/Z64C/src/main.c > CMakeFiles/Z64C_LIB.dir/src/main.c.i

CMakeFiles/Z64C_LIB.dir/src/main.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/Z64C_LIB.dir/src/main.c.s"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /Users/pippoc/Z64C/src/main.c -o CMakeFiles/Z64C_LIB.dir/src/main.c.s

CMakeFiles/Z64C_LIB.dir/src/main.c.o.requires:

.PHONY : CMakeFiles/Z64C_LIB.dir/src/main.c.o.requires

CMakeFiles/Z64C_LIB.dir/src/main.c.o.provides: CMakeFiles/Z64C_LIB.dir/src/main.c.o.requires
	$(MAKE) -f CMakeFiles/Z64C_LIB.dir/build.make CMakeFiles/Z64C_LIB.dir/src/main.c.o.provides.build
.PHONY : CMakeFiles/Z64C_LIB.dir/src/main.c.o.provides

CMakeFiles/Z64C_LIB.dir/src/main.c.o.provides.build: CMakeFiles/Z64C_LIB.dir/src/main.c.o


CMakeFiles/Z64C_LIB.dir/src/move.c.o: CMakeFiles/Z64C_LIB.dir/flags.make
CMakeFiles/Z64C_LIB.dir/src/move.c.o: ../../src/move.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/pippoc/Z64C/target/Release/CMakeFiles --progress-num=$(CMAKE_PROGRESS_12) "Building C object CMakeFiles/Z64C_LIB.dir/src/move.c.o"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/Z64C_LIB.dir/src/move.c.o   -c /Users/pippoc/Z64C/src/move.c

CMakeFiles/Z64C_LIB.dir/src/move.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/Z64C_LIB.dir/src/move.c.i"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /Users/pippoc/Z64C/src/move.c > CMakeFiles/Z64C_LIB.dir/src/move.c.i

CMakeFiles/Z64C_LIB.dir/src/move.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/Z64C_LIB.dir/src/move.c.s"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /Users/pippoc/Z64C/src/move.c -o CMakeFiles/Z64C_LIB.dir/src/move.c.s

CMakeFiles/Z64C_LIB.dir/src/move.c.o.requires:

.PHONY : CMakeFiles/Z64C_LIB.dir/src/move.c.o.requires

CMakeFiles/Z64C_LIB.dir/src/move.c.o.provides: CMakeFiles/Z64C_LIB.dir/src/move.c.o.requires
	$(MAKE) -f CMakeFiles/Z64C_LIB.dir/build.make CMakeFiles/Z64C_LIB.dir/src/move.c.o.provides.build
.PHONY : CMakeFiles/Z64C_LIB.dir/src/move.c.o.provides

CMakeFiles/Z64C_LIB.dir/src/move.c.o.provides.build: CMakeFiles/Z64C_LIB.dir/src/move.c.o


CMakeFiles/Z64C_LIB.dir/src/piece.c.o: CMakeFiles/Z64C_LIB.dir/flags.make
CMakeFiles/Z64C_LIB.dir/src/piece.c.o: ../../src/piece.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/pippoc/Z64C/target/Release/CMakeFiles --progress-num=$(CMAKE_PROGRESS_13) "Building C object CMakeFiles/Z64C_LIB.dir/src/piece.c.o"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/Z64C_LIB.dir/src/piece.c.o   -c /Users/pippoc/Z64C/src/piece.c

CMakeFiles/Z64C_LIB.dir/src/piece.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/Z64C_LIB.dir/src/piece.c.i"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /Users/pippoc/Z64C/src/piece.c > CMakeFiles/Z64C_LIB.dir/src/piece.c.i

CMakeFiles/Z64C_LIB.dir/src/piece.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/Z64C_LIB.dir/src/piece.c.s"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /Users/pippoc/Z64C/src/piece.c -o CMakeFiles/Z64C_LIB.dir/src/piece.c.s

CMakeFiles/Z64C_LIB.dir/src/piece.c.o.requires:

.PHONY : CMakeFiles/Z64C_LIB.dir/src/piece.c.o.requires

CMakeFiles/Z64C_LIB.dir/src/piece.c.o.provides: CMakeFiles/Z64C_LIB.dir/src/piece.c.o.requires
	$(MAKE) -f CMakeFiles/Z64C_LIB.dir/build.make CMakeFiles/Z64C_LIB.dir/src/piece.c.o.provides.build
.PHONY : CMakeFiles/Z64C_LIB.dir/src/piece.c.o.provides

CMakeFiles/Z64C_LIB.dir/src/piece.c.o.provides.build: CMakeFiles/Z64C_LIB.dir/src/piece.c.o


CMakeFiles/Z64C_LIB.dir/src/search.c.o: CMakeFiles/Z64C_LIB.dir/flags.make
CMakeFiles/Z64C_LIB.dir/src/search.c.o: ../../src/search.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/pippoc/Z64C/target/Release/CMakeFiles --progress-num=$(CMAKE_PROGRESS_14) "Building C object CMakeFiles/Z64C_LIB.dir/src/search.c.o"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/Z64C_LIB.dir/src/search.c.o   -c /Users/pippoc/Z64C/src/search.c

CMakeFiles/Z64C_LIB.dir/src/search.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/Z64C_LIB.dir/src/search.c.i"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /Users/pippoc/Z64C/src/search.c > CMakeFiles/Z64C_LIB.dir/src/search.c.i

CMakeFiles/Z64C_LIB.dir/src/search.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/Z64C_LIB.dir/src/search.c.s"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /Users/pippoc/Z64C/src/search.c -o CMakeFiles/Z64C_LIB.dir/src/search.c.s

CMakeFiles/Z64C_LIB.dir/src/search.c.o.requires:

.PHONY : CMakeFiles/Z64C_LIB.dir/src/search.c.o.requires

CMakeFiles/Z64C_LIB.dir/src/search.c.o.provides: CMakeFiles/Z64C_LIB.dir/src/search.c.o.requires
	$(MAKE) -f CMakeFiles/Z64C_LIB.dir/build.make CMakeFiles/Z64C_LIB.dir/src/search.c.o.provides.build
.PHONY : CMakeFiles/Z64C_LIB.dir/src/search.c.o.provides

CMakeFiles/Z64C_LIB.dir/src/search.c.o.provides.build: CMakeFiles/Z64C_LIB.dir/src/search.c.o


CMakeFiles/Z64C_LIB.dir/src/transpos_table.c.o: CMakeFiles/Z64C_LIB.dir/flags.make
CMakeFiles/Z64C_LIB.dir/src/transpos_table.c.o: ../../src/transpos_table.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/pippoc/Z64C/target/Release/CMakeFiles --progress-num=$(CMAKE_PROGRESS_15) "Building C object CMakeFiles/Z64C_LIB.dir/src/transpos_table.c.o"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/Z64C_LIB.dir/src/transpos_table.c.o   -c /Users/pippoc/Z64C/src/transpos_table.c

CMakeFiles/Z64C_LIB.dir/src/transpos_table.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/Z64C_LIB.dir/src/transpos_table.c.i"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /Users/pippoc/Z64C/src/transpos_table.c > CMakeFiles/Z64C_LIB.dir/src/transpos_table.c.i

CMakeFiles/Z64C_LIB.dir/src/transpos_table.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/Z64C_LIB.dir/src/transpos_table.c.s"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /Users/pippoc/Z64C/src/transpos_table.c -o CMakeFiles/Z64C_LIB.dir/src/transpos_table.c.s

CMakeFiles/Z64C_LIB.dir/src/transpos_table.c.o.requires:

.PHONY : CMakeFiles/Z64C_LIB.dir/src/transpos_table.c.o.requires

CMakeFiles/Z64C_LIB.dir/src/transpos_table.c.o.provides: CMakeFiles/Z64C_LIB.dir/src/transpos_table.c.o.requires
	$(MAKE) -f CMakeFiles/Z64C_LIB.dir/build.make CMakeFiles/Z64C_LIB.dir/src/transpos_table.c.o.provides.build
.PHONY : CMakeFiles/Z64C_LIB.dir/src/transpos_table.c.o.provides

CMakeFiles/Z64C_LIB.dir/src/transpos_table.c.o.provides.build: CMakeFiles/Z64C_LIB.dir/src/transpos_table.c.o


CMakeFiles/Z64C_LIB.dir/src/uci.c.o: CMakeFiles/Z64C_LIB.dir/flags.make
CMakeFiles/Z64C_LIB.dir/src/uci.c.o: ../../src/uci.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/pippoc/Z64C/target/Release/CMakeFiles --progress-num=$(CMAKE_PROGRESS_16) "Building C object CMakeFiles/Z64C_LIB.dir/src/uci.c.o"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/Z64C_LIB.dir/src/uci.c.o   -c /Users/pippoc/Z64C/src/uci.c

CMakeFiles/Z64C_LIB.dir/src/uci.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/Z64C_LIB.dir/src/uci.c.i"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /Users/pippoc/Z64C/src/uci.c > CMakeFiles/Z64C_LIB.dir/src/uci.c.i

CMakeFiles/Z64C_LIB.dir/src/uci.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/Z64C_LIB.dir/src/uci.c.s"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /Users/pippoc/Z64C/src/uci.c -o CMakeFiles/Z64C_LIB.dir/src/uci.c.s

CMakeFiles/Z64C_LIB.dir/src/uci.c.o.requires:

.PHONY : CMakeFiles/Z64C_LIB.dir/src/uci.c.o.requires

CMakeFiles/Z64C_LIB.dir/src/uci.c.o.provides: CMakeFiles/Z64C_LIB.dir/src/uci.c.o.requires
	$(MAKE) -f CMakeFiles/Z64C_LIB.dir/build.make CMakeFiles/Z64C_LIB.dir/src/uci.c.o.provides.build
.PHONY : CMakeFiles/Z64C_LIB.dir/src/uci.c.o.provides

CMakeFiles/Z64C_LIB.dir/src/uci.c.o.provides.build: CMakeFiles/Z64C_LIB.dir/src/uci.c.o


CMakeFiles/Z64C_LIB.dir/src/utils.c.o: CMakeFiles/Z64C_LIB.dir/flags.make
CMakeFiles/Z64C_LIB.dir/src/utils.c.o: ../../src/utils.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/pippoc/Z64C/target/Release/CMakeFiles --progress-num=$(CMAKE_PROGRESS_17) "Building C object CMakeFiles/Z64C_LIB.dir/src/utils.c.o"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/Z64C_LIB.dir/src/utils.c.o   -c /Users/pippoc/Z64C/src/utils.c

CMakeFiles/Z64C_LIB.dir/src/utils.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/Z64C_LIB.dir/src/utils.c.i"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /Users/pippoc/Z64C/src/utils.c > CMakeFiles/Z64C_LIB.dir/src/utils.c.i

CMakeFiles/Z64C_LIB.dir/src/utils.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/Z64C_LIB.dir/src/utils.c.s"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /Users/pippoc/Z64C/src/utils.c -o CMakeFiles/Z64C_LIB.dir/src/utils.c.s

CMakeFiles/Z64C_LIB.dir/src/utils.c.o.requires:

.PHONY : CMakeFiles/Z64C_LIB.dir/src/utils.c.o.requires

CMakeFiles/Z64C_LIB.dir/src/utils.c.o.provides: CMakeFiles/Z64C_LIB.dir/src/utils.c.o.requires
	$(MAKE) -f CMakeFiles/Z64C_LIB.dir/build.make CMakeFiles/Z64C_LIB.dir/src/utils.c.o.provides.build
.PHONY : CMakeFiles/Z64C_LIB.dir/src/utils.c.o.provides

CMakeFiles/Z64C_LIB.dir/src/utils.c.o.provides.build: CMakeFiles/Z64C_LIB.dir/src/utils.c.o


# Object files for target Z64C_LIB
Z64C_LIB_OBJECTS = \
"CMakeFiles/Z64C_LIB.dir/src/ann.c.o" \
"CMakeFiles/Z64C_LIB.dir/src/board.c.o" \
"CMakeFiles/Z64C_LIB.dir/src/castling.c.o" \
"CMakeFiles/Z64C_LIB.dir/src/clock.c.o" \
"CMakeFiles/Z64C_LIB.dir/src/color.c.o" \
"CMakeFiles/Z64C_LIB.dir/src/coord.c.o" \
"CMakeFiles/Z64C_LIB.dir/src/engine.c.o" \
"CMakeFiles/Z64C_LIB.dir/src/fen.c.o" \
"CMakeFiles/Z64C_LIB.dir/src/game.c.o" \
"CMakeFiles/Z64C_LIB.dir/src/history.c.o" \
"CMakeFiles/Z64C_LIB.dir/src/main.c.o" \
"CMakeFiles/Z64C_LIB.dir/src/move.c.o" \
"CMakeFiles/Z64C_LIB.dir/src/piece.c.o" \
"CMakeFiles/Z64C_LIB.dir/src/search.c.o" \
"CMakeFiles/Z64C_LIB.dir/src/transpos_table.c.o" \
"CMakeFiles/Z64C_LIB.dir/src/uci.c.o" \
"CMakeFiles/Z64C_LIB.dir/src/utils.c.o"

# External object files for target Z64C_LIB
Z64C_LIB_EXTERNAL_OBJECTS =

libZ64C_LIB.dylib: CMakeFiles/Z64C_LIB.dir/src/ann.c.o
libZ64C_LIB.dylib: CMakeFiles/Z64C_LIB.dir/src/board.c.o
libZ64C_LIB.dylib: CMakeFiles/Z64C_LIB.dir/src/castling.c.o
libZ64C_LIB.dylib: CMakeFiles/Z64C_LIB.dir/src/clock.c.o
libZ64C_LIB.dylib: CMakeFiles/Z64C_LIB.dir/src/color.c.o
libZ64C_LIB.dylib: CMakeFiles/Z64C_LIB.dir/src/coord.c.o
libZ64C_LIB.dylib: CMakeFiles/Z64C_LIB.dir/src/engine.c.o
libZ64C_LIB.dylib: CMakeFiles/Z64C_LIB.dir/src/fen.c.o
libZ64C_LIB.dylib: CMakeFiles/Z64C_LIB.dir/src/game.c.o
libZ64C_LIB.dylib: CMakeFiles/Z64C_LIB.dir/src/history.c.o
libZ64C_LIB.dylib: CMakeFiles/Z64C_LIB.dir/src/main.c.o
libZ64C_LIB.dylib: CMakeFiles/Z64C_LIB.dir/src/move.c.o
libZ64C_LIB.dylib: CMakeFiles/Z64C_LIB.dir/src/piece.c.o
libZ64C_LIB.dylib: CMakeFiles/Z64C_LIB.dir/src/search.c.o
libZ64C_LIB.dylib: CMakeFiles/Z64C_LIB.dir/src/transpos_table.c.o
libZ64C_LIB.dylib: CMakeFiles/Z64C_LIB.dir/src/uci.c.o
libZ64C_LIB.dylib: CMakeFiles/Z64C_LIB.dir/src/utils.c.o
libZ64C_LIB.dylib: CMakeFiles/Z64C_LIB.dir/build.make
libZ64C_LIB.dylib: libHIREDIS.dylib
libZ64C_LIB.dylib: CMakeFiles/Z64C_LIB.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/Users/pippoc/Z64C/target/Release/CMakeFiles --progress-num=$(CMAKE_PROGRESS_18) "Linking C shared library libZ64C_LIB.dylib"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/Z64C_LIB.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/Z64C_LIB.dir/build: libZ64C_LIB.dylib

.PHONY : CMakeFiles/Z64C_LIB.dir/build

CMakeFiles/Z64C_LIB.dir/requires: CMakeFiles/Z64C_LIB.dir/src/ann.c.o.requires
CMakeFiles/Z64C_LIB.dir/requires: CMakeFiles/Z64C_LIB.dir/src/board.c.o.requires
CMakeFiles/Z64C_LIB.dir/requires: CMakeFiles/Z64C_LIB.dir/src/castling.c.o.requires
CMakeFiles/Z64C_LIB.dir/requires: CMakeFiles/Z64C_LIB.dir/src/clock.c.o.requires
CMakeFiles/Z64C_LIB.dir/requires: CMakeFiles/Z64C_LIB.dir/src/color.c.o.requires
CMakeFiles/Z64C_LIB.dir/requires: CMakeFiles/Z64C_LIB.dir/src/coord.c.o.requires
CMakeFiles/Z64C_LIB.dir/requires: CMakeFiles/Z64C_LIB.dir/src/engine.c.o.requires
CMakeFiles/Z64C_LIB.dir/requires: CMakeFiles/Z64C_LIB.dir/src/fen.c.o.requires
CMakeFiles/Z64C_LIB.dir/requires: CMakeFiles/Z64C_LIB.dir/src/game.c.o.requires
CMakeFiles/Z64C_LIB.dir/requires: CMakeFiles/Z64C_LIB.dir/src/history.c.o.requires
CMakeFiles/Z64C_LIB.dir/requires: CMakeFiles/Z64C_LIB.dir/src/main.c.o.requires
CMakeFiles/Z64C_LIB.dir/requires: CMakeFiles/Z64C_LIB.dir/src/move.c.o.requires
CMakeFiles/Z64C_LIB.dir/requires: CMakeFiles/Z64C_LIB.dir/src/piece.c.o.requires
CMakeFiles/Z64C_LIB.dir/requires: CMakeFiles/Z64C_LIB.dir/src/search.c.o.requires
CMakeFiles/Z64C_LIB.dir/requires: CMakeFiles/Z64C_LIB.dir/src/transpos_table.c.o.requires
CMakeFiles/Z64C_LIB.dir/requires: CMakeFiles/Z64C_LIB.dir/src/uci.c.o.requires
CMakeFiles/Z64C_LIB.dir/requires: CMakeFiles/Z64C_LIB.dir/src/utils.c.o.requires

.PHONY : CMakeFiles/Z64C_LIB.dir/requires

CMakeFiles/Z64C_LIB.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/Z64C_LIB.dir/cmake_clean.cmake
.PHONY : CMakeFiles/Z64C_LIB.dir/clean

CMakeFiles/Z64C_LIB.dir/depend:
	cd /Users/pippoc/Z64C/target/Release && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /Users/pippoc/Z64C /Users/pippoc/Z64C /Users/pippoc/Z64C/target/Release /Users/pippoc/Z64C/target/Release /Users/pippoc/Z64C/target/Release/CMakeFiles/Z64C_LIB.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/Z64C_LIB.dir/depend

