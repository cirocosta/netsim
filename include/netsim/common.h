#ifndef FSSIM_COMMON_H
#define FSSIM_COMMON_H

#define KGRN "\x1B[32m"
#define KBRED "\x1B[31;1m"
#define KRESET "\033[0m"

#include "netsim/constants.h"

#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <valgrind/valgrind.h>

#define __FILENAME__                                                           \
  (strrchr(__FILE__, '/') ? strrchr(__FILE__, '/') + 1 : __FILE__)

#define LOG(__msg, ...)                                                        \
  do {                                                                         \
    fprintf(stdout, "\n(LOG)\t");                                              \
    fprintf(stdout, __msg, ##__VA_ARGS__);                                     \
  } while (0)

#define LOGERR(__msg, ...)                                                     \
  do {                                                                         \
    fprintf(stderr, "\n(LOGERR)\t");                                           \
    fprintf(stderr, __msg, ##__VA_ARGS__);                                     \
  } while (0)

#define ASSERT(__cond, __msg, ...)                                             \
  do {                                                                         \
    if (!(__cond)) {                                                           \
      VALGRIND_PRINTF_BACKTRACE("\nStack trace @ %s(), %d", __func__,          \
                                __LINE__);                                     \
      fprintf(stderr, "\n" __BASE_FILE__ ": %2d\n", __LINE__);                 \
      fprintf(stderr, KBRED "Assertion `%s` failed\n" KRESET, #__cond);        \
      fprintf(stderr, "\t" __msg "\n", ##__VA_ARGS__);                         \
      exit(EXIT_FAILURE);                                                      \
    }                                                                          \
  } while (0)

#define PASSERT(condition, message, ...)                                       \
  do {                                                                         \
    if (!(condition)) {                                                        \
      VALGRIND_PRINTF_BACKTRACE("\nStack trace @ %s(), %d", __func__,          \
                                __LINE__);                                     \
      fprintf(stderr, "\n" __BASE_FILE__ ": %2d\n", __LINE__);                 \
      fprintf(stderr, message, ##__VA_ARGS__);                                 \
      fprintf(stderr, "%s\n", strerror(errno));                                \
      exit(EXIT_FAILURE);                                                      \
    }                                                                          \
  } while (0)

#define TEST(__test, ...)                                                      \
  do {                                                                         \
    LOG("RUN\t%s: %s:" #__VA_ARGS__, __FILENAME__, #__test);                   \
    fflush(stdout);                                                            \
    __test();                                                                  \
    LOG("OK\t\t%s: %s%s%s:" #__VA_ARGS__, __FILENAME__, KGRN, #__test,         \
        KRESET);                                                               \
    fflush(stdout);                                                            \
  } while (0)

#define STRNCMP(__a, __b)                                                      \
  ASSERT(!strncmp(__a, __b, strlen(__b)), "`%s` != `%s`", __a, __b)

#ifndef NDEBUG
#define DASSERT(__cond, __msg, ...) ASSERT(__cond, __msg, #__VA_ARGS__)
#else
#define DASSERT(__cond, __msg, ...)                                            \
  do {                                                                         \
  } while (0)
#endif

#endif // ! FSSIM_COMMON_H
