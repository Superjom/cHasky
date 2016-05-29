project(chasky)
cmake_minimum_required(VERSION 3.4.0)
include_directories(./)
include_directories(thirdparty/local/include)

set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -Wall")

# common source files
set (SOURCE_FILES
  ${CMAKE_SOURCE_DIR}/chasky/common/strings.cc
  ${CMAKE_SOURCE_DIR}/chasky/common/stat.cc
  ${CMAKE_SOURCE_DIR}/chasky/common/strings_test.cc
  ${CMAKE_SOURCE_DIR}/chasky/common/stat_test.cc
  ${CMAKE_SOURCE_DIR}/chasky/test/test.cc
  )

set (LOCAL_LIB ${CMAKE_SOURCE_DIR}/thirdparty/local/lib)

# test case
add_executable (chaskytest ${SOURCE_FILES})
target_link_libraries (chaskytest
  "${LOCAL_LIB}/libgtest.a"
  "${LOCAL_LIB}/libgtest_main.a"
  "${LOCAL_LIB}/libglog.a"
  )
