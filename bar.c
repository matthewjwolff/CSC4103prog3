#include <sys/ioctl.h>
#include <stdio.h>
#include <unistd.h>

/**
 * Prints a progress bar with percent filled.
 * Does not print a new line. Will print over itself if called repeatedly.
 * If you want the progress bar to work as intended, don't use stdout while using the progress bar
 */
void bar(float percent) {
	struct winsize w;
	// http://stackoverflow.com/questions/1022957/getting-terminal-width-in-c
	// probably wouldn't have figured out this one without that
	ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
	// i'm ok with integer truncation. and take off 2 characters for the [ and ]
	int num_markers = (w.ws_col - 2) * percent;
	printf("\r[");
	int i=0;
	for(i; i<num_markers; i++)
		printf("#");
	for(i; i<w.ws_col-2; i++)
		printf("~");
	printf("]");
}
