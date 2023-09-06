#pragma once

#include <condition_variable>
#include <memory>
#include <mutex>
#include <thread>
#include <vector>

#include "fixed_buffer.h"
#include "log_file.h"
#include "log_stream.h"

namespace logger {

class AsyncLogger {
 public:
  typedef FixedBuffer<kLargeBuffer> Buffer;

  AsyncLogger(const std::string log_filename, int flush_interval = 2);
  ~AsyncLogger();
  void append(const char* logline, int len);
  void start();
  void stop();

 private:
  void threadFunc();

  const int flush_interval_;
  bool running_;
  std::string basename_;
  std::thread* thread_;
  std::mutex mtx_;
  std::condition_variable cond_;
  std::shared_ptr<Buffer> current_buffer_;
  std::shared_ptr<Buffer> next_buffer_;
  std::vector<std::shared_ptr<Buffer>> buffers_;
};

}  // namespace logger