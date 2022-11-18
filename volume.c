/* Volume stats for statusd
   Copyright (c) 2022 bellrise */

#define _XOPEN_SOURCE
#include <string.h>
#include <stdio.h>


void stat_volume(char *buf, int len)
{
	char local[8];
	FILE *f;

	f = popen("pamixer --get-mute", "r");
	if (!f)
		return;

	fgets(local, 8, f);
	snprintf(buf, len, !strncmp(local, "true", 4) ? "" : "");
	pclose(f);
}
