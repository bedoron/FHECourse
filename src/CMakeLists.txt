cmake_minimum_required(VERSION 3.10)

project(FHECourse VERSION 0.0.1 LANGUAGES CXX)

# Executable will be in ../bin
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_HOME_DIRECTORY}/bin)

add_executable(fhecourse doron.cpp)

# Import HELib
#target_link_libraries(doron.cpp ${CMAKE_SOURCE_DIR}/HELib/lib/libfhe.a)

# ADD_LIBRARY(fhe STATIC IMPORTED)
# SET_TARGET_PROPERTIES(fhe PROPERTIES IMPORTED_LOCATION ${CMAKE_SOURCE_DIR}/HELib/lib/libfhe.a)
#TARGET_LINK_LIBRARIES(fhecourse fhe)


