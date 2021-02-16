// Copyright (c) 2020, SmartThings. All Rights Reserved.

#ifndef __LOG_H__
#define __LOG_H__

#include <stdio.h>
#include <string.h>

#define LOG_LEVEL_DEBUG 1
#define LOG_LEVEL_INFO 2
#define LOG_LEVEL_ERROR 3

#ifndef LOG_LEVEL
#define LOG_LEVEL (LOG_LEVEL_INFO)
#endif

#ifndef LOG_MAX_LINE_LENGTH
#define LOG_MAX_LINE_LENGTH (1400)
#endif

#define LOG_DO_PRINTLN(...)                                                    \
  do                                                                           \
  {                                                                            \
    char message[LOG_MAX_LINE_LENGTH];                                         \
    snprintf(message, sizeof message, ##__VA_ARGS__);                          \
    fprintf(stderr, "%s\n", message);                                          \
  } while (0)

#if LOG_LEVEL <= LOG_LEVEL_DEBUG
#define LOG_DEBUG(...) LOG_DO_PRINTLN(__VA_ARGS__);
#else
#define LOG_DEBUG(...)
#endif

#if LOG_LEVEL <= LOG_LEVEL_INFO
#define LOG_INFO(...) LOG_DO_PRINTLN(__VA_ARGS__);
#else
#define LOG_INFO(...)
#endif

#if LOG_LEVEL <= LOG_LEVEL_ERROR
#define LOG_ERROR(...) LOG_DO_PRINTLN(__VA_ARGS__);
#else
#define LOG_ERROR(...)
#endif
#endif // __LOG_H__
