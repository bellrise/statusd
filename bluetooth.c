/* Bluetooth stuff for statusd
   Copyright (c) 2022 bellrise */

#define _XOPEN_SOURCE
#include <string.h>
#include <stdio.h>
#include <ctype.h>


static void rstrip(char *str)
{
	int i = strlen(str);

	while (--i) {
		if (isspace(str[i]))
			str[i] = 0;
		else
			break;
	}
}


void stat_bluetooth(char *buf, int len)
{
	/* TODO: Have some decency and use this overly complicated
	   /org/bluez dbus API instead. */
	char local[16];
	FILE *f;

	f = popen("~/.local/bin/status-bluetooth", "r");
	if (!f)
		return;

	fgets(local, 16, f);
	rstrip(local);
	pclose(f);

	snprintf(buf, len, "%s", local);
}
