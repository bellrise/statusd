/* statusd - a lightweight status daemon
   Copyright (c) 2022 bellrise */

#include <sys/socket.h>
#include <sys/ioctl.h>
#include <sys/time.h>
#include <dbus/dbus.h>
#include <string.h>
#include <unistd.h>
#include <stdio.h>
#include <time.h>
#include <wchar.h>

#include "nm-def.h"


/* The FORMAT string represents which information should be placed returned.
   Here is a list of all possible values:

   	 %b - battery icon & percent
 	 %d - day in year, ex. '16 Nov'
     %t - current time, in HH:MM format
	 %n - network icon */
#define FORMAT "%b %n | %d %t"

/* The maximum size of the output buffer. The actual string may only be
   MAXSIZ - 1 chars long. */
#define MAXSIZ 256


static int netstat_ask_networkmanager();
static void statfmt(const char *fmt, char *res, int len);
static void netstat(char *buf, int len);
static void battstat(char *buf, int len);


int main()
{
	char res[MAXSIZ];

	statfmt(FORMAT, res, MAXSIZ);
	printf("%s\n", res);
}

void statfmt(const char *fmt, char *res, int len)
{
	time_t since_epoch;
	struct tm *tm_now;
	int i, w, strl;
	char tmp[8];

	i = 0;
	w = 0;
	strl = strlen(fmt);
	memset(tmp, 0, 8);
	memset(res, 0, len);

	while (i < strl) {
		if (fmt[i] != '%') {
			res[w++] = fmt[i++];
			continue;
		}

		switch (fmt[i + 1]) {
			case 'b':
				battstat(tmp, 8);
				w += snprintf(res + w, len - w, "%s", tmp);
				break;
				break;
			case 'd':
				time(&since_epoch);
				tm_now = localtime(&since_epoch);
				w += strftime(res + w, len - w, "%d %b", tm_now);
				break;
			case 't':
				time(&since_epoch);
				tm_now = localtime(&since_epoch);
				w += snprintf(res + w, len - w, "%02d:%02d", tm_now->tm_hour, tm_now->tm_min);
				break;
			case 'n':
				netstat(tmp, 8);
				w += snprintf(res + w, len - w, "%s", tmp);
				break;
			default:
				w += snprintf(res + w, len - w, "%%%c", fmt[i - 1]);
				break;
		}

		i += 2;
	}
}

static int netstat_ask_networkmanager()
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

static void netstat(char *buf, int len)
{
	unsigned stat = netstat_ask_networkmanager();

	if (stat <= NM_CONNECTIVITY_PORTAL) {
		/* Unknown, none or portal connectivity. */
		snprintf(buf, len, "");
		return;
	}

	/* Limited or full connection. */
	snprintf(buf, len, "");
}

static void battstat(char *buf, int len)
{
	unsigned percent, index;
	FILE *f;

	f = fopen("/sys/class/power_supply/BATT/capacity", "r");
	if (!f) {
		snprintf(buf, len, "no bat");
		return;
	}

	/* Set percent. */
	fscanf(f, "%u", &percent);
	fclose(f);

	/* Set icon. */
	const char *icons[] = {"", "", "", "", ""};
	index = percent / 20;
	if (index > 4)
		index = 4;

	snprintf(buf, len, "%s %d%%", icons[percent / 20], percent);
}
