// Copyright (c) 2020, SmartThings. All Rights Reserved.

#ifndef __LED_HUB_STATUS_H__
#define __LED_HUB_STATUS_H__

#define AS_ENUM(x) x,

#define LED_HUB_STATUS_FIFO_NAME "hubstatus"

#define LED_HUB_STATUS_LIST(_X_)                                               \
  _X_(LED_HUB_STATUS_UNKNOWN)                                                  \
  _X_(LED_HUB_STATUS_READY_TO_SETUP)                                           \
  _X_(LED_HUB_STATUS_RUNNING)                                                  \
  _X_(LED_HUB_STATUS_NEED_TO_RESET)                                            \
  _X_(LED_HUB_STATUS_CLAIMED)                                                  \
  _X_(LED_HUB_STATUS_UNCLAIMED)                                                \
  _X_(LED_HUB_STATUS_RESET_COMPLETED)                                          \
  _X_(LED_HUB_STATUS_DONGLE_INITIALIZED)                                       \
  _X_(LED_HUB_STATUS_COUNT)

typedef enum ledHubStatus_e
{
  LED_HUB_STATUS_LIST(AS_ENUM)
} ledHubStatus_t;

#endif
