#pragma once

#include <sys/time.h>

#include "async_logger.h"
#include "fixed_buffer.h"

namespace logger {

class Impl {
 public:
  Impl(const char* filename, int line);
  void formatTime();
  std::string getBaseName() { return basename_; }
  LogStream& stream() { return stream_; }
  int getLine() { return line_; }

 private:
  LogStream stream_;
  int line_;
  std::string basename_;
};

class Logger {
 public:
  static std::string log_filename_;
  Logger(const char* filename, int line) : impl(filename, line) {}
  ~Logger();
  LogStream& stream() { return impl.stream(); }
  static std::string getLogFileName() { return log_filename_; }

 private:
  Impl impl;
};

#define LOG Logger(__FILE__, __LINE__).stream()

}  // namespace logger