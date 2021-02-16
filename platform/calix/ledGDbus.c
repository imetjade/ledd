// Copyright (c) 2020 SmartThings. All Rights Reserved.

#include "ledGDbus.h"

#include "log.h"

#ifndef AS_STRING
#define AS_STRING(x) #x,
#endif

#define DBUS_BUS_NAME "com.calix.exos"
#define DBUS_OBJECT_PATH "/com/calix/exos"
#define DBUS_INTERFACE_NAME "com.calix.exos"
#define DBUS_METHOD_NAME "xget"
#define DBUS_LED_CONDITION_SET_PATH "/exec/set-led-condition"
#define DBUS_LED_CONDITION_CLEAR_PATH "/exec/clear-led-condition"

static GDBusConnection *_getGDbusConnection(ledGDbus_t *gdbus)
{
  GDBusConnection *conn = NULL;
  GError *error = NULL;

  if (gdbus->conn != NULL && !g_dbus_connection_is_closed(gdbus->conn))
  {
    return gdbus->conn;
  }

  LOG_INFO("Connecting to the D-Bus..");
  conn = g_bus_get_sync(G_BUS_TYPE_SYSTEM, NULL, &error);
  if (error)
  {
    LOG_ERROR("DBUS connection failed: %s", error->message);
    g_clear_error(&error);
  }
  return conn;
}

bool ledGDbus_setup(ledGDbus_t *gdbus)
{
  gdbus->conn = _getGDbusConnection(gdbus);
  if (gdbus->conn == NULL)
  {
    LOG_ERROR("Failed to create gdbus connection.");
    return false;
  }
  return true;
}

void ledGDbus_destroy(ledGDbus_t *gdbus)
{
  if (gdbus->conn)
  {
    g_object_unref(gdbus->conn);
    gdbus->conn = NULL;
  }
}

static const char *_ledSignal_toString(ledCalixSignal_t ledSignal)
{
  static const char *signalStrings[] = {LED_CALIX_SIGNAL_LIST(AS_STRING)};

  return signalStrings[ledSignal];
}

void ledGDbus_setLedSignal(ledGDbus_t *gdbus,
                           ledCalixSignal_t ledSignal,
                           bool value)
{
  char params[200] = {0};
  GVariant *res = NULL;
  GError *error = NULL;

  if (ledSignal <= LED_CALIX_SIGNAL_INVALID ||
      ledSignal >= LED_CALIX_SIGNAL_COUNT)
  {
    return;
  }

  LOG_INFO("%s %s", value ? "set" : "clear", _ledSignal_toString(ledSignal));

  snprintf(params, sizeof params, "%s[name=\'%s\']",
           value ? DBUS_LED_CONDITION_SET_PATH : DBUS_LED_CONDITION_CLEAR_PATH,
           _ledSignal_toString(ledSignal));

  res =
    g_dbus_connection_call_sync(gdbus->conn, DBUS_BUS_NAME, DBUS_OBJECT_PATH,
                                DBUS_INTERFACE_NAME, DBUS_METHOD_NAME,
                                g_variant_new("(s)", params),
                                G_VARIANT_TYPE("(s)"), G_DBUS_CALL_FLAGS_NONE,
                                -1, NULL, &error);
  if (res == NULL)
  {
    LOG_ERROR("dbus api failed - method: %s, error: %s", params,
              error->message);
    g_clear_error(&error);
    return;
  }

  /* Enable for debugging
  GVariant *response = NULL;
  g_variant_get(res, "(@s)", &response);
  LOG_INFO("dbus api response: %s", g_variant_get_string(response, NULL));
  g_variant_unref(response);
  */

  g_variant_unref(res);
}
