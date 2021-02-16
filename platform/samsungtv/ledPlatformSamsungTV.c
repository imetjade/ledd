// Copyright (c) 2020 SmartThings. All Rights Reserved.

#include "ledPlatformSamsungTV.h"

#include <errno.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>

#include "ledGDbusAPI.h"
#include "ledPlatformLinkV2.h"
#include "log.h"

typedef struct ledPlatformSamsungTV_s
{
  ledPlatform_t linkV2Platform;
  ledGDbusAPI_t gdbus;
} ledPlatformSamsungTV_t;

static inline ledPlatformSamsungTV_t *_getSelf(ledPlatform_t *platform)
{
  return platform->data;
}

static bool _setup(ledPlatform_t *platform)
{
  ledPlatformSamsungTV_t *self = _getSelf(platform);

  if (!ledGDbusAPI_setup(&self->gdbus, platform))
  {
    LOG_ERROR("Failed to setup gdbus api");
    return false;
  }

  return true;
}

static bool _destroy(ledPlatform_t *platform)
{
  ledPlatformSamsungTV_t *self = _getSelf(platform);

  ledGDbusAPI_destroy(&self->gdbus);

  return true;
}

static bool _setHubStatus(ledPlatform_t *platform, ledHubStatus_t hubStatus)
{
  ledPlatformSamsungTV_t *self = _getSelf(platform);

  const char *hubStatusString = ledHubStatus_toString(hubStatus);
  LOG_INFO("hubStatus: %s, %d", hubStatusString, hubStatus);
  ledGDbusAPI_emitHubStatusChanged(&self->gdbus, hubStatus);

  return true;
}

static bool _canIgnoreExitSignal(ledPlatform_t *platform)
{
  ledPlatformSamsungTV_t *self = _getSelf(platform);

  return ledGDbusAPI_canIgnoreExitSignal(&self->gdbus);
}

bool ledPlatformSamsungTV_init(ledPlatform_t *platform)
{
  // clang-format off
  static const ledPlatform_ops_t PLATFORM_OPS = {.setup = _setup,
                                                 .destroy = _destroy,
                                                 .setLedSignal = NULL,
                                                 .setHubStatus = _setHubStatus,
                                                 .canIgnoreExitSignal = _canIgnoreExitSignal};
  // clang-format on

  ledPlatformSamsungTV_t *self = malloc(sizeof *self);
  if (NULL == self)
  {
    LOG_ERROR("Failed to allocate memory for platform");
    return false;
  }
  memset(self, 0, sizeof(ledPlatformSamsungTV_t));

  if (!ledPlatformLinkV2_init(&self->linkV2Platform))
  {
    LOG_ERROR("Failed to initialize linkv2 platform");
    free(self);
    return false;
  }
  ledPlatform_init(platform, &PLATFORM_OPS, self);
  ledPlatform_appendParent(platform, &self->linkV2Platform);

  return true;
}
