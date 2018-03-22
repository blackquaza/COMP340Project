#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
//#include <sys/ioctl.h>

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


int main () {

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
	char *output = calloc(maxy * maxx, sizeof(char));
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
				if (output[y * maxx + x] == '\0' /*|| 
				    output[y * maxx + x] == '\n'*/) x--;
				move(y, x);
				break;
			case 3: // Up arrow
				y--;
				if (y < 0) y++; // If less than zero, move back.
				while (output[y * maxx + x] == '\0' /*||
				       output[y * maxx + x] == '\n'*/) {
					if (x == 0) break;
					x--;
				}
				move(y, x);
				break;
			case 2: // Down arrow
				y++;
				// Back up until we find the line again.
				while (output[y * maxx + x] == '\0' /*||
				       output[y * maxx + x] == '\n'*/) {
					x--;
					if (x == -1) {
						// Go back up if there's nothing below.
						x = maxx - 1;
						y--;
					}
				}
				move(y, x);
				break;
			case 27: // Escape key
				// This is used to exit the loop. It's acutally checked
				// below, because a break here will just break the switch.
				return;
				break;
			case 10: // Enter key
				for (int i = y * maxx + x + 1; i < (y + 1) * maxx; i++) {
					output[i] = '\0';
				}
			default:
				printw("%c", input);
				output[y * maxx + x] = input;
				// This makes sure that there's always a newline at
				// the end of a given line, since it can't go straight
				// to NULL
				if (output[y * maxx + x + 1] == '\0')
					output[y * maxx + x + 1] = '\n';
				getyx(stdscr, y, x);

		}

		//if (input == 27) break;

		refresh();

	}
	
}
