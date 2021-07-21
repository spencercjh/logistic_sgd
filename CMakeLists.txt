cmake_minimum_required(VERSION 3.8)
project(logistic_sgd)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_COMPILER "mpic++")



INCLUDE_DIRECTORIES(${PROJECT_SOURCE_DIR}/include)
SET(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)
aux_source_directory(${PROJECT_SOURCE_DIR}/src SRC_FILE)
#aux_source_directory(${PROJECT_SOURCE_DIR}/src/optimizer SRC_FILE)
#aux_source_directory(${PROJECT_SOURCE_DIR}/src/data SRC_FILE)
#aux_source_directory(${PROJECT_SOURCE_DIR}/src/math SRC_FILE)
#aux_source_directory(${PROJECT_SOURCE_DIR}/src/other SRC_FILE)

add_executable(logistic_sgd ${SRC_FILE} src/properties.cpp include/properties.h src/string_util.cpp include/string_util.h src/type_convert.cpp include/type_convert.h Communication/ring_allreduce.h include/common.h Communication/reduce_operator.h Communication/p2p_communication.h Communication/simple_allreduce.h utils/utils.cpp utils/utils.h src/neighbors.cpp include/neighbors.h)
