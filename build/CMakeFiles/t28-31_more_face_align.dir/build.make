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
include CMakeFiles/t28-31_more_face_align.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/t28-31_more_face_align.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/t28-31_more_face_align.dir/flags.make

CMakeFiles/t28-31_more_face_align.dir/t28-31_more_face_align.cpp.o: CMakeFiles/t28-31_more_face_align.dir/flags.make
CMakeFiles/t28-31_more_face_align.dir/t28-31_more_face_align.cpp.o: ../t28-31_more_face_align.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/paul/work/CV/DIP100/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/t28-31_more_face_align.dir/t28-31_more_face_align.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/t28-31_more_face_align.dir/t28-31_more_face_align.cpp.o -c /home/paul/work/CV/DIP100/t28-31_more_face_align.cpp

CMakeFiles/t28-31_more_face_align.dir/t28-31_more_face_align.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/t28-31_more_face_align.dir/t28-31_more_face_align.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/paul/work/CV/DIP100/t28-31_more_face_align.cpp > CMakeFiles/t28-31_more_face_align.dir/t28-31_more_face_align.cpp.i

CMakeFiles/t28-31_more_face_align.dir/t28-31_more_face_align.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/t28-31_more_face_align.dir/t28-31_more_face_align.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/paul/work/CV/DIP100/t28-31_more_face_align.cpp -o CMakeFiles/t28-31_more_face_align.dir/t28-31_more_face_align.cpp.s

# Object files for target t28-31_more_face_align
t28__31_more_face_align_OBJECTS = \
"CMakeFiles/t28-31_more_face_align.dir/t28-31_more_face_align.cpp.o"

# External object files for target t28-31_more_face_align
t28__31_more_face_align_EXTERNAL_OBJECTS =

t28-31_more_face_align: CMakeFiles/t28-31_more_face_align.dir/t28-31_more_face_align.cpp.o
t28-31_more_face_align: CMakeFiles/t28-31_more_face_align.dir/build.make
t28-31_more_face_align: /usr/local/lib/libopencv_dnn.so.4.0.1
t28-31_more_face_align: /usr/local/lib/libopencv_ml.so.4.0.1
t28-31_more_face_align: /usr/local/lib/libopencv_objdetect.so.4.0.1
t28-31_more_face_align: /usr/local/lib/libopencv_photo.so.4.0.1
t28-31_more_face_align: /usr/local/lib/libopencv_stitching.so.4.0.1
t28-31_more_face_align: /usr/local/lib/libopencv_video.so.4.0.1
t28-31_more_face_align: /usr/local/lib/libopencv_calib3d.so.4.0.1
t28-31_more_face_align: /usr/local/lib/libopencv_features2d.so.4.0.1
t28-31_more_face_align: /usr/local/lib/libopencv_flann.so.4.0.1
t28-31_more_face_align: /usr/local/lib/libopencv_highgui.so.4.0.1
t28-31_more_face_align: /usr/local/lib/libopencv_videoio.so.4.0.1
t28-31_more_face_align: /usr/local/lib/libopencv_imgcodecs.so.4.0.1
t28-31_more_face_align: /usr/local/lib/libopencv_imgproc.so.4.0.1
t28-31_more_face_align: /usr/local/lib/libopencv_core.so.4.0.1
t28-31_more_face_align: CMakeFiles/t28-31_more_face_align.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/paul/work/CV/DIP100/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable t28-31_more_face_align"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/t28-31_more_face_align.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/t28-31_more_face_align.dir/build: t28-31_more_face_align

.PHONY : CMakeFiles/t28-31_more_face_align.dir/build

CMakeFiles/t28-31_more_face_align.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/t28-31_more_face_align.dir/cmake_clean.cmake
.PHONY : CMakeFiles/t28-31_more_face_align.dir/clean

CMakeFiles/t28-31_more_face_align.dir/depend:
	cd /home/paul/work/CV/DIP100/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/paul/work/CV/DIP100 /home/paul/work/CV/DIP100 /home/paul/work/CV/DIP100/build /home/paul/work/CV/DIP100/build /home/paul/work/CV/DIP100/build/CMakeFiles/t28-31_more_face_align.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/t28-31_more_face_align.dir/depend

