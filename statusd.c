/* statusd - a lightweight status daemon
   Copyright (c) 2022 bellrise */

#include <X11/Xlib.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <time.h>

#include "config.h"

static void statfmt(const char *fmt, char *res, int len);

void stat_net(char *buf, int len);
void stat_power(char *buf, int len);
void stat_volume(char *buf, int len);


int main()
{
	char res[MAXSIZ];
	Display *dpy;
	Window win;

	dpy = XOpenDisplay(NULL);
	if (!dpy) {
		puts("failed to open display");
		return 1;
	}

	win = DefaultRootWindow(dpy);

	while (1) {
		statfmt(FORMAT, res, MAXSIZ);
		XStoreName(dpy, win, res);
		XSync(dpy, win);
		sleep(UPDATE_EVERY);
	}

	XCloseDisplay(dpy);
	return 0;
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
				stat_power(tmp, 8);
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
				stat_net(tmp, 8);
				w += snprintf(res + w, len - w, "%s", tmp);
				break;
			case 'v':
				stat_volume(tmp, 8);
				w += snprintf(res + w, len - w, "%s", tmp);
				break;
			default:
				w += snprintf(res + w, len - w, "%%%c", fmt[i + 1]);
				break;
		}

		i += 2;
	}
}
