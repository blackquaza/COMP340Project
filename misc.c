// This file contains miscellanious commands used elsewhere

// Cleans up the terminal on exit. Using this function in case the function is
// terminated externally.
// Referred to https://linux.die.net/man/3/atexit
int gracefulClose() {

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
	printf("doesn't exist, the file will be created.\n\n\tControls:\n\n\tArrow Keys:");
	printf("\n\tMove the cursor around.\n\n\tEsc:\n\tQuit the application (without ");
	printf("saving).\n\n\tF7:\n\tSave the text file.\n\n");

}

// Reads the contents of the file descriptor into the **output array.
// returns the number of lines.
int readFile(FILE *fPtr, char **output) {

	// Everybody loves integers.
	int numlines = 0, tempsize = 50, zero = 50, read = 0;
	
	while (1) {

		output[numlines] = calloc(50, sizeof(char));

		// getline will automatically size the buffer for us. That's nice.
		if ((read = getline(&(output[numlines]), &zero, fPtr)) == -1) {
			// If getline failed to read anything, call it quits.

			// Remove the excess space at the ends of the array.
			output = realloc(output, (numlines+1) * sizeof(char *));
			output[numlines][0] = '\n';

			// Show the opened file on the screen.
			refresh();
			break;


		}

		// Got a line. Put it on the screen.
		printw("%s", output[numlines]);
		numlines++;

		// This tests to see if the line index needs to be increased. Rarely called.
		if (numlines >= tempsize) {

			tempsize += 50;
			output = realloc(output, tempsize * sizeof(char *));

			// realloc doesn't set the new values to NULL. Do this now.
			for (int i = numlines; i < tempsize; i++) {
				output[i] = NULL;
			}

		}

	}

	return numlines + 1;

}

// Saves the file. This was shorter than expected.
int saveFile(FILE *fPtr, char **output, int numlines) {

	fseek(fPtr, 0, SEEK_SET);

	for (int i = 0; i < numlines; i++) {

		fwrite(output[i], sizeof(char), strlen(output[i]), fPtr);

	}

}
