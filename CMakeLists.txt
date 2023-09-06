cmake_minimum_required(VERSION 3.17)

project(logger)

set(CXX_STANDARD_REQUIRED 17)

add_executable(test
  src/async_logger.h src/async_logger.cc
  src/file_util.h src/file_util.cc
  src/fixed_buffer.h
  src/log_file.h src/log_file.cc
  src/log_stream.h src/log_stream.cc
  src/logger.h src/logger.cc
  test.cc
)
