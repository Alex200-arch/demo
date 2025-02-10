# CMake version check
cmake_minimum_required(VERSION 3.26)
include(ExternalProject)

# Create project
project(demo)

# Enable c++20 standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# # user defined variables
# set(EXTERNAL_PROJECT_PATH ${CMAKE_SOURCE_DIR}/external)

# # build gperftools with automake style
# set(GPERFTOOLS gperftools)
# ExternalProject_Add(
#     ${GPERFTOOLS}
#     GIT_REPOSITORY https://github.com/gperftools/gperftools.git
#     GIT_TAG gperftools-2.16
#     SOURCE_DIR ${EXTERNAL_PROJECT_PATH}/${GPERFTOOLS}_src
#     # BINARY_DIR ${EXTERNAL_PROJECT_PATH}/${GPERFTOOLS}_build
#     INSTALL_DIR ${EXTERNAL_PROJECT_PATH}/${GPERFTOOLS}_install
#     CONFIGURE_COMMAND sh <SOURCE_DIR>/autogen.sh && <SOURCE_DIR>/configure --prefix=<INSTALL_DIR>
#     BUILD_IN_SOURCE TRUE
# )

# # build poco with cmake style
# set(POCO poco)
# ExternalProject_Add(
#     ${POCO}
#     GIT_REPOSITORY https://github.com/pocoproject/poco.git
#     GIT_TAG poco-1.14.0-release
#     SOURCE_DIR ${EXTERNAL_PROJECT_PATH}/${POCO}_src
#     BINARY_DIR ${EXTERNAL_PROJECT_PATH}/${POCO}_build
#     INSTALL_DIR ${EXTERNAL_PROJECT_PATH}/${POCO}_install
#     CMAKE_ARGS
#         -DCMAKE_INSTALL_PREFIX=<INSTALL_DIR>
# )

# # build xerces-c with cmake style
# set(XERCESC xerces-c)
# ExternalProject_Add(
#     ${XERCESC}
#     GIT_REPOSITORY https://github.com/apache/xerces-c.git
#     GIT_TAG v3.3.0
#     SOURCE_DIR ${EXTERNAL_PROJECT_PATH}/${XERCESC}_src
#     BINARY_DIR ${EXTERNAL_PROJECT_PATH}/${XERCESC}_build
#     INSTALL_DIR ${EXTERNAL_PROJECT_PATH}/${XERCESC}_install
#     CMAKE_ARGS
#         -DCMAKE_INSTALL_PREFIX=<INSTALL_DIR>
# )

# # build protobuf with cmake style
# set(PROTOBUF protobuf)
# ExternalProject_Add(
#     ${PROTOBUF}
#     GIT_REPOSITORY https://github.com/protocolbuffers/protobuf.git
#     GIT_TAG v29.3
#     SOURCE_DIR ${EXTERNAL_PROJECT_PATH}/${PROTOBUF}_src
#     BINARY_DIR ${EXTERNAL_PROJECT_PATH}/${PROTOBUF}_build
#     INSTALL_DIR ${EXTERNAL_PROJECT_PATH}/${PROTOBUF}_install
#     CMAKE_ARGS
#         -DCMAKE_INSTALL_PREFIX=<INSTALL_DIR>
# )


# Add main.cpp file of the project root directory as a source file
set(FILES "")

include_directories(/usr/include/boost1.78)
include_directories(${CMAKE_SOURCE_DIR}/external/cnl/include)
include_directories(${CMAKE_SOURCE_DIR}/external/libfixmath)
link_directories(${CMAKE_SOURCE_DIR}/external/libfixmath/build)

# Add executable target with source files listed in SOURCE_FILES variable
add_executable(demo main.cpp)
add_executable(demo_test
                main_test.cpp
                test_decimal_boost.cpp
                test_decimal_cnl.cpp
                test_decimal_customer.cpp
                test_decimal_fix16.cpp)
add_executable(demo_benchmark
                main_benchmark.cpp
                bm_decimal_boost.cpp
                bm_decimal_cnl.cpp
                bm_decimal_customer.cpp
                bm_decimal_fix16.cpp)

target_link_libraries(demo_test gtest libfixmath)
target_link_libraries(demo_benchmark benchmark libfixmath)
