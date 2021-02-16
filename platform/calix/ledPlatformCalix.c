// Copyright (c) 2020 SmartThings. All Rights Reserved.

#include "ledPlatformCalix.h"

#include <stdlib.h>

#include "ledCalixSignal.h"
#include "ledGDbus.h"
#include "log.h"

typedef struct ledPlatformCalix_s
{
  ledGDbus_t gdbus;
  ledCalixSignal_t prevSignal;
} ledPlatformCalix_t;

static inline ledPlatformCalix_t *_getLedPlatformCalix(ledPlatform_t *platform)
{
  return platform->data;
}

static void _clearAllLedSignal(ledPlatform_t *platform)
{
  ledPlatformCalix_t *ledPlatformCalix = _getLedPlatformCalix(platform);

  ledGDbus_setLedSignal(&ledPlatformCalix->gdbus, c_st_hub_setup_ready, false);
  ledGDbus_setLedSignal(&ledPlatformCalix->gdbus, c_st_setup_error, false);
  ledGDbus_setLedSignal(&ledPlatformCalix->gdbus, c_st_search, false);
}

static bool _setup(ledPlatform_t *platform)
{
  ledPlatformCalix_t *ledPlatformCalix = _getLedPlatformCalix(platform);

  if (!ledGDbus_setup(&ledPlatformCalix->gdbus))
  {
    return false;
  }

  _clearAllLedSignal(platform);

  return true;
}

static bool _destroy(ledPlatform_t *platform)
{
  ledPlatformCalix_t *ledPlatformCalix = _getLedPlatformCalix(platform);

  ledGDbus_setLedSignal(&ledPlatformCalix->gdbus, ledPlatformCalix->prevSignal,
                        false);
  ledGDbus_destroy(&ledPlatformCalix->gdbus);

  return true;
}

static bool _setLedSignal(ledPlatform_t *platform, leddWashSignal_t ledSignal)
{
  ledCalixSignal_t newSignal = LED_CALIX_SIGNAL_INVALID;
  ledPlatformCalix_t *ledPlatformCalix = _getLedPlatformCalix(platform);

  if (ledSignal < 0 || ledSignal >= LEDD_WASH_SIGNAL_COUNT)
  {
    LOG_ERROR("Invalid LEDD Signal: %d", ledSignal);
    return true;
  }

  LOG_DEBUG("Received LED event: %s", ledSignal_toString(ledSignal));

  switch (ledSignal)
  {
    case LEDD_WASH_READY_TO_SETUP:
      newSignal = c_st_hub_setup_ready;
      break;
    case LEDD_WASH_HUB_STATUS_WORKING:
      newSignal = LED_CALIX_SIGNAL_COUNT;
      break;
    case LEDD_WASH_SEARCHING_SENSOR:
      newSignal = c_st_search;
      break;
    case LEDD_WASH_ERROR:
    case LEDD_WASH_HUB_STATUS_PARTIALLY_WORKING:
    case LEDD_WASH_HUB_STATUS_NOT_WORKING:
      newSignal = c_st_setup_error;
      break;
    case LEDD_WASH_SET_UP_PROCESSING:
    case LEDD_WASH_ZIGBEE_DEVICE_CALL_FEEDBACK:
    case LEDD_WASH_FACTORY_RESET:
    case LEDD_WASH_FACTORY_RESET_DONE:
    case LEDD_WASH_LOG_UPLOAD_START:
    case LEDD_WASH_LOG_UPLOAD_STOP:
    case LEDD_WASH_FIRMWARE_UPDATE_START:
    case LEDD_WASH_FIRMWARE_UPDATE_STOP:
    case LEDD_WASH_SET_PREV_STATE:
    case LEDD_WASH_MFG_TEST:
    case LEDD_WASH_MFG_TEST_DONE:
    default:
      break;
  }

  // Do not set LED state if its already set
  if ((newSignal != LED_CALIX_SIGNAL_INVALID) &&
      (newSignal != ledPlatformCalix->prevSignal))
  {
    // clear previous led state
    ledGDbus_setLedSignal(&ledPlatformCalix->gdbus,
                          ledPlatformCalix->prevSignal, false);

    // set new led state
    ledGDbus_setLedSignal(&ledPlatformCalix->gdbus, newSignal, true);

    // keep track of state that is being set so it can be cleared next time
    ledPlatformCalix->prevSignal = newSignal;
  }

  return true;
}

bool ledPlatformCalix_init(ledPlatform_t *platform)
{
  // clang-format off
  static const ledPlatform_ops_t PLATFORM_OPS = {.setup = _setup,
                                                 .destroy = _destroy,
                                                 .setLedSignal = _setLedSignal,
                                                 .setHubStatus = NULL,
                                                 .canIgnoreExitSignal = NULL};
  // clang-format on

  ledPlatformCalix_t *ledPlatformCalix = malloc(sizeof *ledPlatformCalix);
  if (NULL == ledPlatformCalix)
  {
    LOG_ERROR("Failed to allocate memory for platform");
    return false;
  }

  memset(ledPlatformCalix, 0, sizeof(ledPlatformCalix_t));
  ledPlatformCalix->prevSignal = LED_CALIX_SIGNAL_COUNT;

  ledPlatform_init(platform, &PLATFORM_OPS, ledPlatformCalix);

  return true;
}
