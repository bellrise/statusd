/* Power info for statusd
   Copyright (c) 2022 bellrise */

#include <string.h>
#include <stdio.h>


void stat_power(char *buf, int len)
{
	unsigned percent, index;
	const char *icon;
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

	icon = icons[percent / 20];

	/* Check if charging, possibly make it use inotify() for instant
	   updates for user experience? */
	f = fopen("/sys/class/power_supply/BATT/status", "r");
	if (!f) {
		snprintf(buf, len, "no bat");
		return;
	}

	char line[32];
	fgets(line, 32, f);
	if (!strncmp(line, "Charging", 8))
		icon = "";

	fclose(f);
	snprintf(buf, len, "%s %d%%", icon, percent);
}
