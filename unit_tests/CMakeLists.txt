cmake_minimum_required(VERSION 3.16)
project(ArduinoMidUnitTests LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(FetchContent)
FetchContent_Declare(
  Catch2
  GIT_REPOSITORY https://github.com/catchorg/Catch2.git
  GIT_TAG v3.5.4
)
FetchContent_MakeAvailable(Catch2)

add_executable(arduino_mid_unit_tests
  test_mid_math.cpp
  test_amp_time.cpp
  test_gear.cpp
  test_lpg_bits.cpp
)

target_include_directories(arduino_mid_unit_tests PRIVATE
  ${CMAKE_CURRENT_SOURCE_DIR}/stubs
  ${CMAKE_CURRENT_SOURCE_DIR}/../MID
  ${CMAKE_CURRENT_SOURCE_DIR}/../MID/lib
)

target_compile_definitions(arduino_mid_unit_tests PRIVATE
  CAR_WEIGHT=1155
  MAX_GEAR_NBR=5
)

target_link_libraries(arduino_mid_unit_tests PRIVATE Catch2::Catch2WithMain)

include(CTest)
include(Catch)
catch_discover_tests(arduino_mid_unit_tests)
