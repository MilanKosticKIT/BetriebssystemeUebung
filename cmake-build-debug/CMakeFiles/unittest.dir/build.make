# CMAKE generated file: DO NOT EDIT!
# Generated by "MinGW Makefiles" Generator, CMake Version 3.12

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

SHELL = cmd.exe

# The CMake executable.
CMAKE_COMMAND = C:\Users\test\AppData\Local\JetBrains\Toolbox\apps\CLion\ch-0\182.4892.24\bin\cmake\win\bin\cmake.exe

# The command to remove a file.
RM = C:\Users\test\AppData\Local\JetBrains\Toolbox\apps\CLion\ch-0\182.4892.24\bin\cmake\win\bin\cmake.exe -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = C:\GitHub

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = C:\GitHub\cmake-build-debug

# Include any dependencies generated for this target.
include CMakeFiles/unittest.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/unittest.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/unittest.dir/flags.make

CMakeFiles/unittest.dir/src/blockdevice.cpp.obj: CMakeFiles/unittest.dir/flags.make
CMakeFiles/unittest.dir/src/blockdevice.cpp.obj: CMakeFiles/unittest.dir/includes_CXX.rsp
CMakeFiles/unittest.dir/src/blockdevice.cpp.obj: ../src/blockdevice.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=C:\GitHub\cmake-build-debug\CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/unittest.dir/src/blockdevice.cpp.obj"
	C:\PROGRA~2\MINGW-~1\I686-8~1.0-P\mingw32\bin\G__~1.EXE  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles\unittest.dir\src\blockdevice.cpp.obj -c C:\GitHub\src\blockdevice.cpp

CMakeFiles/unittest.dir/src/blockdevice.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/unittest.dir/src/blockdevice.cpp.i"
	C:\PROGRA~2\MINGW-~1\I686-8~1.0-P\mingw32\bin\G__~1.EXE $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E C:\GitHub\src\blockdevice.cpp > CMakeFiles\unittest.dir\src\blockdevice.cpp.i

CMakeFiles/unittest.dir/src/blockdevice.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/unittest.dir/src/blockdevice.cpp.s"
	C:\PROGRA~2\MINGW-~1\I686-8~1.0-P\mingw32\bin\G__~1.EXE $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S C:\GitHub\src\blockdevice.cpp -o CMakeFiles\unittest.dir\src\blockdevice.cpp.s

CMakeFiles/unittest.dir/src/myfs.cpp.obj: CMakeFiles/unittest.dir/flags.make
CMakeFiles/unittest.dir/src/myfs.cpp.obj: CMakeFiles/unittest.dir/includes_CXX.rsp
CMakeFiles/unittest.dir/src/myfs.cpp.obj: ../src/myfs.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=C:\GitHub\cmake-build-debug\CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object CMakeFiles/unittest.dir/src/myfs.cpp.obj"
	C:\PROGRA~2\MINGW-~1\I686-8~1.0-P\mingw32\bin\G__~1.EXE  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles\unittest.dir\src\myfs.cpp.obj -c C:\GitHub\src\myfs.cpp

CMakeFiles/unittest.dir/src/myfs.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/unittest.dir/src/myfs.cpp.i"
	C:\PROGRA~2\MINGW-~1\I686-8~1.0-P\mingw32\bin\G__~1.EXE $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E C:\GitHub\src\myfs.cpp > CMakeFiles\unittest.dir\src\myfs.cpp.i

CMakeFiles/unittest.dir/src/myfs.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/unittest.dir/src/myfs.cpp.s"
	C:\PROGRA~2\MINGW-~1\I686-8~1.0-P\mingw32\bin\G__~1.EXE $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S C:\GitHub\src\myfs.cpp -o CMakeFiles\unittest.dir\src\myfs.cpp.s

CMakeFiles/unittest.dir/unittests/main.cpp.obj: CMakeFiles/unittest.dir/flags.make
CMakeFiles/unittest.dir/unittests/main.cpp.obj: CMakeFiles/unittest.dir/includes_CXX.rsp
CMakeFiles/unittest.dir/unittests/main.cpp.obj: ../unittests/main.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=C:\GitHub\cmake-build-debug\CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building CXX object CMakeFiles/unittest.dir/unittests/main.cpp.obj"
	C:\PROGRA~2\MINGW-~1\I686-8~1.0-P\mingw32\bin\G__~1.EXE  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles\unittest.dir\unittests\main.cpp.obj -c C:\GitHub\unittests\main.cpp

CMakeFiles/unittest.dir/unittests/main.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/unittest.dir/unittests/main.cpp.i"
	C:\PROGRA~2\MINGW-~1\I686-8~1.0-P\mingw32\bin\G__~1.EXE $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E C:\GitHub\unittests\main.cpp > CMakeFiles\unittest.dir\unittests\main.cpp.i

CMakeFiles/unittest.dir/unittests/main.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/unittest.dir/unittests/main.cpp.s"
	C:\PROGRA~2\MINGW-~1\I686-8~1.0-P\mingw32\bin\G__~1.EXE $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S C:\GitHub\unittests\main.cpp -o CMakeFiles\unittest.dir\unittests\main.cpp.s

CMakeFiles/unittest.dir/unittests/test-blockdevice.cpp.obj: CMakeFiles/unittest.dir/flags.make
CMakeFiles/unittest.dir/unittests/test-blockdevice.cpp.obj: CMakeFiles/unittest.dir/includes_CXX.rsp
CMakeFiles/unittest.dir/unittests/test-blockdevice.cpp.obj: ../unittests/test-blockdevice.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=C:\GitHub\cmake-build-debug\CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Building CXX object CMakeFiles/unittest.dir/unittests/test-blockdevice.cpp.obj"
	C:\PROGRA~2\MINGW-~1\I686-8~1.0-P\mingw32\bin\G__~1.EXE  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles\unittest.dir\unittests\test-blockdevice.cpp.obj -c C:\GitHub\unittests\test-blockdevice.cpp

CMakeFiles/unittest.dir/unittests/test-blockdevice.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/unittest.dir/unittests/test-blockdevice.cpp.i"
	C:\PROGRA~2\MINGW-~1\I686-8~1.0-P\mingw32\bin\G__~1.EXE $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E C:\GitHub\unittests\test-blockdevice.cpp > CMakeFiles\unittest.dir\unittests\test-blockdevice.cpp.i

CMakeFiles/unittest.dir/unittests/test-blockdevice.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/unittest.dir/unittests/test-blockdevice.cpp.s"
	C:\PROGRA~2\MINGW-~1\I686-8~1.0-P\mingw32\bin\G__~1.EXE $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S C:\GitHub\unittests\test-blockdevice.cpp -o CMakeFiles\unittest.dir\unittests\test-blockdevice.cpp.s

CMakeFiles/unittest.dir/unittests/test-myfs.cpp.obj: CMakeFiles/unittest.dir/flags.make
CMakeFiles/unittest.dir/unittests/test-myfs.cpp.obj: CMakeFiles/unittest.dir/includes_CXX.rsp
CMakeFiles/unittest.dir/unittests/test-myfs.cpp.obj: ../unittests/test-myfs.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=C:\GitHub\cmake-build-debug\CMakeFiles --progress-num=$(CMAKE_PROGRESS_5) "Building CXX object CMakeFiles/unittest.dir/unittests/test-myfs.cpp.obj"
	C:\PROGRA~2\MINGW-~1\I686-8~1.0-P\mingw32\bin\G__~1.EXE  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles\unittest.dir\unittests\test-myfs.cpp.obj -c C:\GitHub\unittests\test-myfs.cpp

CMakeFiles/unittest.dir/unittests/test-myfs.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/unittest.dir/unittests/test-myfs.cpp.i"
	C:\PROGRA~2\MINGW-~1\I686-8~1.0-P\mingw32\bin\G__~1.EXE $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E C:\GitHub\unittests\test-myfs.cpp > CMakeFiles\unittest.dir\unittests\test-myfs.cpp.i

CMakeFiles/unittest.dir/unittests/test-myfs.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/unittest.dir/unittests/test-myfs.cpp.s"
	C:\PROGRA~2\MINGW-~1\I686-8~1.0-P\mingw32\bin\G__~1.EXE $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S C:\GitHub\unittests\test-myfs.cpp -o CMakeFiles\unittest.dir\unittests\test-myfs.cpp.s

CMakeFiles/unittest.dir/unittests/helper.cpp.obj: CMakeFiles/unittest.dir/flags.make
CMakeFiles/unittest.dir/unittests/helper.cpp.obj: CMakeFiles/unittest.dir/includes_CXX.rsp
CMakeFiles/unittest.dir/unittests/helper.cpp.obj: ../unittests/helper.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=C:\GitHub\cmake-build-debug\CMakeFiles --progress-num=$(CMAKE_PROGRESS_6) "Building CXX object CMakeFiles/unittest.dir/unittests/helper.cpp.obj"
	C:\PROGRA~2\MINGW-~1\I686-8~1.0-P\mingw32\bin\G__~1.EXE  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles\unittest.dir\unittests\helper.cpp.obj -c C:\GitHub\unittests\helper.cpp

CMakeFiles/unittest.dir/unittests/helper.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/unittest.dir/unittests/helper.cpp.i"
	C:\PROGRA~2\MINGW-~1\I686-8~1.0-P\mingw32\bin\G__~1.EXE $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E C:\GitHub\unittests\helper.cpp > CMakeFiles\unittest.dir\unittests\helper.cpp.i

CMakeFiles/unittest.dir/unittests/helper.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/unittest.dir/unittests/helper.cpp.s"
	C:\PROGRA~2\MINGW-~1\I686-8~1.0-P\mingw32\bin\G__~1.EXE $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S C:\GitHub\unittests\helper.cpp -o CMakeFiles\unittest.dir\unittests\helper.cpp.s

# Object files for target unittest
unittest_OBJECTS = \
"CMakeFiles/unittest.dir/src/blockdevice.cpp.obj" \
"CMakeFiles/unittest.dir/src/myfs.cpp.obj" \
"CMakeFiles/unittest.dir/unittests/main.cpp.obj" \
"CMakeFiles/unittest.dir/unittests/test-blockdevice.cpp.obj" \
"CMakeFiles/unittest.dir/unittests/test-myfs.cpp.obj" \
"CMakeFiles/unittest.dir/unittests/helper.cpp.obj"

# External object files for target unittest
unittest_EXTERNAL_OBJECTS =

bin/unittest.exe: CMakeFiles/unittest.dir/src/blockdevice.cpp.obj
bin/unittest.exe: CMakeFiles/unittest.dir/src/myfs.cpp.obj
bin/unittest.exe: CMakeFiles/unittest.dir/unittests/main.cpp.obj
bin/unittest.exe: CMakeFiles/unittest.dir/unittests/test-blockdevice.cpp.obj
bin/unittest.exe: CMakeFiles/unittest.dir/unittests/test-myfs.cpp.obj
bin/unittest.exe: CMakeFiles/unittest.dir/unittests/helper.cpp.obj
bin/unittest.exe: CMakeFiles/unittest.dir/build.make
bin/unittest.exe: CMakeFiles/unittest.dir/linklibs.rsp
bin/unittest.exe: CMakeFiles/unittest.dir/objects1.rsp
bin/unittest.exe: CMakeFiles/unittest.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=C:\GitHub\cmake-build-debug\CMakeFiles --progress-num=$(CMAKE_PROGRESS_7) "Linking CXX executable bin\unittest.exe"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles\unittest.dir\link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/unittest.dir/build: bin/unittest.exe

.PHONY : CMakeFiles/unittest.dir/build

CMakeFiles/unittest.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles\unittest.dir\cmake_clean.cmake
.PHONY : CMakeFiles/unittest.dir/clean

CMakeFiles/unittest.dir/depend:
	$(CMAKE_COMMAND) -E cmake_depends "MinGW Makefiles" C:\GitHub C:\GitHub C:\GitHub\cmake-build-debug C:\GitHub\cmake-build-debug C:\GitHub\cmake-build-debug\CMakeFiles\unittest.dir\DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/unittest.dir/depend

