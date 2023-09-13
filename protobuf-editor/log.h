/************************************************************************
 * Copyright (c) 2023 Ophir Carmi
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#ifndef LOG_H_
#define LOG_H_

#include <stdio.h>

#ifndef RELEASE
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wshadow"
#pragma GCC diagnostic ignored "-Wconversion"
#pragma GCC diagnostic ignored "-Wzero-as-null-pointer-constant"
#pragma GCC diagnostic ignored "-Wpedantic"
#include <icecream.hpp>
#pragma GCC diagnostic pop
#endif /* RELEASE */

#define PBE_LOG_ERROR(...)                                      \
  {                                                             \
    fprintf(stderr, "%s (%d): \033[1;31m", __FILE__, __LINE__); \
    fprintf(stderr, __VA_ARGS__);                               \
    fprintf(stderr, "\033[0m");                                 \
    fflush(stderr);                                             \
  }
#define PBE_LOG_WARNING(...)                                    \
  {                                                             \
    fprintf(stderr, "%s (%d): \033[1;33m", __FILE__, __LINE__); \
    fprintf(stderr, __VA_ARGS__);                               \
    fprintf(stderr, "\033[0m");                                 \
    fflush(stderr);                                             \
  }

#define PBE_LOG_INFO(...)          \
  {                                \
    fprintf(stdout, "\033[1;32m"); \
    fprintf(stdout, __VA_ARGS__);  \
    fprintf(stdout, "\033[0m");    \
    fflush(stdout);                \
  }

#define PBE_LOG_DEBUG(...) fprintf(stdout, __VA_ARGS__)

#endif  // LOG_H_
