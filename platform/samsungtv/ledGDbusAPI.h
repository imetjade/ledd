// Copyright (c) 2020 SmartThings. All Rights Reserved.

#ifndef __LED_GDBUS_API_H__
#define __LED_GDBUS_API_H__

#include <dbus/dbus-glib.h>
#include <gio/gio.h>
#include <glib.h>
#include <stdbool.h>

#include "ledHubStatus.h"

struct ledPlatform_s;

typedef struct ledGDbusAPI_s
{
  struct ledPlatform_s *ledPlatform;
  GDBusNodeInfo *node;
  guint ownId;
  GDBusConnection *connection;
  guint registrationId;
  ledHubStatus_t lastHubStatus;
  bool remainHubStatus;
  bool canIgnoreExitSignal;
} ledGDbusAPI_t;

bool ledGDbusAPI_setup(ledGDbusAPI_t *gdbus, struct ledPlatform_s *ledPlatform);
void ledGDbusAPI_destroy(ledGDbusAPI_t *gdbus);

void ledGDbusAPI_emitHubStatusChanged(ledGDbusAPI_t *gdbus,
                                      ledHubStatus_t hubStatus);
bool ledGDbusAPI_canIgnoreExitSignal(ledGDbusAPI_t *gdbus);

#endif /*__LED_GDBUS_API_H__*/
