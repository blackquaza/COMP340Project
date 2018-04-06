#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/stat.h>
#include <fcntl.h>
//#include <sys/ioctl.h>
#include <errno.h>

#define _POSIX_C_SOURCE
#define _GNU_SOURCE

// Ended up just using the NCURSES library for terminal control and output. The
// primary purpose of the assignment is input and the file system. The input is
// handled by NCURSES but is checked by the program, and file I/O is done manually
// as well.
// Referred to the following about NCURSES:
// https://www.gnu.org/software/ncurses
// http://tldp.org/HOWTO/NCURSES-Programming-HOWTO/intro.html (plus later pages)
// http://www.cs.ukzn.ac.za/~hughm/os/notes/ncurses.html

#include <ncurses.h>

// Includes various miscellanious commands.
#include "misc.c"


int main (int argc, const char *argv[]) {

	atexit(gracefulClose);

	// Looked up the following for specific character outputs:
	//
	// https://stackoverflow.com/questions/21829705/how-can-output-be-directed-to-specific-coordinates-in-the-linux-terminal
	// http://www.isthe.com/chongo/tech/comp/ansi_escapes.html
	// https://linux.die.net/man/3/system

	//system("clear;echo test;echo test");
	
	//int LINES, COLUMNS;
	//clearTerm(LINES, COLUMNS);

	// Looked up the following regarding signals.
	//
	// https://stackoverflow.com/questions/17766550/ctrl-c-interrupt-event-handling-in-linux
	// https://linux.die.net/man/3/atexit
	
	// Not going to use getopt here, since the only options are --help (or -h) or the
	// file to open.
	
	// Too many arguments. Error out.
	if (argc > 2) {

		wrongUse();
		return -1;

	}

	char *filepath;

	// Should never happen, but just in case we'll plan for it.
	if (argc == 0) return -1;

	// No argument provided. Set the default file name as "newFile".
	if (argc == 1) {

		filepath = malloc(8 * sizeof(char));
		strcpy(filepath, "newFile\0");

	} else {

		// If the argument is the help request, send the message and exit.
		if (strcmp(argv[1], "--help") == 0 || strcmp(argv[1], "-h") == 0) {

			helpSpam();
			return 0;

		}

		filepath = argv[1];

	}

	// Various NCURSES initialization commands.
	initscr(); // Initializes a new screen for the terminal.
	cbreak(); // Disables line buffering, giving the character immediately
		  // to the program. Sends control characters together (ex. Ctrl C).
	keypad(stdscr, TRUE); // Enables the use of the arrow and Function keys.
	noecho(); // Prevents the keyboard input from adding a character. This is done
		  // manually later to make sure control characters are not shown.
	
	char input;
	int x = 0, y = 0, maxx, maxy;
	getmaxyx(stdscr, maxy, maxx);

	// Although the terminal will show the output, the actual changes are going to be
	// made in this chunk of memory here.
	//char *output = calloc(maxy * maxx, sizeof(char));
	int numlines = 1;
	const int MAXLEN = 50;
	char **output;
	//char **output = malloc(sizeof(char *));
	//output[0] = calloc(MAXLEN, sizeof(char));
	//output[0][0] = '\n';

	//FILE *fPtr = fopen(filepath, "a+");
	
	int fd = open(filepath, O_RDWR | O_CREAT, S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);
	FILE *fPtr = fdopen(fd, "w+");

	// Take the contents of the file and stick them in memory.
	// Defined in misc.c
	numlines = readFile(fPtr, output);

	//printw("t3:%s\n", numlines);
	//refresh();
	// Print the contents to the terminal.
	//for (int i = 0; i < numlines; i++) printw("%s", output[i]);
	//refresh();	

	//printw("%i,%i,%i,%i", y, x, maxy, maxx);

	while (input = getch()) {

		switch (input) {

			case 4: // Left arrow
				x--;
				if (x < 0) x++; // If less than zero, move back.
				move(y, x);
				break;
			case 5: // Right arrow
				x++;
				// If at the end of the line, move back.
				if (output[y][x] == '\0') x--;
				move(y, x);
				break;
			case 3: // Up arrow
				y--;
				if (y < 0) y++; // If less than zero, move back.
				while (output[y][x] == '\0') {
					if (x == 0) break;
					x--;
				}
				move(y, x);
				break;
			case 2: // Down arrow
				y++;
				// Can't go farther.
				if (y == numlines) {
					y--;
					while (1) {

						if (output[y][x] == '\n') break;
						x++;
					}
				} else {
					// Back up until we find the line again.
					while (output[y][x] == '\0') {
						x--;
						if (x == -1) {
							// Go back up if there's nothing below.
							x = MAXLEN - 1;
							y--;
						}
					}
				}
				move(y, x);
				break;
			case 27: // Escape key
				// This is used to exit the loop.
				return;
				break;
			case 7: // Backspace key
				if (x != 0) {
					x--;
					int i = 0;
					// Shove everything over to the left one space.
					while (1) {
						output[y][x + i] = output[y][x + i + 1];
						if (output[y][x + i] == '\0') break;
						i++;
					}
					move(y, x);
					for (int j = 0; j <= i; j++) {
						if (output[y][x + j] != '\0') {
							printw("%c", output[y][x + j]);
						} else {
							printw(" ");
						}
					}
					move(y, x);
				}
				break;
			case 15: // F7 key
				break;
			case 10: // Enter key
				/*for (int i = y * maxx + x + 1; i < (y + 1) * maxx; i++) {
					output[i] = '\0';
				}*/
				if (y == numlines - 1) {
					numlines++;
					output = realloc(output, numlines * sizeof(char *));
					output[y+1] = calloc(MAXLEN, sizeof(char));
					output[y+1][0] = '\n';
				}
			default:	
				// These values came from testing the values of each keystroke
				// on my laptop computer.
				if ((input < 32 && input != 10) || input > 126) break;
				int i = 0;
				// Shove everything over to the right one space.
				if (input != 10) {
					while (output[y][x + i] != '\0') i++;
					for (int j = i; j > 0; j--) {
						output[y][x + j] = output[y][x + j - 1];
					}
					output[y][x] = input;
				}
				printw("%c", input);
				getyx(stdscr, y, x);
				for (int j = 0; j < i; j++) {
					if (output[y][x + j] != '\0') {
						printw("%c", output[y][x + j]);
					} else {
						printw(" ");
					}
				}
				move(y, x);
				// This makes sure that there's always a newline at
				// the end of a given line, since it can't go straight
				// to NULL

		}

		refresh();

	}
	
}
