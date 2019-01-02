cmake_minimum_required(VERSION 3.10)

project(FHECourse VERSION 0.0.1 LANGUAGES CXX)
add_executable(fhecourse doron.cpp)

# Import HELib
find_library(helib_location NAMES libfhe.a)
message(STATUS ${helib_location})

add_library(fhe STATIC IMPORTED)
set_target_properties(fhe PROPERTIES IMPORTED_LOCATION ${helib_location})

# Import libntl
find_library(ntl_location NAMES libntl.so)
message(STATUS ${ntl_location})

add_library(ntl SHARED IMPORTED)
set_target_properties(ntl PROPERTIES IMPORTED_LOCATION ${ntl_location})

# Link with HELib
target_link_libraries(fhecourse fhe ntl)