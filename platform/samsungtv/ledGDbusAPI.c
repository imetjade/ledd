// Copyright (c) 2020 SmartThings. All Rights Reserved.

#include "ledGDbusAPI.h"

#include "buttCmd.h"
#include "ledPlatform.h"
#include "log.h"

#define DBUS_BUS_NAME "com.samsung.tv.iot.hub.Service"
#define DBUS_INTERFACE_NAME "com.samsung.tv.iot.hub.ServiceInterface"
#define DBUS_OBJECT_PATH "/com/samsung/tv/iot/hub/Service"

static const gchar introspectionXml[] =
  "<node>"
  "  <interface name='com.samsung.tv.iot.hub.ServiceInterface'>"
  "    <method name='get_hub_status'>"
  "      <arg direction='out' type='s' name='status'/>"
  "    </method>"
  "    <method name='reset_request'>"
  "      <arg direction='out' type='b' name='result'/>"
  "    </method>"
  "    <signal name='hub_status_changed'>"
  "      <arg type='s' name='type'/>"
  "    </signal>"
  "  </interface>"
  "</node>";

static void _handleMethodCall(GDBusConnection *connection,
                              const gchar *sender,
                              const gchar *object_path,
                              const gchar *interface_name,
                              const gchar *method_name,
                              GVariant *parameters,
                              GDBusMethodInvocation *invocation,
                              gpointer user_data)
{
  ledGDbusAPI_t *gdbus = user_data;

  LOG_INFO("dbus method handle: %s", method_name);

  if (g_strcmp0(method_name, "get_hub_status") == 0)
  {
    const char *status = ledHubStatus_toString(gdbus->lastHubStatus);

    LOG_INFO("hubStatus: %s", status);

    g_dbus_method_invocation_return_value(invocation,
                                          g_variant_new("(s)", status));
  }
  else if (g_strcmp0(method_name, "reset_request") == 0)
  {
    gdbus->canIgnoreExitSignal = true;

    gboolean ret = buttCmd_resetHub() ? TRUE : FALSE;

    LOG_INFO("%s", ret ? "succeeded" : "failed");

    gdbus->canIgnoreExitSignal = false;

    g_dbus_method_invocation_return_value(invocation,
                                          g_variant_new("(b)", ret));
  }
}

static const GDBusInterfaceVTable _vtable = {_handleMethodCall, NULL, NULL};

static void _onBusAcquired(GDBusConnection *connection,
                           const gchar *name,
                           gpointer user_data)
{
  ledGDbusAPI_t *gdbus = user_data;
  GError *error = NULL;

  LOG_INFO("Connected to a message bus");

  gdbus->connection = connection;
  gdbus->registrationId =
    g_dbus_connection_register_object(connection, DBUS_OBJECT_PATH,
                                      gdbus->node->interfaces[0], &_vtable,
                                      gdbus, NULL, &error);
  if (gdbus->registrationId == 0)
  {
    LOG_ERROR("Failed to register dbus object: %s", error->message);
    g_clear_error(&error);
  }
}

static void _onNameAcquired(GDBusConnection *connection,
                            const gchar *name,
                            gpointer user_data)
{
  ledGDbusAPI_t *gdbus = user_data;

  LOG_INFO("gdbus name was obtained: %s", name);

  if (gdbus->remainHubStatus)
  {
    ledGDbusAPI_emitHubStatusChanged(gdbus, gdbus->lastHubStatus);
  }
}

static void _onNameLost(GDBusConnection *connection,
                        const gchar *name,
                        gpointer user_data)
{
  LOG_ERROR("gdbus name can't be obtained: %s, gdbus connection: %p", name,
            connection);
}

bool ledGDbusAPI_setup(ledGDbusAPI_t *gdbus, ledPlatform_t *ledPlatform)
{
  GError *error = NULL;

  gdbus->canIgnoreExitSignal = false;
  gdbus->remainHubStatus = false;
  gdbus->ledPlatform = ledPlatform;
  gdbus->node = g_dbus_node_info_new_for_xml(introspectionXml, &error);
  if (gdbus->node == NULL)
  {
    LOG_ERROR("Failed to parse introspection xml: %s", error->message);
    g_clear_error(&error);
    return false;
  }

  gdbus->ownId = g_bus_own_name(G_BUS_TYPE_SYSTEM, DBUS_BUS_NAME,
                                G_BUS_NAME_OWNER_FLAGS_NONE, _onBusAcquired,
                                _onNameAcquired, _onNameLost, gdbus, NULL);
  return true;
}

void ledGDbusAPI_destroy(ledGDbusAPI_t *gdbus)
{
  if (gdbus->ownId)
  {
    g_bus_unown_name(gdbus->ownId);
    gdbus->ownId = 0;
  }
  if (gdbus->node)
  {
    g_dbus_node_info_unref(gdbus->node);
    gdbus->node = NULL;
  }
}

void ledGDbusAPI_emitHubStatusChanged(ledGDbusAPI_t *gdbus,
                                      ledHubStatus_t hubStatus)
{
  const char *type = ledHubStatus_toString(hubStatus);
  GError *error = NULL;

  gdbus->lastHubStatus = hubStatus;
  if (gdbus->connection == NULL)
  {
    LOG_ERROR("Failed to emit %s signal. GDBusConnection is NULL.", type);
    gdbus->remainHubStatus = true;
    return;
  }

  if (g_dbus_connection_emit_signal(gdbus->connection, NULL, DBUS_OBJECT_PATH,
                                    DBUS_INTERFACE_NAME, "hub_status_changed",
                                    g_variant_new("(s)", type), &error))
  {
    LOG_INFO("dbus signal emit: hub_status_changed, %s", type);
  }
  else
  {
    LOG_ERROR("Failed to emit %s signal: %s", type, error->message);
    g_clear_error(&error);
  }
}

bool ledGDbusAPI_canIgnoreExitSignal(ledGDbusAPI_t *gdbus)
{
  return gdbus->canIgnoreExitSignal;
}
