/* Copyright 2023 < 323CA Dumitrascu Filip Teodor > */
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "utils.h"

void alloc_matrix(double ***matrix, int rows, int columns)
{
	/**
	 * Allocates memory for the array of pointers
	 */
	(*matrix) = malloc(rows * sizeof(double *));
	DIE(!(*matrix), "Malloc for the array of pointers failed\n");

	/**
	 * For each pointer, allocates memory for an array with columns elements
	 */
	for (int i = 0; i < rows; i++) {
		(*matrix)[i] = malloc(columns * sizeof(double));
		/**
		 * Knowing that DIE does not deallocate memory checks the result of the
		 * malloc with an if and deallocates memory before calling DIE
		 * (from the defensive programming point of view)
		 */
		if (!(*matrix)[i]) {
			for (int j = 0; j < i; j++)
				free((*matrix)[j]);
			free(*matrix);
			DIE(1, "Malloc for a row failed\n");
		}
	}
}

void free_matrix(double ***matrix, int rows)
{
	/**
	 * Frees all the rows of the matrix and the array of pointers
	 */
	for (int i = 0; i < rows; i++) {
		free((*matrix)[i]);
	}
	free(*matrix);
}

void resize_matrix(format *img, double ***matrix)
{
	int rows = img->y2 - img->y1;
	int columns = img->x2 - img->x1;

	/**
	 * Allocates a new matrix with the sizes according to the selection
	 */
	double **new = NULL;
	alloc_matrix(&new, rows, columns);

	/**
	 * Assigns the values in the new matrix according to the selection
	 */
	for (int i = 0; i < rows; i++) {
		for (int j = 0; j < columns; j++) {
			new[i][j] = (*matrix)[i + img->y1][j + img->x1];
		}
	}

	/**
	 * Frees the original matrix from the memory and
	 * declares the resized one as the new one
	 */
	free_matrix(matrix, img->height);
	*matrix = new;
}

void skip_comment(FILE *fin)
{
	char chr;
	fscanf(fin, "%c", &chr);

	/**
	 * If the line starts with # it is consumed from the buffer otherwise
	 * move the reading back one character to be stored in the matrix
	 */
	if (!strcmp(&chr, "\n")) {
		fscanf(fin, "%c", &chr);
	}

	if (!strcmp(&chr,  "#")) {
		while (strcmp(&chr, "\n")) {
			fscanf(fin, "%c", &chr);
		}

	} else {
		fseek(fin, -1, SEEK_CUR);
	}
}

int no_image(format *img)
{
	char chr;

	if (!img->loaded) {
		printf("No image loaded\n");
		/**
		 * If there is no image, the rest of the chars are read from the line
		 */
		scanf("%c", &chr);

		while (chr != '\n') {
			scanf("%c", &chr);
		}

		return 1;
	}

	return 0;
}

int end_of_line(void)
{
	char chr;

	/**
	 * If the next char is the end of the line this is an invalid command
	 */
	scanf("%c", &chr);

	if (chr == '\n') {
		printf("Invalid command\n");
		return 1;
	}

	return 0;
}

int line_not_finished(void)
{
	char chr;

	/**
	 * If the line is not finished this is an ivalid command
	 */
	scanf("%c", &chr);

	if (chr != '\n') {
		printf("Invalid command\n");

		while (chr != '\n') {
			scanf("%c", &chr);
		}
		return 1;
	}

	return 0;
}

void swap(int *a, int *b)
{
	/**
	 * Swaps the values between two integers
	 */
	int aux = *a;
	*a = *b;
	*b = aux;
}

double clamp(double sum)
{
	/**
	 * Fits the sum between 0 and 255
	 */
	if (sum < 0) {
		sum = 0;

	} else if (sum > MAX_ASCII) {
		sum = MAX_ASCII;
	}

	return sum;
}
