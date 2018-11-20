cmake_minimum_required(VERSION 3.10)

project(FHECourse VERSION 0.0.1 LANGUAGES CXX)

# Executable will be in ../bin
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/../bin)

add_executable(fhecourse main.cpp)

# Import SEAL
find_package(SEAL 3.0.0 EXACT REQUIRED)

# Link SEAL
target_link_libraries(fhecourse SEAL::seal)
