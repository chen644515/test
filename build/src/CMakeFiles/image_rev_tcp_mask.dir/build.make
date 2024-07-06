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
CMAKE_SOURCE_DIR = /home/chenxx/slam_image_send

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/chenxx/slam_image_send/build

# Include any dependencies generated for this target.
include src/CMakeFiles/image_rev_tcp_mask.dir/depend.make

# Include the progress variables for this target.
include src/CMakeFiles/image_rev_tcp_mask.dir/progress.make

# Include the compile flags for this target's objects.
include src/CMakeFiles/image_rev_tcp_mask.dir/flags.make

src/CMakeFiles/image_rev_tcp_mask.dir/image_rev_tcp_mask.cpp.o: src/CMakeFiles/image_rev_tcp_mask.dir/flags.make
src/CMakeFiles/image_rev_tcp_mask.dir/image_rev_tcp_mask.cpp.o: ../src/image_rev_tcp_mask.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/chenxx/slam_image_send/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object src/CMakeFiles/image_rev_tcp_mask.dir/image_rev_tcp_mask.cpp.o"
	cd /home/chenxx/slam_image_send/build/src && /usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/image_rev_tcp_mask.dir/image_rev_tcp_mask.cpp.o -c /home/chenxx/slam_image_send/src/image_rev_tcp_mask.cpp

src/CMakeFiles/image_rev_tcp_mask.dir/image_rev_tcp_mask.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/image_rev_tcp_mask.dir/image_rev_tcp_mask.cpp.i"
	cd /home/chenxx/slam_image_send/build/src && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/chenxx/slam_image_send/src/image_rev_tcp_mask.cpp > CMakeFiles/image_rev_tcp_mask.dir/image_rev_tcp_mask.cpp.i

src/CMakeFiles/image_rev_tcp_mask.dir/image_rev_tcp_mask.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/image_rev_tcp_mask.dir/image_rev_tcp_mask.cpp.s"
	cd /home/chenxx/slam_image_send/build/src && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/chenxx/slam_image_send/src/image_rev_tcp_mask.cpp -o CMakeFiles/image_rev_tcp_mask.dir/image_rev_tcp_mask.cpp.s

# Object files for target image_rev_tcp_mask
image_rev_tcp_mask_OBJECTS = \
"CMakeFiles/image_rev_tcp_mask.dir/image_rev_tcp_mask.cpp.o"

# External object files for target image_rev_tcp_mask
image_rev_tcp_mask_EXTERNAL_OBJECTS =

../bin/image_rev_tcp_mask: src/CMakeFiles/image_rev_tcp_mask.dir/image_rev_tcp_mask.cpp.o
../bin/image_rev_tcp_mask: src/CMakeFiles/image_rev_tcp_mask.dir/build.make
../bin/image_rev_tcp_mask: /usr/lib/x86_64-linux-gnu/libopencv_stitching.so.4.2.0
../bin/image_rev_tcp_mask: /usr/lib/x86_64-linux-gnu/libopencv_aruco.so.4.2.0
../bin/image_rev_tcp_mask: /usr/lib/x86_64-linux-gnu/libopencv_bgsegm.so.4.2.0
../bin/image_rev_tcp_mask: /usr/lib/x86_64-linux-gnu/libopencv_bioinspired.so.4.2.0
../bin/image_rev_tcp_mask: /usr/lib/x86_64-linux-gnu/libopencv_ccalib.so.4.2.0
../bin/image_rev_tcp_mask: /usr/lib/x86_64-linux-gnu/libopencv_dnn_objdetect.so.4.2.0
../bin/image_rev_tcp_mask: /usr/lib/x86_64-linux-gnu/libopencv_dnn_superres.so.4.2.0
../bin/image_rev_tcp_mask: /usr/lib/x86_64-linux-gnu/libopencv_dpm.so.4.2.0
../bin/image_rev_tcp_mask: /usr/lib/x86_64-linux-gnu/libopencv_face.so.4.2.0
../bin/image_rev_tcp_mask: /usr/lib/x86_64-linux-gnu/libopencv_freetype.so.4.2.0
../bin/image_rev_tcp_mask: /usr/lib/x86_64-linux-gnu/libopencv_fuzzy.so.4.2.0
../bin/image_rev_tcp_mask: /usr/lib/x86_64-linux-gnu/libopencv_hdf.so.4.2.0
../bin/image_rev_tcp_mask: /usr/lib/x86_64-linux-gnu/libopencv_hfs.so.4.2.0
../bin/image_rev_tcp_mask: /usr/lib/x86_64-linux-gnu/libopencv_img_hash.so.4.2.0
../bin/image_rev_tcp_mask: /usr/lib/x86_64-linux-gnu/libopencv_line_descriptor.so.4.2.0
../bin/image_rev_tcp_mask: /usr/lib/x86_64-linux-gnu/libopencv_quality.so.4.2.0
../bin/image_rev_tcp_mask: /usr/lib/x86_64-linux-gnu/libopencv_reg.so.4.2.0
../bin/image_rev_tcp_mask: /usr/lib/x86_64-linux-gnu/libopencv_rgbd.so.4.2.0
../bin/image_rev_tcp_mask: /usr/lib/x86_64-linux-gnu/libopencv_saliency.so.4.2.0
../bin/image_rev_tcp_mask: /usr/lib/x86_64-linux-gnu/libopencv_shape.so.4.2.0
../bin/image_rev_tcp_mask: /usr/lib/x86_64-linux-gnu/libopencv_stereo.so.4.2.0
../bin/image_rev_tcp_mask: /usr/lib/x86_64-linux-gnu/libopencv_structured_light.so.4.2.0
../bin/image_rev_tcp_mask: /usr/lib/x86_64-linux-gnu/libopencv_superres.so.4.2.0
../bin/image_rev_tcp_mask: /usr/lib/x86_64-linux-gnu/libopencv_surface_matching.so.4.2.0
../bin/image_rev_tcp_mask: /usr/lib/x86_64-linux-gnu/libopencv_tracking.so.4.2.0
../bin/image_rev_tcp_mask: /usr/lib/x86_64-linux-gnu/libopencv_videostab.so.4.2.0
../bin/image_rev_tcp_mask: /usr/lib/x86_64-linux-gnu/libopencv_viz.so.4.2.0
../bin/image_rev_tcp_mask: /usr/lib/x86_64-linux-gnu/libopencv_xobjdetect.so.4.2.0
../bin/image_rev_tcp_mask: /usr/lib/x86_64-linux-gnu/libopencv_xphoto.so.4.2.0
../bin/image_rev_tcp_mask: /usr/lib/x86_64-linux-gnu/libopencv_highgui.so.4.2.0
../bin/image_rev_tcp_mask: /usr/lib/x86_64-linux-gnu/libopencv_datasets.so.4.2.0
../bin/image_rev_tcp_mask: /usr/lib/x86_64-linux-gnu/libopencv_plot.so.4.2.0
../bin/image_rev_tcp_mask: /usr/lib/x86_64-linux-gnu/libopencv_text.so.4.2.0
../bin/image_rev_tcp_mask: /usr/lib/x86_64-linux-gnu/libopencv_dnn.so.4.2.0
../bin/image_rev_tcp_mask: /usr/lib/x86_64-linux-gnu/libopencv_ml.so.4.2.0
../bin/image_rev_tcp_mask: /usr/lib/x86_64-linux-gnu/libopencv_phase_unwrapping.so.4.2.0
../bin/image_rev_tcp_mask: /usr/lib/x86_64-linux-gnu/libopencv_optflow.so.4.2.0
../bin/image_rev_tcp_mask: /usr/lib/x86_64-linux-gnu/libopencv_ximgproc.so.4.2.0
../bin/image_rev_tcp_mask: /usr/lib/x86_64-linux-gnu/libopencv_video.so.4.2.0
../bin/image_rev_tcp_mask: /usr/lib/x86_64-linux-gnu/libopencv_videoio.so.4.2.0
../bin/image_rev_tcp_mask: /usr/lib/x86_64-linux-gnu/libopencv_imgcodecs.so.4.2.0
../bin/image_rev_tcp_mask: /usr/lib/x86_64-linux-gnu/libopencv_objdetect.so.4.2.0
../bin/image_rev_tcp_mask: /usr/lib/x86_64-linux-gnu/libopencv_calib3d.so.4.2.0
../bin/image_rev_tcp_mask: /usr/lib/x86_64-linux-gnu/libopencv_features2d.so.4.2.0
../bin/image_rev_tcp_mask: /usr/lib/x86_64-linux-gnu/libopencv_flann.so.4.2.0
../bin/image_rev_tcp_mask: /usr/lib/x86_64-linux-gnu/libopencv_photo.so.4.2.0
../bin/image_rev_tcp_mask: /usr/lib/x86_64-linux-gnu/libopencv_imgproc.so.4.2.0
../bin/image_rev_tcp_mask: /usr/lib/x86_64-linux-gnu/libopencv_core.so.4.2.0
../bin/image_rev_tcp_mask: src/CMakeFiles/image_rev_tcp_mask.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/chenxx/slam_image_send/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable ../../bin/image_rev_tcp_mask"
	cd /home/chenxx/slam_image_send/build/src && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/image_rev_tcp_mask.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
src/CMakeFiles/image_rev_tcp_mask.dir/build: ../bin/image_rev_tcp_mask

.PHONY : src/CMakeFiles/image_rev_tcp_mask.dir/build

src/CMakeFiles/image_rev_tcp_mask.dir/clean:
	cd /home/chenxx/slam_image_send/build/src && $(CMAKE_COMMAND) -P CMakeFiles/image_rev_tcp_mask.dir/cmake_clean.cmake
.PHONY : src/CMakeFiles/image_rev_tcp_mask.dir/clean

src/CMakeFiles/image_rev_tcp_mask.dir/depend:
	cd /home/chenxx/slam_image_send/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/chenxx/slam_image_send /home/chenxx/slam_image_send/src /home/chenxx/slam_image_send/build /home/chenxx/slam_image_send/build/src /home/chenxx/slam_image_send/build/src/CMakeFiles/image_rev_tcp_mask.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : src/CMakeFiles/image_rev_tcp_mask.dir/depend

