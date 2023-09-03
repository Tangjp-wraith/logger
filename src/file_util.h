#pragma once

#include <fstream>
#include <memory>

#define BUFFERSIZE 64 * 1024

namespace logger {

class FileUtil {
 public:
  explicit FileUtil(std::string filename);
  ~FileUtil();
  void append(const char* logline,int len);
  void flush();

 private:
  int write(const char* logline, int len);

  FILE* fp_;
  char buffer_[BUFFERSIZE];
};

}  // namespace logger