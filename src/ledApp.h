// Copyright (c) 2020 SmartThings. All Rights Reserved.

#ifndef __LED_APP_H__
#define __LED_APP_H__

#include "ledFileIPC.h"
#include "ledPlatform.h"
#include "ledPoll.h"

typedef struct ledApp_handle_s
{
  ledPlatform_t platform;
  ledFileIPC_t hubStatusIPC;
  ledFileIPC_t ledSignalIPC;
  ledPoll_t poll;
  int exitCode;
} ledApp_handle_t;

#ifdef __cplusplus
extern "C"
{
#endif // __cplusplus

  ledApp_handle_t *ledApp_getHandle(void);
  void ledApp_close(int exitCode);

#ifdef __cplusplus
}
#endif // __cplusplus

#endif /*__LED_APP_H__*/
