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
CMAKE_SOURCE_DIR = /mnt/c/users/alex/desktop/open_spiel/open_spiel

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /mnt/c/users/alex/desktop/open_spiel/open_spiel/examples

# Include any dependencies generated for this target.
include abseil-cpp/absl/log/CMakeFiles/absl_log_internal_message.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include abseil-cpp/absl/log/CMakeFiles/absl_log_internal_message.dir/compiler_depend.make

# Include the progress variables for this target.
include abseil-cpp/absl/log/CMakeFiles/absl_log_internal_message.dir/progress.make

# Include the compile flags for this target's objects.
include abseil-cpp/absl/log/CMakeFiles/absl_log_internal_message.dir/flags.make

abseil-cpp/absl/log/CMakeFiles/absl_log_internal_message.dir/internal/log_message.cc.o: abseil-cpp/absl/log/CMakeFiles/absl_log_internal_message.dir/flags.make
abseil-cpp/absl/log/CMakeFiles/absl_log_internal_message.dir/internal/log_message.cc.o: ../abseil-cpp/absl/log/internal/log_message.cc
abseil-cpp/absl/log/CMakeFiles/absl_log_internal_message.dir/internal/log_message.cc.o: abseil-cpp/absl/log/CMakeFiles/absl_log_internal_message.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/mnt/c/users/alex/desktop/open_spiel/open_spiel/examples/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object abseil-cpp/absl/log/CMakeFiles/absl_log_internal_message.dir/internal/log_message.cc.o"
	cd /mnt/c/users/alex/desktop/open_spiel/open_spiel/examples/abseil-cpp/absl/log && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT abseil-cpp/absl/log/CMakeFiles/absl_log_internal_message.dir/internal/log_message.cc.o -MF CMakeFiles/absl_log_internal_message.dir/internal/log_message.cc.o.d -o CMakeFiles/absl_log_internal_message.dir/internal/log_message.cc.o -c /mnt/c/users/alex/desktop/open_spiel/open_spiel/abseil-cpp/absl/log/internal/log_message.cc

abseil-cpp/absl/log/CMakeFiles/absl_log_internal_message.dir/internal/log_message.cc.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/absl_log_internal_message.dir/internal/log_message.cc.i"
	cd /mnt/c/users/alex/desktop/open_spiel/open_spiel/examples/abseil-cpp/absl/log && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /mnt/c/users/alex/desktop/open_spiel/open_spiel/abseil-cpp/absl/log/internal/log_message.cc > CMakeFiles/absl_log_internal_message.dir/internal/log_message.cc.i

abseil-cpp/absl/log/CMakeFiles/absl_log_internal_message.dir/internal/log_message.cc.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/absl_log_internal_message.dir/internal/log_message.cc.s"
	cd /mnt/c/users/alex/desktop/open_spiel/open_spiel/examples/abseil-cpp/absl/log && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /mnt/c/users/alex/desktop/open_spiel/open_spiel/abseil-cpp/absl/log/internal/log_message.cc -o CMakeFiles/absl_log_internal_message.dir/internal/log_message.cc.s

# Object files for target absl_log_internal_message
absl_log_internal_message_OBJECTS = \
"CMakeFiles/absl_log_internal_message.dir/internal/log_message.cc.o"

# External object files for target absl_log_internal_message
absl_log_internal_message_EXTERNAL_OBJECTS =

abseil-cpp/absl/log/libabsl_log_internal_message.a: abseil-cpp/absl/log/CMakeFiles/absl_log_internal_message.dir/internal/log_message.cc.o
abseil-cpp/absl/log/libabsl_log_internal_message.a: abseil-cpp/absl/log/CMakeFiles/absl_log_internal_message.dir/build.make
abseil-cpp/absl/log/libabsl_log_internal_message.a: abseil-cpp/absl/log/CMakeFiles/absl_log_internal_message.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/mnt/c/users/alex/desktop/open_spiel/open_spiel/examples/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX static library libabsl_log_internal_message.a"
	cd /mnt/c/users/alex/desktop/open_spiel/open_spiel/examples/abseil-cpp/absl/log && $(CMAKE_COMMAND) -P CMakeFiles/absl_log_internal_message.dir/cmake_clean_target.cmake
	cd /mnt/c/users/alex/desktop/open_spiel/open_spiel/examples/abseil-cpp/absl/log && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/absl_log_internal_message.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
abseil-cpp/absl/log/CMakeFiles/absl_log_internal_message.dir/build: abseil-cpp/absl/log/libabsl_log_internal_message.a
.PHONY : abseil-cpp/absl/log/CMakeFiles/absl_log_internal_message.dir/build

abseil-cpp/absl/log/CMakeFiles/absl_log_internal_message.dir/clean:
	cd /mnt/c/users/alex/desktop/open_spiel/open_spiel/examples/abseil-cpp/absl/log && $(CMAKE_COMMAND) -P CMakeFiles/absl_log_internal_message.dir/cmake_clean.cmake
.PHONY : abseil-cpp/absl/log/CMakeFiles/absl_log_internal_message.dir/clean

abseil-cpp/absl/log/CMakeFiles/absl_log_internal_message.dir/depend:
	cd /mnt/c/users/alex/desktop/open_spiel/open_spiel/examples && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /mnt/c/users/alex/desktop/open_spiel/open_spiel /mnt/c/users/alex/desktop/open_spiel/open_spiel/abseil-cpp/absl/log /mnt/c/users/alex/desktop/open_spiel/open_spiel/examples /mnt/c/users/alex/desktop/open_spiel/open_spiel/examples/abseil-cpp/absl/log /mnt/c/users/alex/desktop/open_spiel/open_spiel/examples/abseil-cpp/absl/log/CMakeFiles/absl_log_internal_message.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : abseil-cpp/absl/log/CMakeFiles/absl_log_internal_message.dir/depend

