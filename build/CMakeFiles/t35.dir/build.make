# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.14

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
CMAKE_SOURCE_DIR = /home/paul/work/CV/DIP100

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/paul/work/CV/DIP100/build

# Include any dependencies generated for this target.
include CMakeFiles/t35.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/t35.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/t35.dir/flags.make

CMakeFiles/t35.dir/t35.cpp.o: CMakeFiles/t35.dir/flags.make
CMakeFiles/t35.dir/t35.cpp.o: ../t35.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/paul/work/CV/DIP100/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/t35.dir/t35.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/t35.dir/t35.cpp.o -c /home/paul/work/CV/DIP100/t35.cpp

CMakeFiles/t35.dir/t35.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/t35.dir/t35.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/paul/work/CV/DIP100/t35.cpp > CMakeFiles/t35.dir/t35.cpp.i

CMakeFiles/t35.dir/t35.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/t35.dir/t35.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/paul/work/CV/DIP100/t35.cpp -o CMakeFiles/t35.dir/t35.cpp.s

# Object files for target t35
t35_OBJECTS = \
"CMakeFiles/t35.dir/t35.cpp.o"

# External object files for target t35
t35_EXTERNAL_OBJECTS =

t35: CMakeFiles/t35.dir/t35.cpp.o
t35: CMakeFiles/t35.dir/build.make
t35: /usr/local/lib/libopencv_dnn.so.4.0.1
t35: /usr/local/lib/libopencv_ml.so.4.0.1
t35: /usr/local/lib/libopencv_objdetect.so.4.0.1
t35: /usr/local/lib/libopencv_photo.so.4.0.1
t35: /usr/local/lib/libopencv_stitching.so.4.0.1
t35: /usr/local/lib/libopencv_video.so.4.0.1
t35: /usr/local/lib/libopencv_calib3d.so.4.0.1
t35: /usr/local/lib/libopencv_features2d.so.4.0.1
t35: /usr/local/lib/libopencv_flann.so.4.0.1
t35: /usr/local/lib/libopencv_highgui.so.4.0.1
t35: /usr/local/lib/libopencv_videoio.so.4.0.1
t35: /usr/local/lib/libopencv_imgcodecs.so.4.0.1
t35: /usr/local/lib/libopencv_imgproc.so.4.0.1
t35: /usr/local/lib/libopencv_core.so.4.0.1
t35: CMakeFiles/t35.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/paul/work/CV/DIP100/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable t35"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/t35.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/t35.dir/build: t35

.PHONY : CMakeFiles/t35.dir/build

CMakeFiles/t35.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/t35.dir/cmake_clean.cmake
.PHONY : CMakeFiles/t35.dir/clean

CMakeFiles/t35.dir/depend:
	cd /home/paul/work/CV/DIP100/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/paul/work/CV/DIP100 /home/paul/work/CV/DIP100 /home/paul/work/CV/DIP100/build /home/paul/work/CV/DIP100/build /home/paul/work/CV/DIP100/build/CMakeFiles/t35.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/t35.dir/depend

