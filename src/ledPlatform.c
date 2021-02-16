// Copyright (c) 2020 SmartThings. All Rights Reserved.

#include "ledPlatform.h"

#include <stdlib.h>
#include <string.h>

#define GET_OP(_PP_PLAT, _OPNAME, _P_RESOP)                                    \
  {                                                                            \
    void *op__ = NULL;                                                         \
    for (ledPlatform_t *layer__ = *_PP_PLAT; layer__ != NULL;                  \
         layer__ = layer__->parent)                                            \
    {                                                                          \
      if (layer__->ops._OPNAME)                                                \
      {                                                                        \
        op__ = layer__->ops._OPNAME;                                           \
        *_PP_PLAT = layer__;                                                   \
        break;                                                                 \
      }                                                                        \
    }                                                                          \
    *_P_RESOP = op__;                                                          \
  }                                                                            \
  while (0)

const char *ledSignal_toString(leddWashSignal_t ledSignal)
{
  static const char *signalStrings[] = {LEDD_WASH_SIGNAL_LIST(AS_STRING)};
  return signalStrings[ledSignal];
}

const char *ledHubStatus_toString(ledHubStatus_t hubStatus)
{
  // clang-format off
  static const char *hubStatusStrings[] = {"unknown",
                                           "ready_to_setup",
                                           "running",
                                           "need_to_reset",
                                           "claimed",
                                           "unclaimed",
                                           "reset_completed",
                                           "initialized"};
  // clang-format on
  if (hubStatus < LED_HUB_STATUS_UNKNOWN || hubStatus >= LED_HUB_STATUS_COUNT)
  {
    hubStatus = LED_HUB_STATUS_UNKNOWN;
  }
  return hubStatusStrings[hubStatus];
}

void ledPlatform_init(ledPlatform_t *platform,
                      ledPlatform_ops_t const *ops,
                      void *data)
{
  platform->data = data;
  platform->parent = NULL;
  memcpy(&platform->ops, ops, sizeof *ops);
}

void ledPlatform_appendParent(ledPlatform_t *platform, ledPlatform_t *parent)
{
  if (NULL == platform->parent) // base case
  {
    platform->parent = parent;
  }
  else // recurse
  {
    ledPlatform_appendParent(platform->parent, parent);
  }
}

bool ledPlatform_setup(ledPlatform_t *platform)
{
  // setup is called for all platforms, depth first
  bool success = true;
  if (platform->parent)
  {
    success = ledPlatform_setup(platform->parent);
  }

  if (platform->ops.setup)
  {
    success = success && platform->ops.setup(platform);
  }

  return success;
}

bool ledPlatform_destroy(ledPlatform_t *platform)
{
  // destroy is called for all platforms, depth first
  bool success = true;

  if (platform->parent)
  {
    success = ledPlatform_destroy(platform->parent);
  }

  if (platform->ops.destroy)
  {
    success = success && platform->ops.destroy(platform);
  }

  return success;
}

bool ledPlatform_setLedSignal(ledPlatform_t *platform,
                              leddWashSignal_t ledSignal)
{
  ledPlatform_setLedSignalFn_t op = NULL;
  GET_OP(&platform, setLedSignal, &op);

  return op ? op(platform, ledSignal) : false;
}

bool ledPlatform_setHubStatus(ledPlatform_t *platform, ledHubStatus_t hubStatus)
{
  ledPlatform_setHubStatusFn_t op = NULL;
  GET_OP(&platform, setHubStatus, &op);

  return op ? op(platform, hubStatus) : false;
}

bool ledPlatform_canIgnoreExitSignal(ledPlatform_t *platform)
{
  ledPlatform_canIgnoreExitSignalFn_t op = NULL;
  GET_OP(&platform, canIgnoreExitSignal, &op);

  return op ? op(platform) : false;
}
