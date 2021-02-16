// Copyright (c) 2020 SmartThings. All Rights Reserved.

#ifndef __LED_GDBUS_H__
#define __LED_GDBUS_H__

#include <dbus/dbus-glib.h>
#include <gio/gio.h>
#include <glib.h>
#include <stdbool.h>

#include "ledCalixSignal.h"

typedef struct ledGDbus_s
{
  GDBusConnection *conn;
} ledGDbus_t;

bool ledGDbus_setup(ledGDbus_t *gdbus);
void ledGDbus_destroy(ledGDbus_t *gdbus);
void ledGDbus_setLedSignal(ledGDbus_t *gdbus,
                           ledCalixSignal_t ledSignal,
                           bool isSet);

#endif /*__LED_GDBUS_H__*/
