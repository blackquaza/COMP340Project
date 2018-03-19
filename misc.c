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
