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
CMAKE_COMMAND = /snap/clion/123/bin/cmake/linux/bin/cmake

# The command to remove a file.
RM = /snap/clion/123/bin/cmake/linux/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/mahanpc/Desktop/AP/Project/Backup

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/mahanpc/Desktop/AP/Project/Backup/cmake-build-debug

# Include any dependencies generated for this target.
include CMakeFiles/Project.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/Project.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/Project.dir/flags.make

CMakeFiles/Project.dir/main.cpp.o: CMakeFiles/Project.dir/flags.make
CMakeFiles/Project.dir/main.cpp.o: ../main.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/mahanpc/Desktop/AP/Project/Backup/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/Project.dir/main.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/Project.dir/main.cpp.o -c /home/mahanpc/Desktop/AP/Project/Backup/main.cpp

CMakeFiles/Project.dir/main.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/Project.dir/main.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/mahanpc/Desktop/AP/Project/Backup/main.cpp > CMakeFiles/Project.dir/main.cpp.i

CMakeFiles/Project.dir/main.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/Project.dir/main.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/mahanpc/Desktop/AP/Project/Backup/main.cpp -o CMakeFiles/Project.dir/main.cpp.s

# Object files for target Project
Project_OBJECTS = \
"CMakeFiles/Project.dir/main.cpp.o"

# External object files for target Project
Project_EXTERNAL_OBJECTS =

Project: CMakeFiles/Project.dir/main.cpp.o
Project: CMakeFiles/Project.dir/build.make
Project: CMakeFiles/Project.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/mahanpc/Desktop/AP/Project/Backup/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable Project"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/Project.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/Project.dir/build: Project

.PHONY : CMakeFiles/Project.dir/build

CMakeFiles/Project.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/Project.dir/cmake_clean.cmake
.PHONY : CMakeFiles/Project.dir/clean

CMakeFiles/Project.dir/depend:
	cd /home/mahanpc/Desktop/AP/Project/Backup/cmake-build-debug && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/mahanpc/Desktop/AP/Project/Backup /home/mahanpc/Desktop/AP/Project/Backup /home/mahanpc/Desktop/AP/Project/Backup/cmake-build-debug /home/mahanpc/Desktop/AP/Project/Backup/cmake-build-debug /home/mahanpc/Desktop/AP/Project/Backup/cmake-build-debug/CMakeFiles/Project.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/Project.dir/depend

