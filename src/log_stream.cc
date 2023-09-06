#include "log_stream.h"

#include <algorithm>
#include <cstddef>

#include "fixed_buffer.h"
#include "logger.h"

namespace logger {

template <typename T>
size_t convert(char buf[], T value) {
  char* p = buf;
  bool minus = value < 0 ? true : false;
  while (value) {
    *p++ = (value % 10) + '0';
    value /= 10;
  }
  if (minus) {
    *p++ = '-';
  }
  *p = '\0';
  std::reverse(buf, p);
  return p - buf;
}

template <typename T>
void LogStream::formatInteger(T value) {
  if (buffer_.avail() >= kMaxNumbericSize) {
    size_t len = convert(buffer_.current(), value);
    buffer_.add(len);
  }
}

template <typename T>
void LogStream::formatDecimal(T value) {
  if (buffer_.avail() >= kMaxNumbericSize) {
    int len = snprintf(buffer_.current(), kMaxNumbericSize, "%.12g", value);
    buffer_.add(len);
  }
}

LogStream& LogStream::operator<<(bool value) {
  buffer_.append(value ? "1" : "0", 1);
  return *this;
}

LogStream& LogStream::operator<<(short value) {
  formatInteger(value);
  return *this;
}

LogStream& LogStream::operator<<(unsigned short value) {
  formatInteger(value);
  return *this;
}

LogStream& LogStream::operator<<(int value) {
  formatInteger(value);
  return *this;
}

LogStream& LogStream::operator<<(unsigned int value) {
  formatInteger(value);
  return *this;
}

LogStream& LogStream::operator<<(long value) {
  formatInteger(value);
  return *this;
}

LogStream& LogStream::operator<<(unsigned long value) {
  formatInteger(value);
  return *this;
}

LogStream& LogStream::operator<<(long long value) {
  formatInteger(value);
  return *this;
}

LogStream& LogStream::operator<<(unsigned long long value) {
  formatInteger(value);
  return *this;
}

LogStream& LogStream::operator<<(float value) {
  formatDecimal(value);
  return *this;
}

LogStream& LogStream::operator<<(double value) {
  formatDecimal(value);
  return *this;
}

LogStream& LogStream::operator<<(char value) {
  buffer_.append(&value, 1);
  return *this;
}

LogStream& LogStream::operator<<(const char* value) {
  if (value) {
    buffer_.append(value, strlen(value));
  } else {
    buffer_.append("(null)", 6);
  }
  return *this;
}

LogStream& LogStream::operator<<(const std::string& value) {
  buffer_.append(value.c_str(), value.size());
  return *this;
}

void LogStream::append(const char* data, int len) { buffer_.append(data, len); }

const LogStream::Buffer& LogStream::buffer() const { return buffer_; }

void LogStream::resetBuffer() { buffer_.reset(); }

}  // namespace logger