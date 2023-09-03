#pragma once

#include <cstdio>
#include <string>

#include "fixed_buffer.h"

namespace logger {

class LogStream {
 public:
  LogStream& operator<<(bool);
  LogStream& operator<<(short);
  LogStream& operator<<(unsigned short);
  LogStream& operator<<(int);
  LogStream& operator<<(unsigned int);
  LogStream& operator<<(long);
  LogStream& operator<<(unsigned long);
  LogStream& operator<<(long long);
  LogStream& operator<<(unsigned long long);
  LogStream& operator<<(float);
  LogStream& operator<<(double);
  LogStream& operator<<(char);
  LogStream& operator<<(const char*);
  LogStream& operator<<(const std::string&);
  void append(const char* data, int len);
  const FixedBuffer<kSmallBuffer>& buffer() const;
  void resetBuffer();

 private:
  //TODO
  void staticCheck();

  template <typename T>
  void formatInteger(T);

  template <typename T>
  void formatDecimal(T);

  FixedBuffer<kSmallBuffer> buffer_;
  static const int kMaxNumbericSize = 32;
};

}  // namespace logger