/* Networking stuff for statusd
   Copyright (c) 2022 bellrise */

#include <dbus/dbus.h>
#include <stdio.h>


enum NMConnectivityState
{
	NM_CONNECTIVITY_UNKNOWN = 0,
	NM_CONNECTIVITY_NONE = 1,
	NM_CONNECTIVITY_PORTAL = 2,
	NM_CONNECTIVITY_LIMITED = 3,
	NM_CONNECTIVITY_FULL = 4,
};


static int nm_check_connectivity();


void stat_net(char *buf, int len)
{
	unsigned stat = nm_check_connectivity();

	if (stat <= NM_CONNECTIVITY_PORTAL) {
		/* Unknown, none or portal connectivity. */
		snprintf(buf, len, "");
		return;
	}

	/* Limited or full connection. */
	snprintf(buf, len, "");
}

static int nm_check_connectivity()
{
	DBusConnection *conn;
	DBusMessage *msg;
	DBusMessage *reply;
	DBusError err;
	unsigned res;

	reply = NULL;
	msg = NULL;
	res = 0;

	dbus_error_init(&err);
	conn = dbus_bus_get(DBUS_BUS_SYSTEM, &err);

	if (dbus_error_is_set(&err) || !conn)
		goto done;

	msg = dbus_message_new_method_call("org.freedesktop.NetworkManager",
			"/org/freedesktop/NetworkManager", "org.freedesktop.NetworkManager",
			"CheckConnectivity");
	if (!msg)
		goto done;

	if (!(reply = dbus_connection_send_with_reply_and_block(conn, msg, -1, &err)))
		goto done;

	if (dbus_error_is_set(&err))
		goto done;

	if (!dbus_message_get_args(reply, &err, DBUS_TYPE_UINT32, &res, DBUS_TYPE_INVALID))
		goto done;

done:
	dbus_error_free(&err);
	if (msg)
		dbus_message_unref(msg);
	if (reply)
		dbus_message_unref(reply);

	return res;
}
