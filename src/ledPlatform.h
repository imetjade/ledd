// Copyright (c) 2020 SmartThings. All Rights Reserved.

#ifndef __LED_PLATFORM_H__
#define __LED_PLATFORM_H__

#include <stdbool.h>
#include <stdint.h>

#include "ledHubStatus.h"
#include "ledSignals.h"

#ifndef AS_STRING
#define AS_STRING(x) #x,
#endif

struct ledPlatform_s;

typedef bool (*ledPlatform_setupFn_t)(struct ledPlatform_s *platform);
typedef bool (*ledPlatform_destroyFn_t)(struct ledPlatform_s *platform);
typedef bool (*ledPlatform_setLedSignalFn_t)(struct ledPlatform_s *platform,
                                             leddWashSignal_t ledSignal);
typedef bool (*ledPlatform_setHubStatusFn_t)(struct ledPlatform_s *platform,
                                             ledHubStatus_t hubStatus);
typedef bool (*ledPlatform_canIgnoreExitSignalFn_t)(
  struct ledPlatform_s *platform);

typedef struct ledPlatform_ops_s
{
  ledPlatform_setupFn_t setup;
  ledPlatform_destroyFn_t destroy;
  ledPlatform_setLedSignalFn_t setLedSignal;
  ledPlatform_setHubStatusFn_t setHubStatus;
  ledPlatform_canIgnoreExitSignalFn_t canIgnoreExitSignal;
} ledPlatform_ops_t;

typedef struct ledPlatform_s
{
  void *data;
  struct ledPlatform_s *parent;
  ledPlatform_ops_t ops;
} ledPlatform_t;

const char *ledSignal_toString(leddWashSignal_t ledSignal);
const char *ledHubStatus_toString(ledHubStatus_t hubStatus);
void ledPlatform_init(ledPlatform_t *platform,
                      ledPlatform_ops_t const *ops,
                      void *data);
void ledPlatform_appendParent(ledPlatform_t *platform, ledPlatform_t *parent);
void ledPlatform_setParent(ledPlatform_t *platform, ledPlatform_t *parent);
bool ledPlatform_setup(ledPlatform_t *platform);
bool ledPlatform_destroy(ledPlatform_t *platform);
bool ledPlatform_setLedSignal(ledPlatform_t *platform,
                              leddWashSignal_t ledSignal);
bool ledPlatform_setHubStatus(ledPlatform_t *platform,
                              ledHubStatus_t hubStatus);
bool ledPlatform_canIgnoreExitSignal(ledPlatform_t *platform);

#endif /*__LED_PLATFORM_H__*/
