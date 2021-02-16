// Copyright (c) 2020 SmartThings. All Rights Reserved.

#ifndef __LED_FILE_IPC_H__
#define __LED_FILE_IPC_H__

#include <stdbool.h>

typedef struct ledFileIPC_s
{
  int fd;
} ledFileIPC_t;

bool ledFileIPC_setup(ledFileIPC_t *self, const char *filePath);

#endif /*__LED_FILE_IPC_H__*/
