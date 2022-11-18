/* Power info for statusd
   Copyright (c) 2022 bellrise */

#include <stdio.h>


void stat_power(char *buf, int len)
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
