#pragma once

#include <cstring>

namespace logger {

const int kSmallBuffer = 4000;
const int kLargeBuffer = 4000 * 1024;

template <int SIZE>

class FixedBuffer {
 public:
  FixedBuffer() : cur_(data_) {}
  ~FixedBuffer() {}
  void append(const char* buf, int len) {  //往缓冲区中添加消息
    if (avail() > len) {
      memcpy(cur_, buf, len);
      cur_ += len;
    }
  }
  const char* data() const { return data_; }
  int length() const { return cur_ - data_; }        // 当前缓冲区长度
  char* current() { return cur_; }                   // 当前位置
  int avail() const { return data_ + SIZE - cur_; }  // 当前可用长度
  void add(int len) { cur_ += len; }        // 往缓冲区中添加数据
  void reset() { cur_ = data_; }            //重置
  void bzero() { memset(data_, 0, SIZE); }  //初始化

 private:
  const char* end() const;
  char data_[SIZE];
  char* cur_;
};
}  // namespace logger