# CMAKE generated file: DO NOT EDIT!
# Generated by "MinGW Makefiles" Generator, CMake Version 3.28

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

SHELL = cmd.exe

# The CMake executable.
CMAKE_COMMAND = C:\msys64\mingw64\bin\cmake.exe

# The command to remove a file.
RM = C:\msys64\mingw64\bin\cmake.exe -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = D:\Programming\cpp\002_VSCode\001_Simulations\TMM

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = D:\Programming\cpp\002_VSCode\001_Simulations\TMM\build

# Include any dependencies generated for this target.
include src/manager/CMakeFiles/job_manager_lib.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include src/manager/CMakeFiles/job_manager_lib.dir/compiler_depend.make

# Include the progress variables for this target.
include src/manager/CMakeFiles/job_manager_lib.dir/progress.make

# Include the compile flags for this target's objects.
include src/manager/CMakeFiles/job_manager_lib.dir/flags.make

src/manager/CMakeFiles/job_manager_lib.dir/jobManager.cpp.obj: src/manager/CMakeFiles/job_manager_lib.dir/flags.make
src/manager/CMakeFiles/job_manager_lib.dir/jobManager.cpp.obj: src/manager/CMakeFiles/job_manager_lib.dir/includes_CXX.rsp
src/manager/CMakeFiles/job_manager_lib.dir/jobManager.cpp.obj: D:/Programming/cpp/002_VSCode/001_Simulations/TMM/src/manager/jobManager.cpp
src/manager/CMakeFiles/job_manager_lib.dir/jobManager.cpp.obj: src/manager/CMakeFiles/job_manager_lib.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=D:\Programming\cpp\002_VSCode\001_Simulations\TMM\build\CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object src/manager/CMakeFiles/job_manager_lib.dir/jobManager.cpp.obj"
	cd /d D:\Programming\cpp\002_VSCode\001_Simulations\TMM\build\src\manager && C:\msys64\mingw64\bin\g++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT src/manager/CMakeFiles/job_manager_lib.dir/jobManager.cpp.obj -MF CMakeFiles\job_manager_lib.dir\jobManager.cpp.obj.d -o CMakeFiles\job_manager_lib.dir\jobManager.cpp.obj -c D:\Programming\cpp\002_VSCode\001_Simulations\TMM\src\manager\jobManager.cpp

src/manager/CMakeFiles/job_manager_lib.dir/jobManager.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/job_manager_lib.dir/jobManager.cpp.i"
	cd /d D:\Programming\cpp\002_VSCode\001_Simulations\TMM\build\src\manager && C:\msys64\mingw64\bin\g++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E D:\Programming\cpp\002_VSCode\001_Simulations\TMM\src\manager\jobManager.cpp > CMakeFiles\job_manager_lib.dir\jobManager.cpp.i

src/manager/CMakeFiles/job_manager_lib.dir/jobManager.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/job_manager_lib.dir/jobManager.cpp.s"
	cd /d D:\Programming\cpp\002_VSCode\001_Simulations\TMM\build\src\manager && C:\msys64\mingw64\bin\g++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S D:\Programming\cpp\002_VSCode\001_Simulations\TMM\src\manager\jobManager.cpp -o CMakeFiles\job_manager_lib.dir\jobManager.cpp.s

# Object files for target job_manager_lib
job_manager_lib_OBJECTS = \
"CMakeFiles/job_manager_lib.dir/jobManager.cpp.obj"

# External object files for target job_manager_lib
job_manager_lib_EXTERNAL_OBJECTS =

src/manager/libjob_manager_lib.a: src/manager/CMakeFiles/job_manager_lib.dir/jobManager.cpp.obj
src/manager/libjob_manager_lib.a: src/manager/CMakeFiles/job_manager_lib.dir/build.make
src/manager/libjob_manager_lib.a: src/manager/CMakeFiles/job_manager_lib.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --bold --progress-dir=D:\Programming\cpp\002_VSCode\001_Simulations\TMM\build\CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX static library libjob_manager_lib.a"
	cd /d D:\Programming\cpp\002_VSCode\001_Simulations\TMM\build\src\manager && $(CMAKE_COMMAND) -P CMakeFiles\job_manager_lib.dir\cmake_clean_target.cmake
	cd /d D:\Programming\cpp\002_VSCode\001_Simulations\TMM\build\src\manager && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles\job_manager_lib.dir\link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
src/manager/CMakeFiles/job_manager_lib.dir/build: src/manager/libjob_manager_lib.a
.PHONY : src/manager/CMakeFiles/job_manager_lib.dir/build

src/manager/CMakeFiles/job_manager_lib.dir/clean:
	cd /d D:\Programming\cpp\002_VSCode\001_Simulations\TMM\build\src\manager && $(CMAKE_COMMAND) -P CMakeFiles\job_manager_lib.dir\cmake_clean.cmake
.PHONY : src/manager/CMakeFiles/job_manager_lib.dir/clean

src/manager/CMakeFiles/job_manager_lib.dir/depend:
	$(CMAKE_COMMAND) -E cmake_depends "MinGW Makefiles" D:\Programming\cpp\002_VSCode\001_Simulations\TMM D:\Programming\cpp\002_VSCode\001_Simulations\TMM\src\manager D:\Programming\cpp\002_VSCode\001_Simulations\TMM\build D:\Programming\cpp\002_VSCode\001_Simulations\TMM\build\src\manager D:\Programming\cpp\002_VSCode\001_Simulations\TMM\build\src\manager\CMakeFiles\job_manager_lib.dir\DependInfo.cmake "--color=$(COLOR)"
.PHONY : src/manager/CMakeFiles/job_manager_lib.dir/depend

