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
include abseil-cpp/absl/base/CMakeFiles/absl_base.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include abseil-cpp/absl/base/CMakeFiles/absl_base.dir/compiler_depend.make

# Include the progress variables for this target.
include abseil-cpp/absl/base/CMakeFiles/absl_base.dir/progress.make

# Include the compile flags for this target's objects.
include abseil-cpp/absl/base/CMakeFiles/absl_base.dir/flags.make

abseil-cpp/absl/base/CMakeFiles/absl_base.dir/internal/cycleclock.cc.o: abseil-cpp/absl/base/CMakeFiles/absl_base.dir/flags.make
abseil-cpp/absl/base/CMakeFiles/absl_base.dir/internal/cycleclock.cc.o: ../abseil-cpp/absl/base/internal/cycleclock.cc
abseil-cpp/absl/base/CMakeFiles/absl_base.dir/internal/cycleclock.cc.o: abseil-cpp/absl/base/CMakeFiles/absl_base.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/mnt/c/users/alex/desktop/open_spiel/open_spiel/examples/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object abseil-cpp/absl/base/CMakeFiles/absl_base.dir/internal/cycleclock.cc.o"
	cd /mnt/c/users/alex/desktop/open_spiel/open_spiel/examples/abseil-cpp/absl/base && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT abseil-cpp/absl/base/CMakeFiles/absl_base.dir/internal/cycleclock.cc.o -MF CMakeFiles/absl_base.dir/internal/cycleclock.cc.o.d -o CMakeFiles/absl_base.dir/internal/cycleclock.cc.o -c /mnt/c/users/alex/desktop/open_spiel/open_spiel/abseil-cpp/absl/base/internal/cycleclock.cc

abseil-cpp/absl/base/CMakeFiles/absl_base.dir/internal/cycleclock.cc.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/absl_base.dir/internal/cycleclock.cc.i"
	cd /mnt/c/users/alex/desktop/open_spiel/open_spiel/examples/abseil-cpp/absl/base && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /mnt/c/users/alex/desktop/open_spiel/open_spiel/abseil-cpp/absl/base/internal/cycleclock.cc > CMakeFiles/absl_base.dir/internal/cycleclock.cc.i

abseil-cpp/absl/base/CMakeFiles/absl_base.dir/internal/cycleclock.cc.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/absl_base.dir/internal/cycleclock.cc.s"
	cd /mnt/c/users/alex/desktop/open_spiel/open_spiel/examples/abseil-cpp/absl/base && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /mnt/c/users/alex/desktop/open_spiel/open_spiel/abseil-cpp/absl/base/internal/cycleclock.cc -o CMakeFiles/absl_base.dir/internal/cycleclock.cc.s

abseil-cpp/absl/base/CMakeFiles/absl_base.dir/internal/spinlock.cc.o: abseil-cpp/absl/base/CMakeFiles/absl_base.dir/flags.make
abseil-cpp/absl/base/CMakeFiles/absl_base.dir/internal/spinlock.cc.o: ../abseil-cpp/absl/base/internal/spinlock.cc
abseil-cpp/absl/base/CMakeFiles/absl_base.dir/internal/spinlock.cc.o: abseil-cpp/absl/base/CMakeFiles/absl_base.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/mnt/c/users/alex/desktop/open_spiel/open_spiel/examples/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object abseil-cpp/absl/base/CMakeFiles/absl_base.dir/internal/spinlock.cc.o"
	cd /mnt/c/users/alex/desktop/open_spiel/open_spiel/examples/abseil-cpp/absl/base && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT abseil-cpp/absl/base/CMakeFiles/absl_base.dir/internal/spinlock.cc.o -MF CMakeFiles/absl_base.dir/internal/spinlock.cc.o.d -o CMakeFiles/absl_base.dir/internal/spinlock.cc.o -c /mnt/c/users/alex/desktop/open_spiel/open_spiel/abseil-cpp/absl/base/internal/spinlock.cc

abseil-cpp/absl/base/CMakeFiles/absl_base.dir/internal/spinlock.cc.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/absl_base.dir/internal/spinlock.cc.i"
	cd /mnt/c/users/alex/desktop/open_spiel/open_spiel/examples/abseil-cpp/absl/base && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /mnt/c/users/alex/desktop/open_spiel/open_spiel/abseil-cpp/absl/base/internal/spinlock.cc > CMakeFiles/absl_base.dir/internal/spinlock.cc.i

abseil-cpp/absl/base/CMakeFiles/absl_base.dir/internal/spinlock.cc.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/absl_base.dir/internal/spinlock.cc.s"
	cd /mnt/c/users/alex/desktop/open_spiel/open_spiel/examples/abseil-cpp/absl/base && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /mnt/c/users/alex/desktop/open_spiel/open_spiel/abseil-cpp/absl/base/internal/spinlock.cc -o CMakeFiles/absl_base.dir/internal/spinlock.cc.s

abseil-cpp/absl/base/CMakeFiles/absl_base.dir/internal/sysinfo.cc.o: abseil-cpp/absl/base/CMakeFiles/absl_base.dir/flags.make
abseil-cpp/absl/base/CMakeFiles/absl_base.dir/internal/sysinfo.cc.o: ../abseil-cpp/absl/base/internal/sysinfo.cc
abseil-cpp/absl/base/CMakeFiles/absl_base.dir/internal/sysinfo.cc.o: abseil-cpp/absl/base/CMakeFiles/absl_base.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/mnt/c/users/alex/desktop/open_spiel/open_spiel/examples/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building CXX object abseil-cpp/absl/base/CMakeFiles/absl_base.dir/internal/sysinfo.cc.o"
	cd /mnt/c/users/alex/desktop/open_spiel/open_spiel/examples/abseil-cpp/absl/base && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT abseil-cpp/absl/base/CMakeFiles/absl_base.dir/internal/sysinfo.cc.o -MF CMakeFiles/absl_base.dir/internal/sysinfo.cc.o.d -o CMakeFiles/absl_base.dir/internal/sysinfo.cc.o -c /mnt/c/users/alex/desktop/open_spiel/open_spiel/abseil-cpp/absl/base/internal/sysinfo.cc

abseil-cpp/absl/base/CMakeFiles/absl_base.dir/internal/sysinfo.cc.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/absl_base.dir/internal/sysinfo.cc.i"
	cd /mnt/c/users/alex/desktop/open_spiel/open_spiel/examples/abseil-cpp/absl/base && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /mnt/c/users/alex/desktop/open_spiel/open_spiel/abseil-cpp/absl/base/internal/sysinfo.cc > CMakeFiles/absl_base.dir/internal/sysinfo.cc.i

abseil-cpp/absl/base/CMakeFiles/absl_base.dir/internal/sysinfo.cc.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/absl_base.dir/internal/sysinfo.cc.s"
	cd /mnt/c/users/alex/desktop/open_spiel/open_spiel/examples/abseil-cpp/absl/base && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /mnt/c/users/alex/desktop/open_spiel/open_spiel/abseil-cpp/absl/base/internal/sysinfo.cc -o CMakeFiles/absl_base.dir/internal/sysinfo.cc.s

abseil-cpp/absl/base/CMakeFiles/absl_base.dir/internal/thread_identity.cc.o: abseil-cpp/absl/base/CMakeFiles/absl_base.dir/flags.make
abseil-cpp/absl/base/CMakeFiles/absl_base.dir/internal/thread_identity.cc.o: ../abseil-cpp/absl/base/internal/thread_identity.cc
abseil-cpp/absl/base/CMakeFiles/absl_base.dir/internal/thread_identity.cc.o: abseil-cpp/absl/base/CMakeFiles/absl_base.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/mnt/c/users/alex/desktop/open_spiel/open_spiel/examples/CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Building CXX object abseil-cpp/absl/base/CMakeFiles/absl_base.dir/internal/thread_identity.cc.o"
	cd /mnt/c/users/alex/desktop/open_spiel/open_spiel/examples/abseil-cpp/absl/base && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT abseil-cpp/absl/base/CMakeFiles/absl_base.dir/internal/thread_identity.cc.o -MF CMakeFiles/absl_base.dir/internal/thread_identity.cc.o.d -o CMakeFiles/absl_base.dir/internal/thread_identity.cc.o -c /mnt/c/users/alex/desktop/open_spiel/open_spiel/abseil-cpp/absl/base/internal/thread_identity.cc

abseil-cpp/absl/base/CMakeFiles/absl_base.dir/internal/thread_identity.cc.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/absl_base.dir/internal/thread_identity.cc.i"
	cd /mnt/c/users/alex/desktop/open_spiel/open_spiel/examples/abseil-cpp/absl/base && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /mnt/c/users/alex/desktop/open_spiel/open_spiel/abseil-cpp/absl/base/internal/thread_identity.cc > CMakeFiles/absl_base.dir/internal/thread_identity.cc.i

abseil-cpp/absl/base/CMakeFiles/absl_base.dir/internal/thread_identity.cc.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/absl_base.dir/internal/thread_identity.cc.s"
	cd /mnt/c/users/alex/desktop/open_spiel/open_spiel/examples/abseil-cpp/absl/base && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /mnt/c/users/alex/desktop/open_spiel/open_spiel/abseil-cpp/absl/base/internal/thread_identity.cc -o CMakeFiles/absl_base.dir/internal/thread_identity.cc.s

abseil-cpp/absl/base/CMakeFiles/absl_base.dir/internal/unscaledcycleclock.cc.o: abseil-cpp/absl/base/CMakeFiles/absl_base.dir/flags.make
abseil-cpp/absl/base/CMakeFiles/absl_base.dir/internal/unscaledcycleclock.cc.o: ../abseil-cpp/absl/base/internal/unscaledcycleclock.cc
abseil-cpp/absl/base/CMakeFiles/absl_base.dir/internal/unscaledcycleclock.cc.o: abseil-cpp/absl/base/CMakeFiles/absl_base.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/mnt/c/users/alex/desktop/open_spiel/open_spiel/examples/CMakeFiles --progress-num=$(CMAKE_PROGRESS_5) "Building CXX object abseil-cpp/absl/base/CMakeFiles/absl_base.dir/internal/unscaledcycleclock.cc.o"
	cd /mnt/c/users/alex/desktop/open_spiel/open_spiel/examples/abseil-cpp/absl/base && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT abseil-cpp/absl/base/CMakeFiles/absl_base.dir/internal/unscaledcycleclock.cc.o -MF CMakeFiles/absl_base.dir/internal/unscaledcycleclock.cc.o.d -o CMakeFiles/absl_base.dir/internal/unscaledcycleclock.cc.o -c /mnt/c/users/alex/desktop/open_spiel/open_spiel/abseil-cpp/absl/base/internal/unscaledcycleclock.cc

abseil-cpp/absl/base/CMakeFiles/absl_base.dir/internal/unscaledcycleclock.cc.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/absl_base.dir/internal/unscaledcycleclock.cc.i"
	cd /mnt/c/users/alex/desktop/open_spiel/open_spiel/examples/abseil-cpp/absl/base && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /mnt/c/users/alex/desktop/open_spiel/open_spiel/abseil-cpp/absl/base/internal/unscaledcycleclock.cc > CMakeFiles/absl_base.dir/internal/unscaledcycleclock.cc.i

abseil-cpp/absl/base/CMakeFiles/absl_base.dir/internal/unscaledcycleclock.cc.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/absl_base.dir/internal/unscaledcycleclock.cc.s"
	cd /mnt/c/users/alex/desktop/open_spiel/open_spiel/examples/abseil-cpp/absl/base && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /mnt/c/users/alex/desktop/open_spiel/open_spiel/abseil-cpp/absl/base/internal/unscaledcycleclock.cc -o CMakeFiles/absl_base.dir/internal/unscaledcycleclock.cc.s

# Object files for target absl_base
absl_base_OBJECTS = \
"CMakeFiles/absl_base.dir/internal/cycleclock.cc.o" \
"CMakeFiles/absl_base.dir/internal/spinlock.cc.o" \
"CMakeFiles/absl_base.dir/internal/sysinfo.cc.o" \
"CMakeFiles/absl_base.dir/internal/thread_identity.cc.o" \
"CMakeFiles/absl_base.dir/internal/unscaledcycleclock.cc.o"

# External object files for target absl_base
absl_base_EXTERNAL_OBJECTS =

abseil-cpp/absl/base/libabsl_base.a: abseil-cpp/absl/base/CMakeFiles/absl_base.dir/internal/cycleclock.cc.o
abseil-cpp/absl/base/libabsl_base.a: abseil-cpp/absl/base/CMakeFiles/absl_base.dir/internal/spinlock.cc.o
abseil-cpp/absl/base/libabsl_base.a: abseil-cpp/absl/base/CMakeFiles/absl_base.dir/internal/sysinfo.cc.o
abseil-cpp/absl/base/libabsl_base.a: abseil-cpp/absl/base/CMakeFiles/absl_base.dir/internal/thread_identity.cc.o
abseil-cpp/absl/base/libabsl_base.a: abseil-cpp/absl/base/CMakeFiles/absl_base.dir/internal/unscaledcycleclock.cc.o
abseil-cpp/absl/base/libabsl_base.a: abseil-cpp/absl/base/CMakeFiles/absl_base.dir/build.make
abseil-cpp/absl/base/libabsl_base.a: abseil-cpp/absl/base/CMakeFiles/absl_base.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/mnt/c/users/alex/desktop/open_spiel/open_spiel/examples/CMakeFiles --progress-num=$(CMAKE_PROGRESS_6) "Linking CXX static library libabsl_base.a"
	cd /mnt/c/users/alex/desktop/open_spiel/open_spiel/examples/abseil-cpp/absl/base && $(CMAKE_COMMAND) -P CMakeFiles/absl_base.dir/cmake_clean_target.cmake
	cd /mnt/c/users/alex/desktop/open_spiel/open_spiel/examples/abseil-cpp/absl/base && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/absl_base.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
abseil-cpp/absl/base/CMakeFiles/absl_base.dir/build: abseil-cpp/absl/base/libabsl_base.a
.PHONY : abseil-cpp/absl/base/CMakeFiles/absl_base.dir/build

abseil-cpp/absl/base/CMakeFiles/absl_base.dir/clean:
	cd /mnt/c/users/alex/desktop/open_spiel/open_spiel/examples/abseil-cpp/absl/base && $(CMAKE_COMMAND) -P CMakeFiles/absl_base.dir/cmake_clean.cmake
.PHONY : abseil-cpp/absl/base/CMakeFiles/absl_base.dir/clean

abseil-cpp/absl/base/CMakeFiles/absl_base.dir/depend:
	cd /mnt/c/users/alex/desktop/open_spiel/open_spiel/examples && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /mnt/c/users/alex/desktop/open_spiel/open_spiel /mnt/c/users/alex/desktop/open_spiel/open_spiel/abseil-cpp/absl/base /mnt/c/users/alex/desktop/open_spiel/open_spiel/examples /mnt/c/users/alex/desktop/open_spiel/open_spiel/examples/abseil-cpp/absl/base /mnt/c/users/alex/desktop/open_spiel/open_spiel/examples/abseil-cpp/absl/base/CMakeFiles/absl_base.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : abseil-cpp/absl/base/CMakeFiles/absl_base.dir/depend

