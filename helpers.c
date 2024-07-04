/* Copyright 2023 < 323CA Dumitrascu Filip Teodor > */
#include <stdio.h>
#include <string.h>
#include <math.h>

#include "helpers.h"

void read_ascii(format *img, FILE *fin)
{
	/**
	 * Reads from the file and stores the values in the grayscale/rgb matrices
	 */
	for (int i = 0; i < img->height; i++)
		for (int j = 0; j < img->width; j++)
			if (!strcmp(img->word, "P2"))
				fscanf(fin, "%lf", &img->gray[i][j]);
			else
				fscanf(fin, "%lf %lf %lf", &img->red[i][j], &img->green[i][j],
					   &img->blue[i][j]);
}

void read_binary(format *img, FILE *fin, char *filename)
{
	FILE *bin = fopen(filename, "rb");
	if (!bin) {
		printf("Failed to load %s\n", filename);
		return;
	}

	/**
	 * Sets the read at the start of the matrix values
	 */
	fseek(bin, ftell(fin), SEEK_SET);

	/**
	 * Reads the binary values
	 */
	for (int i = 0; i < img->height; i++)
		for (int j = 0; j < img->width; j++) {
			if (!strcmp(img->word, "P5")) {
				_uc temp;
				fread(&temp, sizeof(_uc), 1, bin);
				img->gray[i][j] = (double)temp;

			} else {
				_uc temp1, temp2, temp3;
				fread(&temp1, sizeof(_uc), 1, bin);
				fread(&temp2, sizeof(_uc), 1, bin);
				fread(&temp3, sizeof(_uc), 1, bin);

				img->red[i][j] = (double)temp1;
				img->green[i][j] = (double)temp2;
				img->blue[i][j] = (double)temp3;
			}
		}

	fclose(bin);
}

void all(format *img)
{
	char chr;

	/**
	 * Case "SELECT ALL..." is treated
	 */
	scanf("%c", &chr);
	if (chr == ' ')
		if (line_not_finished())
			return;

	img->x1 = 0;
	img->y1 = 0;
	img->x2 = img->width;
	img->y2 = img->height;
	printf("Selected ALL\n");
}

int is_integer(char *string)
{
	int i = 0;

	/**
	 * Checks if the string is only a minus
	 */
	if ((string[i] == '-' && string[i + 1] == '\0'))
		return 0;

	/**
	 * Jumps over the minus
	 */
	if (string[i] == '-')
		i++;

	/**
	 * If it finds a char that is not an int, the string is not an int
	 */
	for (; string[i] != '\0'; i++)
		if (string[i] < '0' || string[i] > '9')
			return 0;

	return 1;
}

int invalid_param(char *string)
{
	char chr;

	/**
	 * If the given string is not an integer, returns from the command
	 */
	if (!is_integer(string)) {
		printf("Invalid command\n");

		scanf("%c", &chr);
		while (chr != '\n')
			scanf("%c", &chr);

		return 1;
	}

	return 0;
}

void right(format *img, double ***matrix)
{
	double **temp = NULL;

	if (img->x2 - img->x1 == img->width && img->y2 - img->y1 == img->height) {
		/**
		 * The case where the whole image has to be rotated is treated
		 * so allocates a temporary matrix with the rotated dimensions
		 */
		alloc_matrix(&temp, img->width, img->height);

		/**
		 * Assigns the rotated values directly to a new matrix
		*/
		for (int i = 0; i < img->height; i++)
			for (int j = 0; j < img->width; j++)
				temp[j][img->height - 1 - i] = (*matrix)[i][j];

		/**
		 * Frees the original matrix from the memory and
		 * declare the rotated one as the new one
		 */
		free_matrix(matrix, img->height);
		*matrix = temp;

	} else {
		/**
		 * Only the selection has to be rotated so allocates a temporary
		 * matrix with the dimensions formed by the selection
		 */
		alloc_matrix(&temp, img->x2 - img->x1, img->y2 - img->y1);

		/**
		 * Assigns the rotated values directly to a new matrix
		*/
		for (int i = img->y1; i < img->y2; i++)
			for (int j = img->x1; j < img->x2; j++)
				temp[j - img->x1][img->y2 - 1 - i] = (*matrix)[i][j];

		/**
		 * Puts the rotated values on the selected part in the original matrix
		 */
		for (int i = img->y1; i < img->y2; i++)
			for (int j = img->x1; j < img->x2; j++)
				(*matrix)[i][j] = temp[i - img->y1][j - img->x1];

		/**
		 * Frees the original matrix from the memory
		 */
		free_matrix(&temp, img->x2 - img->x1);
	}
}

void down(format *img, double ***matrix)
{
	double **temp = NULL;

	if (img->x2 - img->x1 == img->width && img->y2 - img->y1 == img->height) {
		/**
		 * The case where the whole image has to be rotated is treated
		 * so allocates a temporary matrix with the rotated dimensions
		 */
		alloc_matrix(&temp, img->height, img->width);

		/**
		 * Assigns the rotated values directly to a new matrix
		*/
		for (int i = 0; i < img->height; i++)
			for (int j = 0; j < img->width; j++)
				temp[img->height - 1 - i][img->width - 1 - j] = (*matrix)[i][j];

		/**
		 * Frees the original matrix from the memory and
		 * declare the rotated one as the new one
		 */
		free_matrix(matrix, img->height);
		*matrix = temp;

	} else {
		/**
		 * Only the selection has to be rotated so allocates a temporary
		 * matrix with the dimensions formed by the selection
		 */
		alloc_matrix(&temp, img->y2 - img->y1, img->x2 - img->x1);

		/**
		 * Assigns the rotated values directly to a new matrix
		*/
		for (int i = img->y1; i < img->y2; i++)
			for (int j = img->x1; j < img->x2; j++)
				temp[img->y2 - 1 - i][img->x2 - 1 - j] = (*matrix)[i][j];

		/**
		 * Puts the rotated values on the selected part in the original matrix
		 */
		for (int i = img->y1; i < img->y2; i++)
			for (int j = img->x1; j < img->x2; j++)
				(*matrix)[i][j] = temp[i - img->y1][j - img->x1];

		/**
		 * Frees the original matrix from the memory
		 */
		free_matrix(&temp, img->y2 - img->y1);
	}
}

void left(format *img, double ***matrix)
{
	double **temp = NULL;

	if (img->x2 - img->x1 == img->width && img->y2 - img->y1 == img->height) {
		/**
		 * The case where the whole image has to be rotated is treated
		 * so allocates a temporary matrix with the rotated dimensions
		 */
		alloc_matrix(&temp, img->width, img->height);

		/**
		 * Assigns the rotated values directly to a new matrix
		*/
		for (int i = 0; i < img->height; i++)
			for (int j = 0; j < img->width; j++)
				temp[img->width - 1 - j][i] = (*matrix)[i][j];

		/**
		 * Frees the original matrix from the memory and
		 * declare the rotated one as the new one
		 */
		free_matrix(matrix, img->height);
		*matrix = temp;

	} else {
		/**
		 * Only the selection has to be rotated so allocates a temporary
		 * matrix with the dimensions formed by the selection
		 */
		alloc_matrix(&temp, img->x2 - img->x1, img->y2 - img->y1);

		/**
		 * Assigns the rotated values directly to a new matrix
		*/
		for (int i = img->y1; i < img->y2; i++)
			for (int j = img->x1; j < img->x2; j++)
				temp[img->x2 - 1 - j][i - img->y1] = (*matrix)[i][j];

		/**
		 * Puts the rotated values on the selected part in the original matrix
		 */
		for (int i = img->y1; i < img->y2; i++)
			for (int j = img->x1; j < img->x2; j++)
				(*matrix)[i][j] = temp[i - img->y1][j - img->x1];

		/**
		 * Frees the original matrix from the memory
		 */
		free_matrix(&temp, img->x2 - img->x1);
	}
}

void filter(format *img, double kernel[KERNEL][KERNEL], int *ok)
{
	/**
	 * Only for rgb matrices
	 */
	if (!strcmp(img->word, "P2") || !strcmp(img->word, "P5")) {
		printf("Easy, Charlie Chaplin\n");
		*ok = 0;
		return;
	}

	/**
	 * Allocates temporary matrices to memorize the filtered value
	 */
	double **temp_red = NULL, **temp_green = NULL, **temp_blue = NULL;
	alloc_matrix(&temp_red, img->y2 - img->y1, img->x2 - img->x1);
	alloc_matrix(&temp_green, img->y2 - img->y1, img->x2 - img->x1);
	alloc_matrix(&temp_blue, img->y2 - img->y1, img->x2 - img->x1);

	/**
	 * Calculates the filtered value in a temp matrix
	 */
	for (int i = img->y1; i < img->y2; i++)
		for (int j = img->x1; j < img->x2; j++) {
			double s1 = 0, s2 = 0, s3 = 0;

			if (i != 0 && j != 0 && i != img->height - 1 &&
				j != img->width - 1) {
				for (int r = 0; r < KERNEL; r++)
					for (int c = 0; c < KERNEL; c++) {
						s1 += img->red[i + r - 1][j + c - 1] * kernel[r][c];
						s2 += img->green[i + r - 1][j + c - 1] * kernel[r][c];
						s3 += img->blue[i + r - 1][j + c - 1] * kernel[r][c];
					}

				temp_red[i - img->y1][j - img->x1] = clamp(s1);
				temp_green[i - img->y1][j - img->x1] = clamp(s2);
				temp_blue[i - img->y1][j - img->x1] = clamp(s3);
			}
		}

	/**
	 * Only if the pixel is not on the edge assigns the value
	 */
	for (int i = img->y1; i < img->y2; i++)
		for (int j = img->x1; j < img->x2; j++)
			if (i != 0 && j != 0)
				if (i != img->height - 1 && j != img->width - 1) {
					img->red[i][j] = temp_red[i - img->y1][j - img->x1];
					img->green[i][j] = temp_green[i - img->y1][j - img->x1];
					img->blue[i][j] = temp_blue[i - img->y1][j - img->x1];
				}

	/**
	 * Frees the temporary matrices
	 */
	free_matrix(&temp_red, img->y2 - img->y1);
	free_matrix(&temp_green, img->y2 - img->y1);
	free_matrix(&temp_blue, img->y2 - img->y1);
}

void write_ascii(format *img, FILE *fout)
{
	/**
	 * Consumes the word 'ascii' from stdin
	 */
	char *word;
	scanf("%ms", &word);
	free(word);

	/**
	 * Stores in the file the specific values for image
	 * type, width, height and max pixel value
	 */
	if (!strcmp(img->word, "P2") || !strcmp(img->word, "P5"))
		fprintf(fout, "P2\n");
	else
		fprintf(fout, "P3\n");

	fprintf(fout, "%d %d\n", img->width, img->height);
	fprintf(fout, "%d\n", img->max);

	/**
	 * Stores the matrices ascii in the file
	 */
	for (int i = 0; i < img->height; i++) {
		for (int j = 0; j < img->width; j++)
			if (!strcmp(img->word, "P2") || !strcmp(img->word, "P5")) {
				fprintf(fout, "%d ", (int)round(img->gray[i][j]));

			} else {
				fprintf(fout, "%d ", (int)round(img->red[i][j]));
				fprintf(fout, "%d ", (int)round(img->green[i][j]));
				fprintf(fout, "%d ", (int)round(img->blue[i][j]));
			}
		fprintf(fout, "\n");
	}
	fclose(fout);
}

void write_binary(format *img, FILE *fout, char *filename)
{
	/**
	 * Stores in the file the specific values for image
	 * type, width, height and max pixel value
	 */
	if (!strcmp(img->word, "P2") || !strcmp(img->word, "P5"))
		fprintf(fout, "P5\n");
	else
		fprintf(fout, "P6\n");

	fprintf(fout, "%d %d\n", img->width, img->height);
	fprintf(fout, "%d\n", img->max);

	fclose(fout);

	FILE *bout = fopen(filename, "ab");
	if (!bout) {
		printf("Failed to save %s", filename);
		return;
	}

	/**
	 * Stores the matrices ascii in the file
	 */
	for (int i = 0; i < img->height; i++)
		for (int j = 0; j < img->width; j++)
			if (!strcmp(img->word, "P2") || !strcmp(img->word, "P5")) {
				_uc temp = (_uc)round(img->gray[i][j]);
				fwrite(&temp, sizeof(_uc), 1, bout);

			} else {
				_uc temp1 = (_uc)round(img->red[i][j]);
				_uc temp2 = (_uc)round(img->green[i][j]);
				_uc temp3 = (_uc)round(img->blue[i][j]);
				fwrite(&temp1, sizeof(_uc), 1, bout);
				fwrite(&temp2, sizeof(_uc), 1, bout);
				fwrite(&temp3, sizeof(_uc), 1, bout);
			}

	fclose(bout);
}
