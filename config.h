/* The FORMAT string represents which information should be placed returned.
   Here is a list of all possible values:

	%B - bluetooth status
	%b - battery icon & percent
	%d - day in year, ex. '16 Nov'
	%t - current time, in HH:MM format
	%n - network icon
	%v - volume muted/not-muted icon */
#define FORMAT "%B %v %n %b | %d %t"

/* The maximum size of the output buffer. The actual string may only be
   MAXSIZ - 1 chars long. */
#define MAXSIZ 256

/* Update every n seconds. */
#define UPDATE_EVERY 3
