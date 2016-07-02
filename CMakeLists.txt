project(chasky)
cmake_minimum_required(VERSION 3.4.0)
include_directories(./ ./build/protos)
include_directories(thirdparty/local/include)
include_directories(build/protos)

SET(GCC_COVERAGE_LINK_FLAGS    "-lprotobuf")
set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -Wall -lprotobuf")
SET( CMAKE_EXE_LINKER_FLAGS  "${CMAKE_EXE_LINKER_FLAGS} ${GCC_COVERAGE_LINK_FLAGS}" )

# common source files
set (SOURCE_FILES
  ${CMAKE_SOURCE_DIR}/chasky/common/strings.cc
  ${CMAKE_SOURCE_DIR}/chasky/common/stat.cc
  ${CMAKE_SOURCE_DIR}/chasky/core/common/eigen_vector.cc
  ${CMAKE_SOURCE_DIR}/chasky/core/common/status.cc
  ${CMAKE_SOURCE_DIR}/chasky/common/string_piece.cc
  ${CMAKE_SOURCE_DIR}/chasky/core/framework/argument.cc
  ${CMAKE_SOURCE_DIR}/chasky/core/framework/function_def_builder.cc
  #${CMAKE_SOURCE_DIR}/chasky/core/framework/exec_context.cc
  ${CMAKE_SOURCE_DIR}/chasky/core/framework/function.cc
  ${CMAKE_SOURCE_DIR}/chasky/core/framework/attr_value_util.cc
  ${CMAKE_SOURCE_DIR}/chasky/core/framework/node.cc
  #${CMAKE_SOURCE_DIR}/chasky/core/runtime/session.cc
  )

set (TEST_SOURCE_FILES
  ${CMAKE_SOURCE_DIR}/chasky/common/strings_test.cc
  ${CMAKE_SOURCE_DIR}/chasky/common/stat_test.cc
  ${CMAKE_SOURCE_DIR}/chasky/test/test.cc
  ${CMAKE_SOURCE_DIR}/chasky/core/common/eigen_vector_test.cc
  ${CMAKE_SOURCE_DIR}/chasky/core/common/status_test.cc
  ${CMAKE_SOURCE_DIR}/chasky/common/string_piece_test.cc
  ${CMAKE_SOURCE_DIR}/chasky/core/framework/attr_value_util_test.cc
  ${CMAKE_SOURCE_DIR}/chasky/core/framework/function_def_builder_test.cc
  ${CMAKE_SOURCE_DIR}/chasky/core/framework/argument_test.cc
  ${CMAKE_SOURCE_DIR}/chasky/core/framework/node_test.cc
  )

set (PROTOS_FILES
  ${CMAKE_SOURCE_DIR}/build/protos/chasky/core/framework/argument.pb.cc
  ${CMAKE_SOURCE_DIR}/build/protos/chasky/core/framework/function.pb.cc
  ${CMAKE_SOURCE_DIR}/build/protos/chasky/core/framework/attr_value.pb.cc
  ${CMAKE_SOURCE_DIR}/build/protos/chasky/core/framework/types.pb.cc
  ${CMAKE_SOURCE_DIR}/build/protos/chasky/core/framework/graph.pb.cc
)

set (LOCAL_LIB ${CMAKE_SOURCE_DIR}/thirdparty/local/lib)

# test case
add_executable (chaskytest
  ${SOURCE_FILES}
  ${PROTOS_FILES}
  ${TEST_SOURCE_FILES}
  )

target_link_libraries (chaskytest
  "${LOCAL_LIB}/libgtest.a"
  "${LOCAL_LIB}/libgtest_main.a"
  "${LOCAL_LIB}/libglog.a"
  "${LOCAL_LIB}/libprotobuf.a"
  )

