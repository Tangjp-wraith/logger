#pragma once

#include <iostream>
#include <memory>
#include <mutex>

#include "file_util.h"

namespace logger {

class LogFile {
 public:
  LogFile(const std::string& basename, int flush_every_n = 1024);
  ~LogFile();
  void append(const char* logline, int len);
  void flush();
  bool rollFile();

 private:
  void append_unlocked(const char* logline, int len);

  const std::string basename_;
  const int flush_every_n_;
  int count_;
  std::mutex mtx_;
  std::unique_ptr<FileUtil> file_;
};

}  // namespace logger