cmake_minimum_required(VERSION 3.17)

project(logger)

set(CXX_STANDARD_REQUIRED 17)

set(LOGGER_INCLUDE_DIR ${PROJECT_SOURCE_DIR}/src)

add_executable(test ${LOGGER_INCLUDE_DIR} test.cc)


 