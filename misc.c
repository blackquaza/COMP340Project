// This file contains miscellanious commands used elsewhere

void clearTerm(int *LINES, int *COLUMNS) {

	system("clear");

	// Looked up the following for terminal size information:
	// https://stackoverflow.com/questions/263890/how-do-i-find-the-width-height-of-a-terminal-window
	// https://stackoverflow.com/questions/1022957/getting-terminal-width-in-c



	/* PRE-NCURSES
	struct winsize window;
	ioctl(0, TIOCGWINSZ, &window);
	LINES = window.ws_row;
	COLUMNS = window.ws_col;
	
	for (int i = 0; i < LINES; i++) {

		for (int j = 0; j < COLUMNS; j++) {

			system("echo ESC[");
	*/




}

// Cleans up the terminal on exit. Using this function in case the function is
// terminated externally.
// Referred to https://linux.die.net/man/3/atexit
int gracefulClose() {

	//system("clear");
	endwin();

}

// Prints off the error message when there are too many arguments.
void wrongUse() {

	printf("Incorrect use of ./dim. Use ./dim --help for usage informaion.\n");

}

// Prints off the messaging when using --help.
void helpSpam() {

	printf("\n\t\tDakota's VIM\n\n\tUsage:\n\n\t./dim --help \n\tShows this help screen.");
	printf("\n\n\t./dim FILENAME\n\tOpens the editor with the specified file. If it ");
	printf("doesn't exist, the file will be created.\n\n");

}

// Reads the contents of the file descriptor into the **output array.
void readFile(int fd, char **output) {

	//
	int numlines = 0, tempsize = 50;
	output = calloc(tempsize, sizeof(char *));

	while (1) {

		// getline will automatically size the buffer for us. That's nice.
		if (getline(output[numlines], 0, fd) == -1) {

			// If getline failed to read anything, call it quits.

			// First, remove the excess space at the ends of the array.
			output = realloc(output, (numlines+1) * sizeof(char *));
			output[numlines][0] = '\n';
			break;


		}

		numlines++;

		if (numlines >= tempsize) {

			tempsize += 50;
			output = realloc(output, tempsize * sizeof(char *));

			// realloc doesn't get the new values to NULL. Do this now
			for (int i = numlines; i < tempsize; i++) {
				output[i] = NULL;
			}

		}

	}

}
