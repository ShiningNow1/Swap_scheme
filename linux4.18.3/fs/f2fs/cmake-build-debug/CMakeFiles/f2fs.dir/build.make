# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.8

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
CMAKE_COMMAND = /usr/share/clion/bin/cmake/bin/cmake

# The command to remove a file.
RM = /usr/share/clion/bin/cmake/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/cxy/hybridStorage/linux4.18.3/fs/f2fs

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/cxy/hybridStorage/linux4.18.3/fs/f2fs/cmake-build-debug

# Include any dependencies generated for this target.
include CMakeFiles/f2fs.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/f2fs.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/f2fs.dir/flags.make

CMakeFiles/f2fs.dir/acl.c.o: CMakeFiles/f2fs.dir/flags.make
CMakeFiles/f2fs.dir/acl.c.o: ../acl.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/cxy/hybridStorage/linux4.18.3/fs/f2fs/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building C object CMakeFiles/f2fs.dir/acl.c.o"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/f2fs.dir/acl.c.o   -c /home/cxy/hybridStorage/linux4.18.3/fs/f2fs/acl.c

CMakeFiles/f2fs.dir/acl.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/f2fs.dir/acl.c.i"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/cxy/hybridStorage/linux4.18.3/fs/f2fs/acl.c > CMakeFiles/f2fs.dir/acl.c.i

CMakeFiles/f2fs.dir/acl.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/f2fs.dir/acl.c.s"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/cxy/hybridStorage/linux4.18.3/fs/f2fs/acl.c -o CMakeFiles/f2fs.dir/acl.c.s

CMakeFiles/f2fs.dir/acl.c.o.requires:

.PHONY : CMakeFiles/f2fs.dir/acl.c.o.requires

CMakeFiles/f2fs.dir/acl.c.o.provides: CMakeFiles/f2fs.dir/acl.c.o.requires
	$(MAKE) -f CMakeFiles/f2fs.dir/build.make CMakeFiles/f2fs.dir/acl.c.o.provides.build
.PHONY : CMakeFiles/f2fs.dir/acl.c.o.provides

CMakeFiles/f2fs.dir/acl.c.o.provides.build: CMakeFiles/f2fs.dir/acl.c.o


CMakeFiles/f2fs.dir/checkpoint.c.o: CMakeFiles/f2fs.dir/flags.make
CMakeFiles/f2fs.dir/checkpoint.c.o: ../checkpoint.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/cxy/hybridStorage/linux4.18.3/fs/f2fs/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building C object CMakeFiles/f2fs.dir/checkpoint.c.o"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/f2fs.dir/checkpoint.c.o   -c /home/cxy/hybridStorage/linux4.18.3/fs/f2fs/checkpoint.c

CMakeFiles/f2fs.dir/checkpoint.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/f2fs.dir/checkpoint.c.i"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/cxy/hybridStorage/linux4.18.3/fs/f2fs/checkpoint.c > CMakeFiles/f2fs.dir/checkpoint.c.i

CMakeFiles/f2fs.dir/checkpoint.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/f2fs.dir/checkpoint.c.s"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/cxy/hybridStorage/linux4.18.3/fs/f2fs/checkpoint.c -o CMakeFiles/f2fs.dir/checkpoint.c.s

CMakeFiles/f2fs.dir/checkpoint.c.o.requires:

.PHONY : CMakeFiles/f2fs.dir/checkpoint.c.o.requires

CMakeFiles/f2fs.dir/checkpoint.c.o.provides: CMakeFiles/f2fs.dir/checkpoint.c.o.requires
	$(MAKE) -f CMakeFiles/f2fs.dir/build.make CMakeFiles/f2fs.dir/checkpoint.c.o.provides.build
.PHONY : CMakeFiles/f2fs.dir/checkpoint.c.o.provides

CMakeFiles/f2fs.dir/checkpoint.c.o.provides.build: CMakeFiles/f2fs.dir/checkpoint.c.o


CMakeFiles/f2fs.dir/data.c.o: CMakeFiles/f2fs.dir/flags.make
CMakeFiles/f2fs.dir/data.c.o: ../data.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/cxy/hybridStorage/linux4.18.3/fs/f2fs/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building C object CMakeFiles/f2fs.dir/data.c.o"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/f2fs.dir/data.c.o   -c /home/cxy/hybridStorage/linux4.18.3/fs/f2fs/data.c

CMakeFiles/f2fs.dir/data.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/f2fs.dir/data.c.i"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/cxy/hybridStorage/linux4.18.3/fs/f2fs/data.c > CMakeFiles/f2fs.dir/data.c.i

CMakeFiles/f2fs.dir/data.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/f2fs.dir/data.c.s"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/cxy/hybridStorage/linux4.18.3/fs/f2fs/data.c -o CMakeFiles/f2fs.dir/data.c.s

CMakeFiles/f2fs.dir/data.c.o.requires:

.PHONY : CMakeFiles/f2fs.dir/data.c.o.requires

CMakeFiles/f2fs.dir/data.c.o.provides: CMakeFiles/f2fs.dir/data.c.o.requires
	$(MAKE) -f CMakeFiles/f2fs.dir/build.make CMakeFiles/f2fs.dir/data.c.o.provides.build
.PHONY : CMakeFiles/f2fs.dir/data.c.o.provides

CMakeFiles/f2fs.dir/data.c.o.provides.build: CMakeFiles/f2fs.dir/data.c.o


CMakeFiles/f2fs.dir/debug.c.o: CMakeFiles/f2fs.dir/flags.make
CMakeFiles/f2fs.dir/debug.c.o: ../debug.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/cxy/hybridStorage/linux4.18.3/fs/f2fs/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Building C object CMakeFiles/f2fs.dir/debug.c.o"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/f2fs.dir/debug.c.o   -c /home/cxy/hybridStorage/linux4.18.3/fs/f2fs/debug.c

CMakeFiles/f2fs.dir/debug.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/f2fs.dir/debug.c.i"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/cxy/hybridStorage/linux4.18.3/fs/f2fs/debug.c > CMakeFiles/f2fs.dir/debug.c.i

CMakeFiles/f2fs.dir/debug.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/f2fs.dir/debug.c.s"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/cxy/hybridStorage/linux4.18.3/fs/f2fs/debug.c -o CMakeFiles/f2fs.dir/debug.c.s

CMakeFiles/f2fs.dir/debug.c.o.requires:

.PHONY : CMakeFiles/f2fs.dir/debug.c.o.requires

CMakeFiles/f2fs.dir/debug.c.o.provides: CMakeFiles/f2fs.dir/debug.c.o.requires
	$(MAKE) -f CMakeFiles/f2fs.dir/build.make CMakeFiles/f2fs.dir/debug.c.o.provides.build
.PHONY : CMakeFiles/f2fs.dir/debug.c.o.provides

CMakeFiles/f2fs.dir/debug.c.o.provides.build: CMakeFiles/f2fs.dir/debug.c.o


CMakeFiles/f2fs.dir/dir.c.o: CMakeFiles/f2fs.dir/flags.make
CMakeFiles/f2fs.dir/dir.c.o: ../dir.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/cxy/hybridStorage/linux4.18.3/fs/f2fs/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_5) "Building C object CMakeFiles/f2fs.dir/dir.c.o"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/f2fs.dir/dir.c.o   -c /home/cxy/hybridStorage/linux4.18.3/fs/f2fs/dir.c

CMakeFiles/f2fs.dir/dir.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/f2fs.dir/dir.c.i"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/cxy/hybridStorage/linux4.18.3/fs/f2fs/dir.c > CMakeFiles/f2fs.dir/dir.c.i

CMakeFiles/f2fs.dir/dir.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/f2fs.dir/dir.c.s"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/cxy/hybridStorage/linux4.18.3/fs/f2fs/dir.c -o CMakeFiles/f2fs.dir/dir.c.s

CMakeFiles/f2fs.dir/dir.c.o.requires:

.PHONY : CMakeFiles/f2fs.dir/dir.c.o.requires

CMakeFiles/f2fs.dir/dir.c.o.provides: CMakeFiles/f2fs.dir/dir.c.o.requires
	$(MAKE) -f CMakeFiles/f2fs.dir/build.make CMakeFiles/f2fs.dir/dir.c.o.provides.build
.PHONY : CMakeFiles/f2fs.dir/dir.c.o.provides

CMakeFiles/f2fs.dir/dir.c.o.provides.build: CMakeFiles/f2fs.dir/dir.c.o


CMakeFiles/f2fs.dir/extent_cache.c.o: CMakeFiles/f2fs.dir/flags.make
CMakeFiles/f2fs.dir/extent_cache.c.o: ../extent_cache.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/cxy/hybridStorage/linux4.18.3/fs/f2fs/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_6) "Building C object CMakeFiles/f2fs.dir/extent_cache.c.o"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/f2fs.dir/extent_cache.c.o   -c /home/cxy/hybridStorage/linux4.18.3/fs/f2fs/extent_cache.c

CMakeFiles/f2fs.dir/extent_cache.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/f2fs.dir/extent_cache.c.i"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/cxy/hybridStorage/linux4.18.3/fs/f2fs/extent_cache.c > CMakeFiles/f2fs.dir/extent_cache.c.i

CMakeFiles/f2fs.dir/extent_cache.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/f2fs.dir/extent_cache.c.s"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/cxy/hybridStorage/linux4.18.3/fs/f2fs/extent_cache.c -o CMakeFiles/f2fs.dir/extent_cache.c.s

CMakeFiles/f2fs.dir/extent_cache.c.o.requires:

.PHONY : CMakeFiles/f2fs.dir/extent_cache.c.o.requires

CMakeFiles/f2fs.dir/extent_cache.c.o.provides: CMakeFiles/f2fs.dir/extent_cache.c.o.requires
	$(MAKE) -f CMakeFiles/f2fs.dir/build.make CMakeFiles/f2fs.dir/extent_cache.c.o.provides.build
.PHONY : CMakeFiles/f2fs.dir/extent_cache.c.o.provides

CMakeFiles/f2fs.dir/extent_cache.c.o.provides.build: CMakeFiles/f2fs.dir/extent_cache.c.o


CMakeFiles/f2fs.dir/file.c.o: CMakeFiles/f2fs.dir/flags.make
CMakeFiles/f2fs.dir/file.c.o: ../file.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/cxy/hybridStorage/linux4.18.3/fs/f2fs/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_7) "Building C object CMakeFiles/f2fs.dir/file.c.o"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/f2fs.dir/file.c.o   -c /home/cxy/hybridStorage/linux4.18.3/fs/f2fs/file.c

CMakeFiles/f2fs.dir/file.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/f2fs.dir/file.c.i"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/cxy/hybridStorage/linux4.18.3/fs/f2fs/file.c > CMakeFiles/f2fs.dir/file.c.i

CMakeFiles/f2fs.dir/file.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/f2fs.dir/file.c.s"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/cxy/hybridStorage/linux4.18.3/fs/f2fs/file.c -o CMakeFiles/f2fs.dir/file.c.s

CMakeFiles/f2fs.dir/file.c.o.requires:

.PHONY : CMakeFiles/f2fs.dir/file.c.o.requires

CMakeFiles/f2fs.dir/file.c.o.provides: CMakeFiles/f2fs.dir/file.c.o.requires
	$(MAKE) -f CMakeFiles/f2fs.dir/build.make CMakeFiles/f2fs.dir/file.c.o.provides.build
.PHONY : CMakeFiles/f2fs.dir/file.c.o.provides

CMakeFiles/f2fs.dir/file.c.o.provides.build: CMakeFiles/f2fs.dir/file.c.o


CMakeFiles/f2fs.dir/gc.c.o: CMakeFiles/f2fs.dir/flags.make
CMakeFiles/f2fs.dir/gc.c.o: ../gc.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/cxy/hybridStorage/linux4.18.3/fs/f2fs/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_8) "Building C object CMakeFiles/f2fs.dir/gc.c.o"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/f2fs.dir/gc.c.o   -c /home/cxy/hybridStorage/linux4.18.3/fs/f2fs/gc.c

CMakeFiles/f2fs.dir/gc.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/f2fs.dir/gc.c.i"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/cxy/hybridStorage/linux4.18.3/fs/f2fs/gc.c > CMakeFiles/f2fs.dir/gc.c.i

CMakeFiles/f2fs.dir/gc.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/f2fs.dir/gc.c.s"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/cxy/hybridStorage/linux4.18.3/fs/f2fs/gc.c -o CMakeFiles/f2fs.dir/gc.c.s

CMakeFiles/f2fs.dir/gc.c.o.requires:

.PHONY : CMakeFiles/f2fs.dir/gc.c.o.requires

CMakeFiles/f2fs.dir/gc.c.o.provides: CMakeFiles/f2fs.dir/gc.c.o.requires
	$(MAKE) -f CMakeFiles/f2fs.dir/build.make CMakeFiles/f2fs.dir/gc.c.o.provides.build
.PHONY : CMakeFiles/f2fs.dir/gc.c.o.provides

CMakeFiles/f2fs.dir/gc.c.o.provides.build: CMakeFiles/f2fs.dir/gc.c.o


CMakeFiles/f2fs.dir/gc_nvm.c.o: CMakeFiles/f2fs.dir/flags.make
CMakeFiles/f2fs.dir/gc_nvm.c.o: ../gc_nvm.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/cxy/hybridStorage/linux4.18.3/fs/f2fs/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_9) "Building C object CMakeFiles/f2fs.dir/gc_nvm.c.o"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/f2fs.dir/gc_nvm.c.o   -c /home/cxy/hybridStorage/linux4.18.3/fs/f2fs/gc_nvm.c

CMakeFiles/f2fs.dir/gc_nvm.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/f2fs.dir/gc_nvm.c.i"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/cxy/hybridStorage/linux4.18.3/fs/f2fs/gc_nvm.c > CMakeFiles/f2fs.dir/gc_nvm.c.i

CMakeFiles/f2fs.dir/gc_nvm.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/f2fs.dir/gc_nvm.c.s"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/cxy/hybridStorage/linux4.18.3/fs/f2fs/gc_nvm.c -o CMakeFiles/f2fs.dir/gc_nvm.c.s

CMakeFiles/f2fs.dir/gc_nvm.c.o.requires:

.PHONY : CMakeFiles/f2fs.dir/gc_nvm.c.o.requires

CMakeFiles/f2fs.dir/gc_nvm.c.o.provides: CMakeFiles/f2fs.dir/gc_nvm.c.o.requires
	$(MAKE) -f CMakeFiles/f2fs.dir/build.make CMakeFiles/f2fs.dir/gc_nvm.c.o.provides.build
.PHONY : CMakeFiles/f2fs.dir/gc_nvm.c.o.provides

CMakeFiles/f2fs.dir/gc_nvm.c.o.provides.build: CMakeFiles/f2fs.dir/gc_nvm.c.o


CMakeFiles/f2fs.dir/hash.c.o: CMakeFiles/f2fs.dir/flags.make
CMakeFiles/f2fs.dir/hash.c.o: ../hash.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/cxy/hybridStorage/linux4.18.3/fs/f2fs/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_10) "Building C object CMakeFiles/f2fs.dir/hash.c.o"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/f2fs.dir/hash.c.o   -c /home/cxy/hybridStorage/linux4.18.3/fs/f2fs/hash.c

CMakeFiles/f2fs.dir/hash.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/f2fs.dir/hash.c.i"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/cxy/hybridStorage/linux4.18.3/fs/f2fs/hash.c > CMakeFiles/f2fs.dir/hash.c.i

CMakeFiles/f2fs.dir/hash.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/f2fs.dir/hash.c.s"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/cxy/hybridStorage/linux4.18.3/fs/f2fs/hash.c -o CMakeFiles/f2fs.dir/hash.c.s

CMakeFiles/f2fs.dir/hash.c.o.requires:

.PHONY : CMakeFiles/f2fs.dir/hash.c.o.requires

CMakeFiles/f2fs.dir/hash.c.o.provides: CMakeFiles/f2fs.dir/hash.c.o.requires
	$(MAKE) -f CMakeFiles/f2fs.dir/build.make CMakeFiles/f2fs.dir/hash.c.o.provides.build
.PHONY : CMakeFiles/f2fs.dir/hash.c.o.provides

CMakeFiles/f2fs.dir/hash.c.o.provides.build: CMakeFiles/f2fs.dir/hash.c.o


CMakeFiles/f2fs.dir/inline.c.o: CMakeFiles/f2fs.dir/flags.make
CMakeFiles/f2fs.dir/inline.c.o: ../inline.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/cxy/hybridStorage/linux4.18.3/fs/f2fs/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_11) "Building C object CMakeFiles/f2fs.dir/inline.c.o"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/f2fs.dir/inline.c.o   -c /home/cxy/hybridStorage/linux4.18.3/fs/f2fs/inline.c

CMakeFiles/f2fs.dir/inline.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/f2fs.dir/inline.c.i"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/cxy/hybridStorage/linux4.18.3/fs/f2fs/inline.c > CMakeFiles/f2fs.dir/inline.c.i

CMakeFiles/f2fs.dir/inline.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/f2fs.dir/inline.c.s"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/cxy/hybridStorage/linux4.18.3/fs/f2fs/inline.c -o CMakeFiles/f2fs.dir/inline.c.s

CMakeFiles/f2fs.dir/inline.c.o.requires:

.PHONY : CMakeFiles/f2fs.dir/inline.c.o.requires

CMakeFiles/f2fs.dir/inline.c.o.provides: CMakeFiles/f2fs.dir/inline.c.o.requires
	$(MAKE) -f CMakeFiles/f2fs.dir/build.make CMakeFiles/f2fs.dir/inline.c.o.provides.build
.PHONY : CMakeFiles/f2fs.dir/inline.c.o.provides

CMakeFiles/f2fs.dir/inline.c.o.provides.build: CMakeFiles/f2fs.dir/inline.c.o


CMakeFiles/f2fs.dir/inode.c.o: CMakeFiles/f2fs.dir/flags.make
CMakeFiles/f2fs.dir/inode.c.o: ../inode.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/cxy/hybridStorage/linux4.18.3/fs/f2fs/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_12) "Building C object CMakeFiles/f2fs.dir/inode.c.o"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/f2fs.dir/inode.c.o   -c /home/cxy/hybridStorage/linux4.18.3/fs/f2fs/inode.c

CMakeFiles/f2fs.dir/inode.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/f2fs.dir/inode.c.i"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/cxy/hybridStorage/linux4.18.3/fs/f2fs/inode.c > CMakeFiles/f2fs.dir/inode.c.i

CMakeFiles/f2fs.dir/inode.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/f2fs.dir/inode.c.s"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/cxy/hybridStorage/linux4.18.3/fs/f2fs/inode.c -o CMakeFiles/f2fs.dir/inode.c.s

CMakeFiles/f2fs.dir/inode.c.o.requires:

.PHONY : CMakeFiles/f2fs.dir/inode.c.o.requires

CMakeFiles/f2fs.dir/inode.c.o.provides: CMakeFiles/f2fs.dir/inode.c.o.requires
	$(MAKE) -f CMakeFiles/f2fs.dir/build.make CMakeFiles/f2fs.dir/inode.c.o.provides.build
.PHONY : CMakeFiles/f2fs.dir/inode.c.o.provides

CMakeFiles/f2fs.dir/inode.c.o.provides.build: CMakeFiles/f2fs.dir/inode.c.o


CMakeFiles/f2fs.dir/namei.c.o: CMakeFiles/f2fs.dir/flags.make
CMakeFiles/f2fs.dir/namei.c.o: ../namei.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/cxy/hybridStorage/linux4.18.3/fs/f2fs/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_13) "Building C object CMakeFiles/f2fs.dir/namei.c.o"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/f2fs.dir/namei.c.o   -c /home/cxy/hybridStorage/linux4.18.3/fs/f2fs/namei.c

CMakeFiles/f2fs.dir/namei.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/f2fs.dir/namei.c.i"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/cxy/hybridStorage/linux4.18.3/fs/f2fs/namei.c > CMakeFiles/f2fs.dir/namei.c.i

CMakeFiles/f2fs.dir/namei.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/f2fs.dir/namei.c.s"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/cxy/hybridStorage/linux4.18.3/fs/f2fs/namei.c -o CMakeFiles/f2fs.dir/namei.c.s

CMakeFiles/f2fs.dir/namei.c.o.requires:

.PHONY : CMakeFiles/f2fs.dir/namei.c.o.requires

CMakeFiles/f2fs.dir/namei.c.o.provides: CMakeFiles/f2fs.dir/namei.c.o.requires
	$(MAKE) -f CMakeFiles/f2fs.dir/build.make CMakeFiles/f2fs.dir/namei.c.o.provides.build
.PHONY : CMakeFiles/f2fs.dir/namei.c.o.provides

CMakeFiles/f2fs.dir/namei.c.o.provides.build: CMakeFiles/f2fs.dir/namei.c.o


CMakeFiles/f2fs.dir/node.c.o: CMakeFiles/f2fs.dir/flags.make
CMakeFiles/f2fs.dir/node.c.o: ../node.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/cxy/hybridStorage/linux4.18.3/fs/f2fs/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_14) "Building C object CMakeFiles/f2fs.dir/node.c.o"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/f2fs.dir/node.c.o   -c /home/cxy/hybridStorage/linux4.18.3/fs/f2fs/node.c

CMakeFiles/f2fs.dir/node.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/f2fs.dir/node.c.i"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/cxy/hybridStorage/linux4.18.3/fs/f2fs/node.c > CMakeFiles/f2fs.dir/node.c.i

CMakeFiles/f2fs.dir/node.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/f2fs.dir/node.c.s"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/cxy/hybridStorage/linux4.18.3/fs/f2fs/node.c -o CMakeFiles/f2fs.dir/node.c.s

CMakeFiles/f2fs.dir/node.c.o.requires:

.PHONY : CMakeFiles/f2fs.dir/node.c.o.requires

CMakeFiles/f2fs.dir/node.c.o.provides: CMakeFiles/f2fs.dir/node.c.o.requires
	$(MAKE) -f CMakeFiles/f2fs.dir/build.make CMakeFiles/f2fs.dir/node.c.o.provides.build
.PHONY : CMakeFiles/f2fs.dir/node.c.o.provides

CMakeFiles/f2fs.dir/node.c.o.provides.build: CMakeFiles/f2fs.dir/node.c.o


CMakeFiles/f2fs.dir/nvm.c.o: CMakeFiles/f2fs.dir/flags.make
CMakeFiles/f2fs.dir/nvm.c.o: ../nvm.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/cxy/hybridStorage/linux4.18.3/fs/f2fs/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_15) "Building C object CMakeFiles/f2fs.dir/nvm.c.o"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/f2fs.dir/nvm.c.o   -c /home/cxy/hybridStorage/linux4.18.3/fs/f2fs/nvm.c

CMakeFiles/f2fs.dir/nvm.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/f2fs.dir/nvm.c.i"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/cxy/hybridStorage/linux4.18.3/fs/f2fs/nvm.c > CMakeFiles/f2fs.dir/nvm.c.i

CMakeFiles/f2fs.dir/nvm.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/f2fs.dir/nvm.c.s"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/cxy/hybridStorage/linux4.18.3/fs/f2fs/nvm.c -o CMakeFiles/f2fs.dir/nvm.c.s

CMakeFiles/f2fs.dir/nvm.c.o.requires:

.PHONY : CMakeFiles/f2fs.dir/nvm.c.o.requires

CMakeFiles/f2fs.dir/nvm.c.o.provides: CMakeFiles/f2fs.dir/nvm.c.o.requires
	$(MAKE) -f CMakeFiles/f2fs.dir/build.make CMakeFiles/f2fs.dir/nvm.c.o.provides.build
.PHONY : CMakeFiles/f2fs.dir/nvm.c.o.provides

CMakeFiles/f2fs.dir/nvm.c.o.provides.build: CMakeFiles/f2fs.dir/nvm.c.o


CMakeFiles/f2fs.dir/recovery.c.o: CMakeFiles/f2fs.dir/flags.make
CMakeFiles/f2fs.dir/recovery.c.o: ../recovery.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/cxy/hybridStorage/linux4.18.3/fs/f2fs/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_16) "Building C object CMakeFiles/f2fs.dir/recovery.c.o"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/f2fs.dir/recovery.c.o   -c /home/cxy/hybridStorage/linux4.18.3/fs/f2fs/recovery.c

CMakeFiles/f2fs.dir/recovery.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/f2fs.dir/recovery.c.i"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/cxy/hybridStorage/linux4.18.3/fs/f2fs/recovery.c > CMakeFiles/f2fs.dir/recovery.c.i

CMakeFiles/f2fs.dir/recovery.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/f2fs.dir/recovery.c.s"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/cxy/hybridStorage/linux4.18.3/fs/f2fs/recovery.c -o CMakeFiles/f2fs.dir/recovery.c.s

CMakeFiles/f2fs.dir/recovery.c.o.requires:

.PHONY : CMakeFiles/f2fs.dir/recovery.c.o.requires

CMakeFiles/f2fs.dir/recovery.c.o.provides: CMakeFiles/f2fs.dir/recovery.c.o.requires
	$(MAKE) -f CMakeFiles/f2fs.dir/build.make CMakeFiles/f2fs.dir/recovery.c.o.provides.build
.PHONY : CMakeFiles/f2fs.dir/recovery.c.o.provides

CMakeFiles/f2fs.dir/recovery.c.o.provides.build: CMakeFiles/f2fs.dir/recovery.c.o


CMakeFiles/f2fs.dir/segment.c.o: CMakeFiles/f2fs.dir/flags.make
CMakeFiles/f2fs.dir/segment.c.o: ../segment.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/cxy/hybridStorage/linux4.18.3/fs/f2fs/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_17) "Building C object CMakeFiles/f2fs.dir/segment.c.o"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/f2fs.dir/segment.c.o   -c /home/cxy/hybridStorage/linux4.18.3/fs/f2fs/segment.c

CMakeFiles/f2fs.dir/segment.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/f2fs.dir/segment.c.i"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/cxy/hybridStorage/linux4.18.3/fs/f2fs/segment.c > CMakeFiles/f2fs.dir/segment.c.i

CMakeFiles/f2fs.dir/segment.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/f2fs.dir/segment.c.s"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/cxy/hybridStorage/linux4.18.3/fs/f2fs/segment.c -o CMakeFiles/f2fs.dir/segment.c.s

CMakeFiles/f2fs.dir/segment.c.o.requires:

.PHONY : CMakeFiles/f2fs.dir/segment.c.o.requires

CMakeFiles/f2fs.dir/segment.c.o.provides: CMakeFiles/f2fs.dir/segment.c.o.requires
	$(MAKE) -f CMakeFiles/f2fs.dir/build.make CMakeFiles/f2fs.dir/segment.c.o.provides.build
.PHONY : CMakeFiles/f2fs.dir/segment.c.o.provides

CMakeFiles/f2fs.dir/segment.c.o.provides.build: CMakeFiles/f2fs.dir/segment.c.o


CMakeFiles/f2fs.dir/shrinker.c.o: CMakeFiles/f2fs.dir/flags.make
CMakeFiles/f2fs.dir/shrinker.c.o: ../shrinker.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/cxy/hybridStorage/linux4.18.3/fs/f2fs/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_18) "Building C object CMakeFiles/f2fs.dir/shrinker.c.o"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/f2fs.dir/shrinker.c.o   -c /home/cxy/hybridStorage/linux4.18.3/fs/f2fs/shrinker.c

CMakeFiles/f2fs.dir/shrinker.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/f2fs.dir/shrinker.c.i"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/cxy/hybridStorage/linux4.18.3/fs/f2fs/shrinker.c > CMakeFiles/f2fs.dir/shrinker.c.i

CMakeFiles/f2fs.dir/shrinker.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/f2fs.dir/shrinker.c.s"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/cxy/hybridStorage/linux4.18.3/fs/f2fs/shrinker.c -o CMakeFiles/f2fs.dir/shrinker.c.s

CMakeFiles/f2fs.dir/shrinker.c.o.requires:

.PHONY : CMakeFiles/f2fs.dir/shrinker.c.o.requires

CMakeFiles/f2fs.dir/shrinker.c.o.provides: CMakeFiles/f2fs.dir/shrinker.c.o.requires
	$(MAKE) -f CMakeFiles/f2fs.dir/build.make CMakeFiles/f2fs.dir/shrinker.c.o.provides.build
.PHONY : CMakeFiles/f2fs.dir/shrinker.c.o.provides

CMakeFiles/f2fs.dir/shrinker.c.o.provides.build: CMakeFiles/f2fs.dir/shrinker.c.o


CMakeFiles/f2fs.dir/super.c.o: CMakeFiles/f2fs.dir/flags.make
CMakeFiles/f2fs.dir/super.c.o: ../super.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/cxy/hybridStorage/linux4.18.3/fs/f2fs/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_19) "Building C object CMakeFiles/f2fs.dir/super.c.o"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/f2fs.dir/super.c.o   -c /home/cxy/hybridStorage/linux4.18.3/fs/f2fs/super.c

CMakeFiles/f2fs.dir/super.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/f2fs.dir/super.c.i"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/cxy/hybridStorage/linux4.18.3/fs/f2fs/super.c > CMakeFiles/f2fs.dir/super.c.i

CMakeFiles/f2fs.dir/super.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/f2fs.dir/super.c.s"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/cxy/hybridStorage/linux4.18.3/fs/f2fs/super.c -o CMakeFiles/f2fs.dir/super.c.s

CMakeFiles/f2fs.dir/super.c.o.requires:

.PHONY : CMakeFiles/f2fs.dir/super.c.o.requires

CMakeFiles/f2fs.dir/super.c.o.provides: CMakeFiles/f2fs.dir/super.c.o.requires
	$(MAKE) -f CMakeFiles/f2fs.dir/build.make CMakeFiles/f2fs.dir/super.c.o.provides.build
.PHONY : CMakeFiles/f2fs.dir/super.c.o.provides

CMakeFiles/f2fs.dir/super.c.o.provides.build: CMakeFiles/f2fs.dir/super.c.o


CMakeFiles/f2fs.dir/sysfs.c.o: CMakeFiles/f2fs.dir/flags.make
CMakeFiles/f2fs.dir/sysfs.c.o: ../sysfs.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/cxy/hybridStorage/linux4.18.3/fs/f2fs/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_20) "Building C object CMakeFiles/f2fs.dir/sysfs.c.o"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/f2fs.dir/sysfs.c.o   -c /home/cxy/hybridStorage/linux4.18.3/fs/f2fs/sysfs.c

CMakeFiles/f2fs.dir/sysfs.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/f2fs.dir/sysfs.c.i"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/cxy/hybridStorage/linux4.18.3/fs/f2fs/sysfs.c > CMakeFiles/f2fs.dir/sysfs.c.i

CMakeFiles/f2fs.dir/sysfs.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/f2fs.dir/sysfs.c.s"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/cxy/hybridStorage/linux4.18.3/fs/f2fs/sysfs.c -o CMakeFiles/f2fs.dir/sysfs.c.s

CMakeFiles/f2fs.dir/sysfs.c.o.requires:

.PHONY : CMakeFiles/f2fs.dir/sysfs.c.o.requires

CMakeFiles/f2fs.dir/sysfs.c.o.provides: CMakeFiles/f2fs.dir/sysfs.c.o.requires
	$(MAKE) -f CMakeFiles/f2fs.dir/build.make CMakeFiles/f2fs.dir/sysfs.c.o.provides.build
.PHONY : CMakeFiles/f2fs.dir/sysfs.c.o.provides

CMakeFiles/f2fs.dir/sysfs.c.o.provides.build: CMakeFiles/f2fs.dir/sysfs.c.o


CMakeFiles/f2fs.dir/trace.c.o: CMakeFiles/f2fs.dir/flags.make
CMakeFiles/f2fs.dir/trace.c.o: ../trace.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/cxy/hybridStorage/linux4.18.3/fs/f2fs/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_21) "Building C object CMakeFiles/f2fs.dir/trace.c.o"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/f2fs.dir/trace.c.o   -c /home/cxy/hybridStorage/linux4.18.3/fs/f2fs/trace.c

CMakeFiles/f2fs.dir/trace.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/f2fs.dir/trace.c.i"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/cxy/hybridStorage/linux4.18.3/fs/f2fs/trace.c > CMakeFiles/f2fs.dir/trace.c.i

CMakeFiles/f2fs.dir/trace.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/f2fs.dir/trace.c.s"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/cxy/hybridStorage/linux4.18.3/fs/f2fs/trace.c -o CMakeFiles/f2fs.dir/trace.c.s

CMakeFiles/f2fs.dir/trace.c.o.requires:

.PHONY : CMakeFiles/f2fs.dir/trace.c.o.requires

CMakeFiles/f2fs.dir/trace.c.o.provides: CMakeFiles/f2fs.dir/trace.c.o.requires
	$(MAKE) -f CMakeFiles/f2fs.dir/build.make CMakeFiles/f2fs.dir/trace.c.o.provides.build
.PHONY : CMakeFiles/f2fs.dir/trace.c.o.provides

CMakeFiles/f2fs.dir/trace.c.o.provides.build: CMakeFiles/f2fs.dir/trace.c.o


CMakeFiles/f2fs.dir/xattr.c.o: CMakeFiles/f2fs.dir/flags.make
CMakeFiles/f2fs.dir/xattr.c.o: ../xattr.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/cxy/hybridStorage/linux4.18.3/fs/f2fs/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_22) "Building C object CMakeFiles/f2fs.dir/xattr.c.o"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/f2fs.dir/xattr.c.o   -c /home/cxy/hybridStorage/linux4.18.3/fs/f2fs/xattr.c

CMakeFiles/f2fs.dir/xattr.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/f2fs.dir/xattr.c.i"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/cxy/hybridStorage/linux4.18.3/fs/f2fs/xattr.c > CMakeFiles/f2fs.dir/xattr.c.i

CMakeFiles/f2fs.dir/xattr.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/f2fs.dir/xattr.c.s"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/cxy/hybridStorage/linux4.18.3/fs/f2fs/xattr.c -o CMakeFiles/f2fs.dir/xattr.c.s

CMakeFiles/f2fs.dir/xattr.c.o.requires:

.PHONY : CMakeFiles/f2fs.dir/xattr.c.o.requires

CMakeFiles/f2fs.dir/xattr.c.o.provides: CMakeFiles/f2fs.dir/xattr.c.o.requires
	$(MAKE) -f CMakeFiles/f2fs.dir/build.make CMakeFiles/f2fs.dir/xattr.c.o.provides.build
.PHONY : CMakeFiles/f2fs.dir/xattr.c.o.provides

CMakeFiles/f2fs.dir/xattr.c.o.provides.build: CMakeFiles/f2fs.dir/xattr.c.o


# Object files for target f2fs
f2fs_OBJECTS = \
"CMakeFiles/f2fs.dir/acl.c.o" \
"CMakeFiles/f2fs.dir/checkpoint.c.o" \
"CMakeFiles/f2fs.dir/data.c.o" \
"CMakeFiles/f2fs.dir/debug.c.o" \
"CMakeFiles/f2fs.dir/dir.c.o" \
"CMakeFiles/f2fs.dir/extent_cache.c.o" \
"CMakeFiles/f2fs.dir/file.c.o" \
"CMakeFiles/f2fs.dir/gc.c.o" \
"CMakeFiles/f2fs.dir/gc_nvm.c.o" \
"CMakeFiles/f2fs.dir/hash.c.o" \
"CMakeFiles/f2fs.dir/inline.c.o" \
"CMakeFiles/f2fs.dir/inode.c.o" \
"CMakeFiles/f2fs.dir/namei.c.o" \
"CMakeFiles/f2fs.dir/node.c.o" \
"CMakeFiles/f2fs.dir/nvm.c.o" \
"CMakeFiles/f2fs.dir/recovery.c.o" \
"CMakeFiles/f2fs.dir/segment.c.o" \
"CMakeFiles/f2fs.dir/shrinker.c.o" \
"CMakeFiles/f2fs.dir/super.c.o" \
"CMakeFiles/f2fs.dir/sysfs.c.o" \
"CMakeFiles/f2fs.dir/trace.c.o" \
"CMakeFiles/f2fs.dir/xattr.c.o"

# External object files for target f2fs
f2fs_EXTERNAL_OBJECTS =

f2fs: CMakeFiles/f2fs.dir/acl.c.o
f2fs: CMakeFiles/f2fs.dir/checkpoint.c.o
f2fs: CMakeFiles/f2fs.dir/data.c.o
f2fs: CMakeFiles/f2fs.dir/debug.c.o
f2fs: CMakeFiles/f2fs.dir/dir.c.o
f2fs: CMakeFiles/f2fs.dir/extent_cache.c.o
f2fs: CMakeFiles/f2fs.dir/file.c.o
f2fs: CMakeFiles/f2fs.dir/gc.c.o
f2fs: CMakeFiles/f2fs.dir/gc_nvm.c.o
f2fs: CMakeFiles/f2fs.dir/hash.c.o
f2fs: CMakeFiles/f2fs.dir/inline.c.o
f2fs: CMakeFiles/f2fs.dir/inode.c.o
f2fs: CMakeFiles/f2fs.dir/namei.c.o
f2fs: CMakeFiles/f2fs.dir/node.c.o
f2fs: CMakeFiles/f2fs.dir/nvm.c.o
f2fs: CMakeFiles/f2fs.dir/recovery.c.o
f2fs: CMakeFiles/f2fs.dir/segment.c.o
f2fs: CMakeFiles/f2fs.dir/shrinker.c.o
f2fs: CMakeFiles/f2fs.dir/super.c.o
f2fs: CMakeFiles/f2fs.dir/sysfs.c.o
f2fs: CMakeFiles/f2fs.dir/trace.c.o
f2fs: CMakeFiles/f2fs.dir/xattr.c.o
f2fs: CMakeFiles/f2fs.dir/build.make
f2fs: CMakeFiles/f2fs.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/cxy/hybridStorage/linux4.18.3/fs/f2fs/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_23) "Linking C executable f2fs"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/f2fs.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/f2fs.dir/build: f2fs

.PHONY : CMakeFiles/f2fs.dir/build

CMakeFiles/f2fs.dir/requires: CMakeFiles/f2fs.dir/acl.c.o.requires
CMakeFiles/f2fs.dir/requires: CMakeFiles/f2fs.dir/checkpoint.c.o.requires
CMakeFiles/f2fs.dir/requires: CMakeFiles/f2fs.dir/data.c.o.requires
CMakeFiles/f2fs.dir/requires: CMakeFiles/f2fs.dir/debug.c.o.requires
CMakeFiles/f2fs.dir/requires: CMakeFiles/f2fs.dir/dir.c.o.requires
CMakeFiles/f2fs.dir/requires: CMakeFiles/f2fs.dir/extent_cache.c.o.requires
CMakeFiles/f2fs.dir/requires: CMakeFiles/f2fs.dir/file.c.o.requires
CMakeFiles/f2fs.dir/requires: CMakeFiles/f2fs.dir/gc.c.o.requires
CMakeFiles/f2fs.dir/requires: CMakeFiles/f2fs.dir/gc_nvm.c.o.requires
CMakeFiles/f2fs.dir/requires: CMakeFiles/f2fs.dir/hash.c.o.requires
CMakeFiles/f2fs.dir/requires: CMakeFiles/f2fs.dir/inline.c.o.requires
CMakeFiles/f2fs.dir/requires: CMakeFiles/f2fs.dir/inode.c.o.requires
CMakeFiles/f2fs.dir/requires: CMakeFiles/f2fs.dir/namei.c.o.requires
CMakeFiles/f2fs.dir/requires: CMakeFiles/f2fs.dir/node.c.o.requires
CMakeFiles/f2fs.dir/requires: CMakeFiles/f2fs.dir/nvm.c.o.requires
CMakeFiles/f2fs.dir/requires: CMakeFiles/f2fs.dir/recovery.c.o.requires
CMakeFiles/f2fs.dir/requires: CMakeFiles/f2fs.dir/segment.c.o.requires
CMakeFiles/f2fs.dir/requires: CMakeFiles/f2fs.dir/shrinker.c.o.requires
CMakeFiles/f2fs.dir/requires: CMakeFiles/f2fs.dir/super.c.o.requires
CMakeFiles/f2fs.dir/requires: CMakeFiles/f2fs.dir/sysfs.c.o.requires
CMakeFiles/f2fs.dir/requires: CMakeFiles/f2fs.dir/trace.c.o.requires
CMakeFiles/f2fs.dir/requires: CMakeFiles/f2fs.dir/xattr.c.o.requires

.PHONY : CMakeFiles/f2fs.dir/requires

CMakeFiles/f2fs.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/f2fs.dir/cmake_clean.cmake
.PHONY : CMakeFiles/f2fs.dir/clean

CMakeFiles/f2fs.dir/depend:
	cd /home/cxy/hybridStorage/linux4.18.3/fs/f2fs/cmake-build-debug && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/cxy/hybridStorage/linux4.18.3/fs/f2fs /home/cxy/hybridStorage/linux4.18.3/fs/f2fs /home/cxy/hybridStorage/linux4.18.3/fs/f2fs/cmake-build-debug /home/cxy/hybridStorage/linux4.18.3/fs/f2fs/cmake-build-debug /home/cxy/hybridStorage/linux4.18.3/fs/f2fs/cmake-build-debug/CMakeFiles/f2fs.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/f2fs.dir/depend

