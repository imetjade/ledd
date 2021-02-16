// Copyright (c) 2019, SmartThings. All Rights Reserved.
#if !defined(LEDD_SIGNALS_H)
#define LEDD_SIGNALS_H

#define AS_ENUM(x) x,

// TODO: Add some sort of compile time check in code to verify that the version
// is valid
#define LEDD_SIGNALS_VERSION 1

#define LED_STATE_FIFO_NAME "ledstate"

#define LEDD_WASH_SIGNAL_LIST(_X_)                                             \
  _X_(LEDD_WASH_SIGNAL_FIRMWARE_DOWNLOADING)                                   \
  _X_(LEDD_WASH_SET_UP_PROCESSING)                                             \
  _X_(LEDD_WASH_READY_TO_SETUP)                                                \
  _X_(LEDD_WASH_ERROR)                                                         \
  _X_(LEDD_WASH_HUB_STATUS_WORKING)                                            \
  _X_(LEDD_WASH_HUB_STATUS_PARTIALLY_WORKING)                                  \
  _X_(LEDD_WASH_HUB_STATUS_NOT_WORKING)                                        \
  _X_(LEDD_WASH_SEARCHING_SENSOR)                                              \
  _X_(LEDD_WASH_ZIGBEE_DEVICE_CALL_FEEDBACK)                                   \
  _X_(LEDD_WASH_FIRMWARE_UPDATE)                                               \
  _X_(LEDD_WASH_LOG_UPLOAD_START)                                              \
  _X_(LEDD_WASH_LOG_UPLOAD_STOP)                                               \
  _X_(LEDD_WASH_FACTORY_RESET)                                                 \
  _X_(LEDD_WASH_FACTORY_RESET_DONE)                                            \
  _X_(LEDD_WASH_FIRMWARE_UPDATE_START)                                         \
  _X_(LEDD_WASH_FIRMWARE_UPDATE_STOP)                                          \
  _X_(LEDD_WASH_SET_PREV_STATE)                                                \
  _X_(LEDD_WASH_MFG_TEST)                                                      \
  _X_(LEDD_WASH_MFG_TEST_DONE)                                                 \
  _X_(LEDD_WASH_SIGNAL_COUNT)

typedef enum leddWashSignal_e
{
  LEDD_WASH_SIGNAL_LIST(AS_ENUM)
} leddWashSignal_t;
#endif
