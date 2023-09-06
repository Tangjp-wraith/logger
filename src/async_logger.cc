#include "async_logger.h"

#include <chrono>
#include <memory>
#include <mutex>

#include "fixed_buffer.h"
#include "log_file.h"

namespace logger {

AsyncLogger::AsyncLogger(const std::string log_filename, int flush_interval)
    : flush_interval_(flush_interval),
      running_(false),
      basename_(log_filename),
      thread_(new std::thread(&AsyncLogger::threadFunc, this)),
      current_buffer_(std::make_shared<Buffer>()),
      next_buffer_(std::make_shared<Buffer>()) {}

AsyncLogger::~AsyncLogger() {
  if (running_) {
    stop();
  }
}

void AsyncLogger::append(const char *logline, int len) {
  std::lock_guard<std::mutex> locker(mtx_);
  if (current_buffer_->avail() > len) {
    current_buffer_->append(logline, len);
  } else {
    buffers_.emplace_back(current_buffer_);
    if (next_buffer_) {
      current_buffer_ = std::move(next_buffer_);
    } else {
      current_buffer_.reset(new Buffer());
    }
    current_buffer_->append(logline, len);
    cond_.notify_one();
  }
}

void AsyncLogger::threadFunc() {
  auto new_buffer1 = std::make_shared<Buffer>();
  auto new_buffer2 = std::make_shared<Buffer>();
  LogFile output(basename_);
  std::vector<std::shared_ptr<Buffer>> buffers_to_write;
  while (running_) {
    {
      std::unique_lock<std::mutex> locker(mtx_);
      if (buffers_.empty()) {
        cond_.wait_for(locker,
                       std::chrono::microseconds(flush_interval_ * 1000));
      }
      buffers_.emplace_back(current_buffer_);
      current_buffer_ = std::move(new_buffer1);
      buffers_to_write.swap(buffers_);
      if (!next_buffer_) {
        next_buffer_ = std::move(new_buffer2);
      }
    }
    for (auto &wi : buffers_to_write) {
      output.append(wi->data(), wi->length());
    }
    if (!new_buffer1) {
      new_buffer1.reset(new Buffer());
    }
    if (!new_buffer2) {
      new_buffer2.reset(new Buffer());
    }
    buffers_to_write.clear();
    output.flush();
  }
  output.flush();
}
void AsyncLogger::start() { running_ = true; }

void AsyncLogger::stop() {
  running_ = false;
  cond_.notify_one();
  thread_->join();
}

}  // namespace logger