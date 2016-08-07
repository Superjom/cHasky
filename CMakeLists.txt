project(chasky)
cmake_minimum_required(VERSION 3.4.0)
include_directories(./ ./build/protos)
include_directories(thirdparty/local/include)
include_directories(build/protos)
include_directories(
  /System/Library/Frameworks/Python.framework/Versions/2.7/include/python2.7)

SET(GCC_COVERAGE_LINK_FLAGS    "-lprotobuf")
set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -Wall -framework Python")
SET( CMAKE_EXE_LINKER_FLAGS  "${CMAKE_EXE_LINKER_FLAGS} ${GCC_COVERAGE_LINK_FLAGS}" )

# common source files
set (SOURCE_FILES
  ${CMAKE_SOURCE_DIR}/chasky/common/strings.cc
  ${CMAKE_SOURCE_DIR}/chasky/common/stat.cc
  ${CMAKE_SOURCE_DIR}/chasky/core/common/eigen_vector.cc
  # ${CMAKE_SOURCE_DIR}/chasky/core/common/eigen_matrix.cc
  ${CMAKE_SOURCE_DIR}/chasky/core/common/status.cc
  ${CMAKE_SOURCE_DIR}/chasky/common/string_piece.cc
  ${CMAKE_SOURCE_DIR}/chasky/core/framework/argument.cc
  ${CMAKE_SOURCE_DIR}/chasky/core/framework/argument_field.cc
  ${CMAKE_SOURCE_DIR}/chasky/core/framework/function_def_builder.cc
  #${CMAKE_SOURCE_DIR}/chasky/core/framework/exec_context.cc
  ${CMAKE_SOURCE_DIR}/chasky/core/framework/function.cc
  ${CMAKE_SOURCE_DIR}/chasky/core/framework/attr_value_util.cc
  ${CMAKE_SOURCE_DIR}/chasky/core/framework/node.cc
  ${CMAKE_SOURCE_DIR}/chasky/core/framework/edge.cc
  ${CMAKE_SOURCE_DIR}/chasky/core/framework/edge_utils.cc
  ${CMAKE_SOURCE_DIR}/chasky/core/framework/graph.cc
  ${CMAKE_SOURCE_DIR}/chasky/core/runtime/session.cc
  ${CMAKE_SOURCE_DIR}/chasky/core/runtime/graph_builder.cc
  ${CMAKE_SOURCE_DIR}/chasky/core/functions/AddN.cc
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
  ${CMAKE_SOURCE_DIR}/chasky/core/framework/function_test.cc
  ${CMAKE_SOURCE_DIR}/chasky/core/framework/node_test.cc
  ${CMAKE_SOURCE_DIR}/chasky/core/framework/graph_test.cc
  ${CMAKE_SOURCE_DIR}/chasky/core/common/eigen_matrix_test.cc
  ${CMAKE_SOURCE_DIR}/chasky/core/functions/AddN_test.cc
)

set (PROTOS_FILES
  ${CMAKE_SOURCE_DIR}/build/protos/chasky/core/framework/argument.pb.cc
  ${CMAKE_SOURCE_DIR}/build/protos/chasky/core/framework/function.pb.cc
  ${CMAKE_SOURCE_DIR}/build/protos/chasky/core/framework/attr_value.pb.cc
  ${CMAKE_SOURCE_DIR}/build/protos/chasky/core/framework/types.pb.cc
  ${CMAKE_SOURCE_DIR}/build/protos/chasky/core/framework/graph.pb.cc
)

set (SWIG_FILES
  ${CMAKE_SOURCE_DIR}/chasky/api/session.cc
  ${CMAKE_SOURCE_DIR}/chasky/api/chasky_wrap.cxx
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

add_library(_chasky SHARED ${SOURCE_FILES}
  ${PROTOS_FILES}
  ${SWIG_FILES}
  )

target_link_libraries (_chasky
  "${LOCAL_LIB}/libgtest.a"
  "${LOCAL_LIB}/libgtest_main.a"
  "${LOCAL_LIB}/libglog.a"
  "${LOCAL_LIB}/libprotobuf.a"
  )
set_target_properties(_chasky
        PROPERTIES PREFIX ""
                   SUFFIX ".so"
        )

add_custom_command(
  OUTPUT  ${CMAKE_SOURCE_DIR}/chasky/api/chasky_wrap.cxx
  COMMAND cd ${CMAKE_SOURCE_DIR}/chasky/api; sh compile_swig.sh
  DEPENDS ${CMAKE_SOURCE_DIR}/chasky/api/chasky.swig ${CMAKE_SOURCE_DIR}/chasky/api/chasky_api.h
  )

set( CMAKE_EXPORT_COMPILE_COMMANDS 1 )
