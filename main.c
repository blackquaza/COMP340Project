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
	int x = 0, y = 0;

	while (input = getch()) {

		switch (input) {

			case 4: // Left arrow
				x--;
				move(y, x);
				break;
			case 5: // Right arrow
				x++;
				move(y, x);
				break;
			case 3: // Up arrow
				y--;
				move(y, x);
				break;
			case 2: // Down arrow
				y++;
				move(y, x);
				break;
			default:
				printw("%c", input);
				getyx(stdscr, y, x);

		}

		refresh();

	}
	
	atexit(gracefulClose);

}
