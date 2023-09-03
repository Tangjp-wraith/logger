#include "log_file.h"

#include <memory>

namespace logger {

LogFile::LogFile(const std::string &basename, int flush_every_n)
    : basename_(basename),
      flush_every_n_(flush_every_n),
      count_(0),
      file_(std::make_unique<FileUtil>(basename)) {}

LogFile::~LogFile() {}

void LogFile::append(const char *logline, int len) {
  append_unlocked(logline, len);
}

bool LogFile::rollFile() {
  // TODO rollFile
  return false;
}

void LogFile::append_unlocked(const char *logline, int len) {
  file_->append(logline, len);
  ++count_;
  if (flush_every_n_ == count_) {
    count_ = 0;
    file_->flush();
  }
}

}  // namespace logger