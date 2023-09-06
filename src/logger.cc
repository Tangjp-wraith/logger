#include "logger.h"

#include <pthread.h>

namespace logger {
static pthread_once_t once_control = PTHREAD_ONCE_INIT;

std::string Logger::log_filename_ = "./log/log";

Impl::Impl(const char* filename, int line) : line_(line), basename_(filename) {
  formatTime();
}

void Impl::formatTime() {
  struct timeval tv;
  time_t time;
  char str_t[26] = {0};
  gettimeofday(&tv, NULL);
  time = tv.tv_sec;
  struct tm* p_time = localtime(&time);
  strftime(str_t, 26, "%Y-%m-%d %H:%M:%S\n", p_time);
  stream_ << str_t;
}

std::unique_ptr<AsyncLogger>& getAsyncLogger() {
  static std::unique_ptr<AsyncLogger> AsyncLog(
      new AsyncLogger(Logger::getLogFileName()));
  return AsyncLog;
}

void init() {
  Logger::log_filename_ = "./log/log";
  getAsyncLogger()->start();
}

void output(const char* msg, int len) {
  pthread_once(&once_control, init);
  getAsyncLogger()->append(msg, len);
}

Logger::~Logger() {
  stream() << " -- " << impl.getBaseName() << ":" << impl.getLine() << "\n";
  const LogStream::Buffer& buf(impl.stream().buffer());
  output(buf.data(), buf.length());
}


}  // namespace logger