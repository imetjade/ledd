# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.10

# Default target executed when no arguments are given to make.
default_target: all

.PHONY : default_target

# Allow only one "make -f Makefile2" at a time, but pass parallelism.
.NOTPARALLEL:


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
CMAKE_SOURCE_DIR = /home-mc/imetjade.kim/workspace/ledd

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home-mc/imetjade.kim/workspace/ledd

#=============================================================================
# Targets provided globally by CMake.

# Special rule for the target install/strip
install/strip: preinstall
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --cyan "Installing the project stripped..."
	/usr/bin/cmake -DCMAKE_INSTALL_DO_STRIP=1 -P cmake_install.cmake
.PHONY : install/strip

# Special rule for the target install/strip
install/strip/fast: preinstall/fast
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --cyan "Installing the project stripped..."
	/usr/bin/cmake -DCMAKE_INSTALL_DO_STRIP=1 -P cmake_install.cmake
.PHONY : install/strip/fast

# Special rule for the target edit_cache
edit_cache:
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --cyan "No interactive CMake dialog available..."
	/usr/bin/cmake -E echo No\ interactive\ CMake\ dialog\ available.
.PHONY : edit_cache

# Special rule for the target edit_cache
edit_cache/fast: edit_cache

.PHONY : edit_cache/fast

# Special rule for the target list_install_components
list_install_components:
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --cyan "Available install components are: \"Unspecified\""
.PHONY : list_install_components

# Special rule for the target list_install_components
list_install_components/fast: list_install_components

.PHONY : list_install_components/fast

# Special rule for the target install/local
install/local: preinstall
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --cyan "Installing only the local directory..."
	/usr/bin/cmake -DCMAKE_INSTALL_LOCAL_ONLY=1 -P cmake_install.cmake
.PHONY : install/local

# Special rule for the target install/local
install/local/fast: preinstall/fast
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --cyan "Installing only the local directory..."
	/usr/bin/cmake -DCMAKE_INSTALL_LOCAL_ONLY=1 -P cmake_install.cmake
.PHONY : install/local/fast

# Special rule for the target rebuild_cache
rebuild_cache:
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --cyan "Running CMake to regenerate build system..."
	/usr/bin/cmake -H$(CMAKE_SOURCE_DIR) -B$(CMAKE_BINARY_DIR)
.PHONY : rebuild_cache

# Special rule for the target rebuild_cache
rebuild_cache/fast: rebuild_cache

.PHONY : rebuild_cache/fast

# Special rule for the target install
install: preinstall
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --cyan "Install the project..."
	/usr/bin/cmake -P cmake_install.cmake
.PHONY : install

# Special rule for the target install
install/fast: preinstall/fast
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --cyan "Install the project..."
	/usr/bin/cmake -P cmake_install.cmake
.PHONY : install/fast

# The main all target
all: cmake_check_build_system
	$(CMAKE_COMMAND) -E cmake_progress_start /home-mc/imetjade.kim/workspace/ledd/CMakeFiles /home-mc/imetjade.kim/workspace/ledd/CMakeFiles/progress.marks
	$(MAKE) -f CMakeFiles/Makefile2 all
	$(CMAKE_COMMAND) -E cmake_progress_start /home-mc/imetjade.kim/workspace/ledd/CMakeFiles 0
.PHONY : all

# The main clean target
clean:
	$(MAKE) -f CMakeFiles/Makefile2 clean
.PHONY : clean

# The main clean target
clean/fast: clean

.PHONY : clean/fast

# Prepare targets for installation.
preinstall: all
	$(MAKE) -f CMakeFiles/Makefile2 preinstall
.PHONY : preinstall

# Prepare targets for installation.
preinstall/fast:
	$(MAKE) -f CMakeFiles/Makefile2 preinstall
.PHONY : preinstall/fast

# clear depends
depend:
	$(CMAKE_COMMAND) -H$(CMAKE_SOURCE_DIR) -B$(CMAKE_BINARY_DIR) --check-build-system CMakeFiles/Makefile.cmake 1
.PHONY : depend

#=============================================================================
# Target rules for targets named ledd

# Build rule for target.
ledd: cmake_check_build_system
	$(MAKE) -f CMakeFiles/Makefile2 ledd
.PHONY : ledd

# fast build rule for target.
ledd/fast:
	$(MAKE) -f CMakeFiles/ledd.dir/build.make CMakeFiles/ledd.dir/build
.PHONY : ledd/fast

# target to build an object file
src/ledApp.o:
	$(MAKE) -f CMakeFiles/ledd.dir/build.make CMakeFiles/ledd.dir/src/ledApp.o
.PHONY : src/ledApp.o

# target to preprocess a source file
src/ledApp.i:
	$(MAKE) -f CMakeFiles/ledd.dir/build.make CMakeFiles/ledd.dir/src/ledApp.i
.PHONY : src/ledApp.i

# target to generate assembly for a file
src/ledApp.s:
	$(MAKE) -f CMakeFiles/ledd.dir/build.make CMakeFiles/ledd.dir/src/ledApp.s
.PHONY : src/ledApp.s

# target to build an object file
src/ledFileIPC.o:
	$(MAKE) -f CMakeFiles/ledd.dir/build.make CMakeFiles/ledd.dir/src/ledFileIPC.o
.PHONY : src/ledFileIPC.o

# target to preprocess a source file
src/ledFileIPC.i:
	$(MAKE) -f CMakeFiles/ledd.dir/build.make CMakeFiles/ledd.dir/src/ledFileIPC.i
.PHONY : src/ledFileIPC.i

# target to generate assembly for a file
src/ledFileIPC.s:
	$(MAKE) -f CMakeFiles/ledd.dir/build.make CMakeFiles/ledd.dir/src/ledFileIPC.s
.PHONY : src/ledFileIPC.s

# target to build an object file
src/ledPlatform.o:
	$(MAKE) -f CMakeFiles/ledd.dir/build.make CMakeFiles/ledd.dir/src/ledPlatform.o
.PHONY : src/ledPlatform.o

# target to preprocess a source file
src/ledPlatform.i:
	$(MAKE) -f CMakeFiles/ledd.dir/build.make CMakeFiles/ledd.dir/src/ledPlatform.i
.PHONY : src/ledPlatform.i

# target to generate assembly for a file
src/ledPlatform.s:
	$(MAKE) -f CMakeFiles/ledd.dir/build.make CMakeFiles/ledd.dir/src/ledPlatform.s
.PHONY : src/ledPlatform.s

# target to build an object file
src/ledPoll.o:
	$(MAKE) -f CMakeFiles/ledd.dir/build.make CMakeFiles/ledd.dir/src/ledPoll.o
.PHONY : src/ledPoll.o

# target to preprocess a source file
src/ledPoll.i:
	$(MAKE) -f CMakeFiles/ledd.dir/build.make CMakeFiles/ledd.dir/src/ledPoll.i
.PHONY : src/ledPoll.i

# target to generate assembly for a file
src/ledPoll.s:
	$(MAKE) -f CMakeFiles/ledd.dir/build.make CMakeFiles/ledd.dir/src/ledPoll.s
.PHONY : src/ledPoll.s

# target to build an object file
src/linkv2/ledLinkv2Cmd.o:
	$(MAKE) -f CMakeFiles/ledd.dir/build.make CMakeFiles/ledd.dir/src/linkv2/ledLinkv2Cmd.o
.PHONY : src/linkv2/ledLinkv2Cmd.o

# target to preprocess a source file
src/linkv2/ledLinkv2Cmd.i:
	$(MAKE) -f CMakeFiles/ledd.dir/build.make CMakeFiles/ledd.dir/src/linkv2/ledLinkv2Cmd.i
.PHONY : src/linkv2/ledLinkv2Cmd.i

# target to generate assembly for a file
src/linkv2/ledLinkv2Cmd.s:
	$(MAKE) -f CMakeFiles/ledd.dir/build.make CMakeFiles/ledd.dir/src/linkv2/ledLinkv2Cmd.s
.PHONY : src/linkv2/ledLinkv2Cmd.s

# target to build an object file
src/linkv2/ledNcpSocket.o:
	$(MAKE) -f CMakeFiles/ledd.dir/build.make CMakeFiles/ledd.dir/src/linkv2/ledNcpSocket.o
.PHONY : src/linkv2/ledNcpSocket.o

# target to preprocess a source file
src/linkv2/ledNcpSocket.i:
	$(MAKE) -f CMakeFiles/ledd.dir/build.make CMakeFiles/ledd.dir/src/linkv2/ledNcpSocket.i
.PHONY : src/linkv2/ledNcpSocket.i

# target to generate assembly for a file
src/linkv2/ledNcpSocket.s:
	$(MAKE) -f CMakeFiles/ledd.dir/build.make CMakeFiles/ledd.dir/src/linkv2/ledNcpSocket.s
.PHONY : src/linkv2/ledNcpSocket.s

# target to build an object file
src/linkv2/ledPlatformLinkV2.o:
	$(MAKE) -f CMakeFiles/ledd.dir/build.make CMakeFiles/ledd.dir/src/linkv2/ledPlatformLinkV2.o
.PHONY : src/linkv2/ledPlatformLinkV2.o

# target to preprocess a source file
src/linkv2/ledPlatformLinkV2.i:
	$(MAKE) -f CMakeFiles/ledd.dir/build.make CMakeFiles/ledd.dir/src/linkv2/ledPlatformLinkV2.i
.PHONY : src/linkv2/ledPlatformLinkV2.i

# target to generate assembly for a file
src/linkv2/ledPlatformLinkV2.s:
	$(MAKE) -f CMakeFiles/ledd.dir/build.make CMakeFiles/ledd.dir/src/linkv2/ledPlatformLinkV2.s
.PHONY : src/linkv2/ledPlatformLinkV2.s

# Help Target
help:
	@echo "The following are some of the valid targets for this Makefile:"
	@echo "... all (the default if no target is provided)"
	@echo "... clean"
	@echo "... depend"
	@echo "... install/strip"
	@echo "... edit_cache"
	@echo "... ledd"
	@echo "... list_install_components"
	@echo "... install/local"
	@echo "... rebuild_cache"
	@echo "... install"
	@echo "... src/ledApp.o"
	@echo "... src/ledApp.i"
	@echo "... src/ledApp.s"
	@echo "... src/ledFileIPC.o"
	@echo "... src/ledFileIPC.i"
	@echo "... src/ledFileIPC.s"
	@echo "... src/ledPlatform.o"
	@echo "... src/ledPlatform.i"
	@echo "... src/ledPlatform.s"
	@echo "... src/ledPoll.o"
	@echo "... src/ledPoll.i"
	@echo "... src/ledPoll.s"
	@echo "... src/linkv2/ledLinkv2Cmd.o"
	@echo "... src/linkv2/ledLinkv2Cmd.i"
	@echo "... src/linkv2/ledLinkv2Cmd.s"
	@echo "... src/linkv2/ledNcpSocket.o"
	@echo "... src/linkv2/ledNcpSocket.i"
	@echo "... src/linkv2/ledNcpSocket.s"
	@echo "... src/linkv2/ledPlatformLinkV2.o"
	@echo "... src/linkv2/ledPlatformLinkV2.i"
	@echo "... src/linkv2/ledPlatformLinkV2.s"
.PHONY : help



#=============================================================================
# Special targets to cleanup operation of make.

# Special rule to run CMake to check the build system integrity.
# No rule that depends on this can have commands that come from listfiles
# because they might be regenerated.
cmake_check_build_system:
	$(CMAKE_COMMAND) -H$(CMAKE_SOURCE_DIR) -B$(CMAKE_BINARY_DIR) --check-build-system CMakeFiles/Makefile.cmake 0
.PHONY : cmake_check_build_system

