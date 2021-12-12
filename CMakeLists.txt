cmake_minimum_required(VERSION 3.22)
project(CarpFishingShip)

set(CMAKE_C_STANDARD 99)

set(CXXMAKE_C_STANDARD 11)
set(POSITION_INDEPENDENT_CODE ON)

enable_testing()

add_subdirectory(tests)
add_subdirectory(googletest)

#target_link_libraries(CarpFishingShip CarpFishingShip-lib)
