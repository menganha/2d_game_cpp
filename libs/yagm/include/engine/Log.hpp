#pragma once

#ifndef NDEBUG
#  define LTRACE(...) Logger::log(Logger::LOG_TRACE, __VA_ARGS__)
#  define LDEBUG(...) Logger::log(Logger::LOG_DEBUG, __VA_ARGS__)
#  define LINFO(...) Logger::log(Logger::LOG_INFO, __VA_ARGS__)
#  define LWARN(...) Logger::log(Logger::LOG_WARN, __VA_ARGS__)
#  define LERROR(...) Logger::log(Logger::LOG_ERROR, __VA_ARGS__)
#else
#  define LTRACE(...) (void)0
#  define LDEBUG(...) (void)0
#  define LINFO(...) (void)0
#  define LWARN(...) (void)0
#  define LERROR(...) (void)0
#endif

namespace Logger {

enum
{
  LOG_TRACE,
  LOG_DEBUG,
  LOG_INFO,
  LOG_WARN,
  LOG_ERROR
};

// const char *log_level_string(int level);
// void log_set_lock(log_LockFn fn, void *udata);
// void log_set_level(int level);
// void log_set_quiet(bool enable);
// int log_add_callback(log_LogFn fn, void *udata, int level);
// int log_add_fp(FILE *fp, int level);

void
log(int level, const char* fmt, ...) __attribute__((format(printf, 2, 3)));

}
