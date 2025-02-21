# CMake version check
cmake_minimum_required(VERSION 3.26)
include(ExternalProject)

# Create project
project(demo)

# Enable c++20 standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_subdirectory(common-lib)

add_executable(demo main.cpp)

# add_executable(IPC1 IPCtest1.cpp)
# add_executable(IPC11 IPCtest11.cpp)
# add_executable(IPC2 IPCtest2.cpp)
