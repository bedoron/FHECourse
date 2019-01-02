cmake_minimum_required(VERSION 3.0.0)
project(FHECourse VERSION 0.1.0 LANGUAGES CXX)


include(GNUInstallDirs)
include(CTest)
enable_testing()
set(CMAKE_BUILD_TYPE Debug)

add_subdirectory(HELib)
add_subdirectory(src)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)