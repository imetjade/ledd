// Copyright (c) 2020 SmartThings. All Rights Reserved.

#include "ledPlatformGenericLinkV2.h"

#include <errno.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>

#include "ledPlatformLinkV2.h"
#include "log.h"

typedef struct ledPlatformGenericLinkV2_s
{
  ledPlatform_t linkV2Platform;
} ledPlatformGenericLinkV2_t;

static inline ledPlatformGenericLinkV2_t *_getSelf(ledPlatform_t *platform)
{
  return platform->data;
}

static bool _setup(ledPlatform_t *platform)
{
  ledPlatformGenericLinkV2_t *self = _getSelf(platform);

  return true;
}

static bool _destroy(ledPlatform_t *platform)
{
  ledPlatformGenericLinkV2_t *self = _getSelf(platform);

  return true;
}

bool ledPlatformGenericLinkV2_init(ledPlatform_t *platform)
{
  // clang-format off
  static const ledPlatform_ops_t PLATFORM_OPS = {.setup = _setup,
                                                 .destroy = _destroy,
                                                 .setLedSignal = NULL,
                                                 .setHubStatus = NULL,
                                                 .canIgnoreExitSignal = NULL};
  // clang-format on

  ledPlatformGenericLinkV2_t *self = malloc(sizeof *self);
  if (NULL == self)
  {
    LOG_ERROR("Failed to allocate memory for platform");
    return false;
  }
  memset(self, 0, sizeof(ledPlatformGenericLinkV2_t));

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
