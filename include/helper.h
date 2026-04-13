#pragma once

#include <cstdio>
#include <ctime>
#include <format>

// 缓存行大小
#ifdef __cpp_lib_hardware_interference_size
constexpr std::size_t kCacheLineSize =
    std::hardware_constructive_interference_size;
#else
constexpr std::size_t kCacheLineSize = 64;
#endif

inline thread_local char tls_timebuf[24]{0};

inline const char *gettimestamp() {
  struct timespec ts;
  clock_gettime(CLOCK_REALTIME, &ts);
  struct tm t_info;
  localtime_r(&ts.tv_sec, &t_info);
  strftime(tls_timebuf, 20, "%Y-%m-%d %H:%M:%S", &t_info);
  snprintf(tls_timebuf + 19, 5, ".%d", static_cast<int>(ts.tv_nsec / 1000000));
  return tls_timebuf;
}

#define LOG_D(fmt, ...)                                                        \
  fprintf(stdout, "[%s] [\033[34mDEBUG\033[0m] [%s:%d] %s\n", gettimestamp(),  \
          __FILE__, __LINE__, std::format(fmt, ##__VA_ARGS__).c_str())

#define LOG_I(fmt, ...)                                                        \
  fprintf(stdout, "[%s] [\033[32mINFO\033[0m] [%s:%d] %s\n", gettimestamp(),   \
          __FILE__, __LINE__, std::format(fmt, ##__VA_ARGS__).c_str())

#define LOG_W(fmt, ...)                                                        \
  fprintf(stderr, "[%s] [\033[33mWARN\033[0m] [%s:%d] %s\n", gettimestamp(),   \
          __FILE__, __LINE__, std::format(fmt, ##__VA_ARGS__).c_str())

#define LOG_E(fmt, ...)                                                        \
  fprintf(stderr, "[%s] [\033[31mERROR\033[0m] [%s:%d] %s\n", gettimestamp(),  \
          __FILE__, __LINE__, std::format(fmt, ##__VA_ARGS__).c_str())
