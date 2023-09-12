#ifndef LOG_H_
#define LOG_H_

#include <stdio.h>

#ifndef RELEASE
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wshadow"
#pragma GCC diagnostic ignored "-Wconversion"
#pragma GCC diagnostic ignored "-Wzero-as-null-pointer-constant"
#include <icecream.hpp>
#pragma GCC diagnostic pop
#endif /* RELEASE */

#define PBE_LOG_ERROR(...)                              \
  {                                                             \
    fprintf(stderr, "%s (%d): \033[1;31m", __FILE__, __LINE__); \
    fprintf(stderr, __VA_ARGS__);                               \
    fprintf(stderr, "\033[0m");                                 \
    fflush(stderr);                                             \
  }
#define PBE_LOG_WARNING(...)                            \
  {                                                             \
    fprintf(stderr, "%s (%d): \033[1;33m", __FILE__, __LINE__); \
    fprintf(stderr, __VA_ARGS__);                               \
    fprintf(stderr, "\033[0m");                                 \
    fflush(stderr);                                             \
  }

#define PBE_LOG_INFO(...)  \
  {                                \
    fprintf(stdout, "\033[1;32m"); \
    fprintf(stdout, __VA_ARGS__);  \
    fprintf(stdout, "\033[0m");    \
    fflush(stdout);                \
  }

#define PBE_LOG_DEBUG(...) fprintf(stdout, __VA_ARGS__)

#endif  // LOG_H_
