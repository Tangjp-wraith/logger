#include "file_util.h"

#include <cstddef>
#include <cstdio>

namespace logger {

FileUtil::FileUtil(std::string filename) : fp_(fopen(filename.c_str(), "ae")) {
  setbuffer(fp_, buffer_, sizeof(buffer_));
}

FileUtil::~FileUtil() { fclose(fp_); }

void FileUtil::append(const char* logline, int len) {
  while (len) {
    size_t x = write(logline, len);
    if (!x) {
      if (ferror(fp_)) {
        perror("FileUtil::append() failed!");
      }
      break;
    }
    logline += x;
    len -= x;
  }
}

void FileUtil::flush() { fflush(fp_); }

int FileUtil::write(const char* logline, int len) {
  return fwrite_unlocked(logline, 1, len, fp_);
}

}  // namespace logger