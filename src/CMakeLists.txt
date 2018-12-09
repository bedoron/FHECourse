cmake_minimum_required(VERSION 3.10)

project(FHECourse VERSION 0.0.1 LANGUAGES CXX)

# Executable will be in ../bin
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_HOME_DIRECTORY}/bin)

add_executable(fhecourse doron.cpp)

# Import SEAL
find_package(fhe REQUIRED)

# Link SEAL
target_link_libraries(fhecourse fhe)

