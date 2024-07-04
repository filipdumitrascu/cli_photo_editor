/* Copyright 2023 < 323CA Dumitrascu Filip Teodor > */
#include <stdio.h>
#include <string.h>

#include "commands.h"

int main(void)
{
	int exit = 0;
	char *command, chr;
	format img;
	img.loaded = 0;

	/**
	 * While EXIT command was not given as input, reads the first
	 * word of each line in order to call the specific function.
	 */
	while (!exit) {
		scanf("%ms", &command);

		if (!strcmp(command, "LOAD")) {
			load(&img);

		} else if (!strcmp(command, "SELECT")) {
			select(&img);

		} else if (!strcmp(command, "HISTOGRAM")) {
			histogram(&img);

		} else if (!strcmp(command, "EQUALIZE")) {
			equalize(&img);

		} else if (!strcmp(command, "ROTATE")) {
			rotate(&img);

		} else if (!strcmp(command, "CROP")) {
			crop(&img);

		} else if (!strcmp(command, "APPLY")) {
			apply(&img);

		} else if (!strcmp(command, "SAVE")) {
			save(&img);

		} else if (!strcmp(command, "EXIT")) {
			quit(&img);
			exit = 1;

		} else {
			printf("Invalid command\n");
			/**
			 * Reads the remaining characters until encountering '\n'
			 */
			scanf("%c", &chr);
			while (chr != '\n')
				scanf("%c", &chr);
		}

		free(command);
	}

	return 0;
}
