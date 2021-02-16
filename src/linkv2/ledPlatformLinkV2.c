// Copyright (c) 2020 SmartThings. All Rights Reserved.

#include "ledPlatformLinkV2.h"

#include <errno.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>

#include "ledLinkv2Cmd.h"
#include "ledSignals.h"
#include "log.h"

#ifndef AS_STRING
#define AS_STRING(x) #x,
#endif

#define PREV_LED_SIGNAL_FILENAME "prevLedSignal"
#define ZIGBEE_FW_UPDATE_FILENAME "stZigbeeFwUpdate"
#define LED_BLINK_COUNT (0xff)

typedef struct ledPlatformLinkV2_s
{
  ledLinkv2Cmd_t linkv2Cmd;
  leddWashSignal_t prevSignal;
  int prevSignalFd;
  char prevSignalPath[512];
  bool isLEDChangeBlocked;
  char zigbeeFwUpdatePath[512];
} ledPlatformLinkV2_t;

static inline ledPlatformLinkV2_t *_getLedPlatformLinkV2(ledPlatform_t *platform)
{
  return platform->data;
}

static const char *_getEnvDefault(char const *key, char const *def)
{
  const char *res = getenv(key);

  return res ? res : def;
}

static void _readPrevLedSignal(ledPlatformLinkV2_t *platform)
{
  uint8_t ledSignal = 0;
  snprintf(platform->prevSignalPath, sizeof platform->prevSignalPath, "%s/%s",
           _getEnvDefault("HUB_TMP_PATH", "/tmp"), PREV_LED_SIGNAL_FILENAME);
  platform->prevSignalFd = open(platform->prevSignalPath, O_RDWR);
  if (platform->prevSignalFd > 0)
  {
    if (read(platform->prevSignalFd, &ledSignal, sizeof ledSignal) !=
        sizeof ledSignal)
    {
      LOG_ERROR("Failed read ledd signal errno - %d/%s", errno,
                strerror(errno));
    }
  }
  else
  {
    platform->prevSignalFd =
      open(platform->prevSignalPath, O_CREAT | O_RDWR, 0600);
    if (platform->prevSignalFd < 0)
    {
      LOG_ERROR("Failed to open %s, errno - %d/%s", platform->prevSignalPath,
                errno, strerror(errno));
    }

    LOG_INFO("Opening new ledd signal file");
  }
  LOG_INFO("Initial LED signal %x", ledSignal);

  platform->prevSignal = ledSignal;
}

static bool _savePrevLedSignal(ledPlatformLinkV2_t *platform)
{
  if (lseek(platform->prevSignalFd, 0, SEEK_SET) != 0)
  {
    LOG_ERROR("Failed to seek to offset 0 in %s", platform->prevSignalPath);
    return false;
  }

  int result = write(platform->prevSignalFd, &platform->prevSignal,
                     sizeof platform->prevSignal);
  if (result != sizeof platform->prevSignal)
  {
    LOG_ERROR("Failed to write ledd signal. result: %d, errno: %d/%s", result,
              errno, strerror(errno));
    return false;
  }
  result = fsync(platform->prevSignalFd);
  if (result != 0)
  {
    LOG_ERROR("Failed to fsync ledd signal. result: %d, errno: %d/%s", result,
              errno, strerror(errno));
    return false;
  }
  return true;
}

static bool _setup(ledPlatform_t *platform)
{
  ledPlatformLinkV2_t *ledPlatformLinkV2 = _getLedPlatformLinkV2(platform);

  snprintf(ledPlatformLinkV2->zigbeeFwUpdatePath,
           sizeof ledPlatformLinkV2->zigbeeFwUpdatePath, "%s/%s",
           _getEnvDefault("HUB_DATA_PATH", "/hub/data"),
           ZIGBEE_FW_UPDATE_FILENAME);

  _readPrevLedSignal(ledPlatformLinkV2);

  if (!ledLinkv2Cmd_setup(&ledPlatformLinkV2->linkv2Cmd))
  {
    LOG_ERROR("Failed to init Linkv2 cmd");
    return false;
  }
  ledLinkv2Cmd_offSolid(&ledPlatformLinkV2->linkv2Cmd);

  return true;
}

static bool _destroy(ledPlatform_t *platform)
{
  ledPlatformLinkV2_t *ledPlatformLinkV2 = _getLedPlatformLinkV2(platform);

  ledLinkv2Cmd_destroy(&ledPlatformLinkV2->linkv2Cmd);

  return true;
}

// The reason of for checking the firmware update status using
// "stZigbeeFwUpdate" file explicitly: This was done to stop and store any led
// change command that can come in the middle of firmware update. If some
// module requests led change to be done in middle of the firmware update the
// request will be lost as xNCP can't carter to that request. This was done
// when hub-core was responsible to do firmware update and ncp-updater wasn't
// present. We can remove this method now because firmware update is the first
// thing that happens when hub-firmware starts.
//
// However, if in future we decide to do zigbee firmware update via hub-core,
// we would need this method to be present because there is a chance of missing
// the led states if it changes in middle of firmware update.
//
// Reference:
// https://github.com/PhysicalGraph/hub-wash-sdk/pull/124#discussion_r387493706
static bool isZigbeeFwUpdating(ledPlatformLinkV2_t *ledPlatformLinkV2)
{
  FILE *fd = fopen(ledPlatformLinkV2->zigbeeFwUpdatePath, "r");
  if (fd == NULL)
  {
    LOG_INFO("File not opened : %s", strerror(errno));
    return false;
  }

  // Read Zigbee FW update state file
  fscanf(fd, "%d", (int *)(&ledPlatformLinkV2->isLEDChangeBlocked));
  LOG_INFO("Zigbee FW update state: %d", ledPlatformLinkV2->isLEDChangeBlocked);
  fclose(fd);
  return ledPlatformLinkV2->isLEDChangeBlocked;
}

static bool _setLedSignal(ledPlatform_t *platform, leddWashSignal_t ledSignal)
{
  ledPlatformLinkV2_t *ledPlatformLinkV2 = _getLedPlatformLinkV2(platform);

  if (ledSignal < 0 || ledSignal >= LEDD_WASH_SIGNAL_COUNT)
  {
    LOG_ERROR("Invalid signal: %d", ledSignal);
    return true;
  }

  if (ledSignal < LEDD_WASH_LOG_UPLOAD_START)
  {
    ledPlatformLinkV2->prevSignal = ledSignal;
    LOG_INFO("Previous signal: %d", ledPlatformLinkV2->prevSignal);
    _savePrevLedSignal(ledPlatformLinkV2);

    // Since hubCore is restarted as a part of Factory reset, it would try to
    // set states like WORKING(Solid Green LED) or PARTIALLY_WORKING (Yellow)
    // when it starts again. During factory reset, the LED is supposed to be
    // continuously blinking green. This change is added to ignore any other
    // LED states while factory reset is going on, so that the LED is always
    // blinking green during factory reset.
    //
    // Reference:
    // https://github.com/PhysicalGraph/hub-wash-sdk/pull/124#discussion_r388072917
    if (ledPlatformLinkV2->isLEDChangeBlocked)
    {
      return true;
    }
  }

  LOG_INFO("LED signal: %s", ledSignal_toString(ledSignal));
  switch (ledSignal)
  {
    case LEDD_WASH_SIGNAL_FIRMWARE_DOWNLOADING:
      // TODO: When to unset the bit??
      ledLinkv2Cmd_setBlueBlink(&ledPlatformLinkV2->linkv2Cmd);
      break;
    case LEDD_WASH_SET_UP_PROCESSING:
      ledLinkv2Cmd_setGreenBlink(&ledPlatformLinkV2->linkv2Cmd);
      break;
    case LEDD_WASH_READY_TO_SETUP:
      ledLinkv2Cmd_setRedGreenBlinkPattern(&ledPlatformLinkV2->linkv2Cmd);
      break;
    case LEDD_WASH_ERROR:
      ledLinkv2Cmd_setRedBlink(&ledPlatformLinkV2->linkv2Cmd);
      break;
    case LEDD_WASH_HUB_STATUS_WORKING:
      ledLinkv2Cmd_setGreenSolid(&ledPlatformLinkV2->linkv2Cmd);
      break;
    case LEDD_WASH_HUB_STATUS_PARTIALLY_WORKING:
      ledLinkv2Cmd_setYellowSolid(&ledPlatformLinkV2->linkv2Cmd);
      break;
    case LEDD_WASH_HUB_STATUS_NOT_WORKING:
      ledLinkv2Cmd_setRedSolid(&ledPlatformLinkV2->linkv2Cmd);
      break;
    case LEDD_WASH_SEARCHING_SENSOR:
      ledLinkv2Cmd_setGreenBlink(&ledPlatformLinkV2->linkv2Cmd);
      break;
    case LEDD_WASH_ZIGBEE_DEVICE_CALL_FEEDBACK:
      ledLinkv2Cmd_setGreenBlink(&ledPlatformLinkV2->linkv2Cmd);
      break;
    case LEDD_WASH_FIRMWARE_UPDATE:
      // TODO: When to unset the bit??
      ledLinkv2Cmd_setGreenBlink(&ledPlatformLinkV2->linkv2Cmd);
      break;
    case LEDD_WASH_FACTORY_RESET:
      ledPlatformLinkV2->isLEDChangeBlocked = true;
      ledLinkv2Cmd_offBlink(&ledPlatformLinkV2->linkv2Cmd);
      sleep(1);
      ledLinkv2Cmd_setGreenBlink(&ledPlatformLinkV2->linkv2Cmd);
      break;
    case LEDD_WASH_FACTORY_RESET_DONE:
      ledPlatformLinkV2->isLEDChangeBlocked = false;
      break;
    case LEDD_WASH_LOG_UPLOAD_START:
      break;
    case LEDD_WASH_LOG_UPLOAD_STOP:
      break;
    case LEDD_WASH_FIRMWARE_UPDATE_START:
      if (!isZigbeeFwUpdating(ledPlatformLinkV2))
      {
        ledLinkv2Cmd_setGreenBlink(&ledPlatformLinkV2->linkv2Cmd);
      }
      break;
    case LEDD_WASH_FIRMWARE_UPDATE_STOP:
      if (!isZigbeeFwUpdating(ledPlatformLinkV2))
      {
        _setLedSignal(platform, ledPlatformLinkV2->prevSignal);
      }
      break;
    case LEDD_WASH_SET_PREV_STATE:
      if (!ledPlatformLinkV2->isLEDChangeBlocked)
      {
        _setLedSignal(platform, ledPlatformLinkV2->prevSignal);
      }
      break;
    case LEDD_WASH_MFG_TEST:
      ledLinkv2Cmd_setRGBBlinkPattern(&ledPlatformLinkV2->linkv2Cmd);
      break;
    case LEDD_WASH_MFG_TEST_DONE:
      ledLinkv2Cmd_offBlink(&ledPlatformLinkV2->linkv2Cmd);
      break;
    default:
      break;
  }

  return true;
}

bool ledPlatformLinkV2_init(ledPlatform_t *platform)
{
  // clang-format off
  static const ledPlatform_ops_t PLATFORM_OPS = {.setup = _setup,
                                                 .destroy = _destroy,
                                                 .setLedSignal = _setLedSignal,
                                                 .setHubStatus = NULL};
  // clang-format on

  ledPlatformLinkV2_t *self = malloc(sizeof *self);
  if (NULL == self)
  {
    LOG_ERROR("Failed to allocate memory for platform");
    return false;
  }

  memset(self, 0, sizeof(ledPlatformLinkV2_t));

  ledPlatform_init(platform, &PLATFORM_OPS, self);

  return true;
}
