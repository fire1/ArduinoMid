# CMAKE generated file: DO NOT EDIT!
# Generated by "MinGW Makefiles" Generator, CMake Version 3.6

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
CMAKE_COMMAND = "C:\Program Files (x86)\JetBrains\CLion 2016.3\bin\cmake\bin\cmake.exe"

# The command to remove a file.
RM = "C:\Program Files (x86)\JetBrains\CLion 2016.3\bin\cmake\bin\cmake.exe" -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = C:\Users\Admin\Documents\Arduino\ArduinoMid

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = C:\Users\Admin\Documents\Arduino\ArduinoMid\cmake-build-debug

# Include any dependencies generated for this target.
include CMakeFiles/ArduinoMid.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/ArduinoMid.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/ArduinoMid.dir/flags.make

ArduinoMid_MID.ino.cpp: ../MID/MID.ino
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --blue --bold --progress-dir=C:\Users\Admin\Documents\Arduino\ArduinoMid\cmake-build-debug\CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Regnerating MID.ino Sketch"
	"C:\Program Files (x86)\JetBrains\CLion 2016.3\bin\cmake\bin\cmake.exe" C:/Users/Admin/Documents/Arduino/ArduinoMid

CMakeFiles/ArduinoMid.dir/ArduinoMid_MID.ino.cpp.obj: CMakeFiles/ArduinoMid.dir/flags.make
CMakeFiles/ArduinoMid.dir/ArduinoMid_MID.ino.cpp.obj: ArduinoMid_MID.ino.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=C:\Users\Admin\Documents\Arduino\ArduinoMid\cmake-build-debug\CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object CMakeFiles/ArduinoMid.dir/ArduinoMid_MID.ino.cpp.obj"
	C:\PROGRA~2\Arduino\hardware\tools\avr\bin\AVR-G_~1.EXE   $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles\ArduinoMid.dir\ArduinoMid_MID.ino.cpp.obj -c C:\Users\Admin\Documents\Arduino\ArduinoMid\cmake-build-debug\ArduinoMid_MID.ino.cpp

CMakeFiles/ArduinoMid.dir/ArduinoMid_MID.ino.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/ArduinoMid.dir/ArduinoMid_MID.ino.cpp.i"
	C:\PROGRA~2\Arduino\hardware\tools\avr\bin\AVR-G_~1.EXE  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E C:\Users\Admin\Documents\Arduino\ArduinoMid\cmake-build-debug\ArduinoMid_MID.ino.cpp > CMakeFiles\ArduinoMid.dir\ArduinoMid_MID.ino.cpp.i

CMakeFiles/ArduinoMid.dir/ArduinoMid_MID.ino.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/ArduinoMid.dir/ArduinoMid_MID.ino.cpp.s"
	C:\PROGRA~2\Arduino\hardware\tools\avr\bin\AVR-G_~1.EXE  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S C:\Users\Admin\Documents\Arduino\ArduinoMid\cmake-build-debug\ArduinoMid_MID.ino.cpp -o CMakeFiles\ArduinoMid.dir\ArduinoMid_MID.ino.cpp.s

CMakeFiles/ArduinoMid.dir/ArduinoMid_MID.ino.cpp.obj.requires:

.PHONY : CMakeFiles/ArduinoMid.dir/ArduinoMid_MID.ino.cpp.obj.requires

CMakeFiles/ArduinoMid.dir/ArduinoMid_MID.ino.cpp.obj.provides: CMakeFiles/ArduinoMid.dir/ArduinoMid_MID.ino.cpp.obj.requires
	$(MAKE) -f CMakeFiles\ArduinoMid.dir\build.make CMakeFiles/ArduinoMid.dir/ArduinoMid_MID.ino.cpp.obj.provides.build
.PHONY : CMakeFiles/ArduinoMid.dir/ArduinoMid_MID.ino.cpp.obj.provides

CMakeFiles/ArduinoMid.dir/ArduinoMid_MID.ino.cpp.obj.provides.build: CMakeFiles/ArduinoMid.dir/ArduinoMid_MID.ino.cpp.obj


# Object files for target ArduinoMid
ArduinoMid_OBJECTS = \
"CMakeFiles/ArduinoMid.dir/ArduinoMid_MID.ino.cpp.obj"

# External object files for target ArduinoMid
ArduinoMid_EXTERNAL_OBJECTS =

ArduinoMid.elf: CMakeFiles/ArduinoMid.dir/ArduinoMid_MID.ino.cpp.obj
ArduinoMid.elf: CMakeFiles/ArduinoMid.dir/build.make
ArduinoMid.elf: libmega_SPI.a
ArduinoMid.elf: libmega_Wire.a
ArduinoMid.elf: libmega_twi.a
ArduinoMid.elf: libmega_OneWire.a
ArduinoMid.elf: libmega_Firmata.a
ArduinoMid.elf: libmega_MenuBackend.a
ArduinoMid.elf: libmega_SoftwareSerial.a
ArduinoMid.elf: libmega_OneWire.a
ArduinoMid.elf: libmega_DallasTemperature.a
ArduinoMid.elf: libmega_LiquidCrystal.a
ArduinoMid.elf: libmega_CORE.a
ArduinoMid.elf: CMakeFiles/ArduinoMid.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=C:\Users\Admin\Documents\Arduino\ArduinoMid\cmake-build-debug\CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Linking CXX executable ArduinoMid.elf"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles\ArduinoMid.dir\link.txt --verbose=$(VERBOSE)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --blue --bold "Generating EEP image"
	"C:\Program Files (x86)\Arduino\hardware\tools\avr\bin\avr-objcopy.exe" -O ihex -j .eeprom --set-section-flags=.eeprom=alloc,load --no-change-warnings --change-section-lma .eeprom=0 C:/Users/Admin/Documents/Arduino/ArduinoMid/cmake-build-debug/ArduinoMid.elf C:/Users/Admin/Documents/Arduino/ArduinoMid/cmake-build-debug/ArduinoMid.eep
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --blue --bold "Generating HEX image"
	"C:\Program Files (x86)\Arduino\hardware\tools\avr\bin\avr-objcopy.exe" -O ihex -R .eeprom C:/Users/Admin/Documents/Arduino/ArduinoMid/cmake-build-debug/ArduinoMid.elf C:/Users/Admin/Documents/Arduino/ArduinoMid/cmake-build-debug/ArduinoMid.hex
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --blue --bold "Calculating image size"
	"C:\Program Files (x86)\JetBrains\CLion 2016.3\bin\cmake\bin\cmake.exe" -DFIRMWARE_IMAGE=C:/Users/Admin/Documents/Arduino/ArduinoMid/cmake-build-debug/ArduinoMid.elf -DMCU= -DEEPROM_IMAGE=C:/Users/Admin/Documents/Arduino/ArduinoMid/cmake-build-debug/ArduinoMid.eep -P C:/Users/Admin/Documents/Arduino/ArduinoMid/cmake-build-debug/CMakeFiles/FirmwareSize.cmake

# Rule to build all files generated by this target.
CMakeFiles/ArduinoMid.dir/build: ArduinoMid.elf

.PHONY : CMakeFiles/ArduinoMid.dir/build

CMakeFiles/ArduinoMid.dir/requires: CMakeFiles/ArduinoMid.dir/ArduinoMid_MID.ino.cpp.obj.requires

.PHONY : CMakeFiles/ArduinoMid.dir/requires

CMakeFiles/ArduinoMid.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles\ArduinoMid.dir\cmake_clean.cmake
.PHONY : CMakeFiles/ArduinoMid.dir/clean

CMakeFiles/ArduinoMid.dir/depend: ArduinoMid_MID.ino.cpp
	$(CMAKE_COMMAND) -E cmake_depends "MinGW Makefiles" C:\Users\Admin\Documents\Arduino\ArduinoMid C:\Users\Admin\Documents\Arduino\ArduinoMid C:\Users\Admin\Documents\Arduino\ArduinoMid\cmake-build-debug C:\Users\Admin\Documents\Arduino\ArduinoMid\cmake-build-debug C:\Users\Admin\Documents\Arduino\ArduinoMid\cmake-build-debug\CMakeFiles\ArduinoMid.dir\DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/ArduinoMid.dir/depend

