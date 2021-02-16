// Copyright (c) 2020 SmartThings. All Rights Reserved.

#ifndef __LED_CALIX_SIGNAL_H__
#define __LED_CALIX_SIGNAL_H__

#include "ledSignals.h"

/* Reference LED signals for calix
 * c_st_hub_setup_ready
 * c_st_setup_error
 * c_st_search
 */
#define LED_CALIX_SIGNAL_LIST(_X_)                                             \
  _X_(LED_CALIX_SIGNAL_INVALID)                                                \
  _X_(c_st_hub_setup_ready)                                                    \
  _X_(c_st_setup_error)                                                        \
  _X_(c_st_search)                                                             \
  _X_(LED_CALIX_SIGNAL_COUNT)

typedef enum ledCalixSignal_e
{
  LED_CALIX_SIGNAL_LIST(AS_ENUM)
} ledCalixSignal_t;

#endif /* __LED_CALIX_SIGNAL_H__ */
