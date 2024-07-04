/* Copyright 2023 < 323CA Dumitrascu Filip Teodor > */
#include <stdio.h>
#include <string.h>
#include <math.h>

#include "commands.h"

void load(format *img)
{
	/**
	 * Replaces the img that is already loaded
	 */
	if (img->loaded) {
		if (!strcmp(img->word, "P2") || !strcmp(img->word, "P5")) {
			free_matrix(&img->gray, img->height);

		} else {
			free_matrix(&img->red, img->height);
			free_matrix(&img->green, img->height);
			free_matrix(&img->blue, img->height);
		}
	}
	img->loaded = 0;

	char *filename;
	scanf("%ms", &filename);

	/**
	 * Checks if the file was opened successfully
	 */
	FILE *fin = fopen(filename, "r");
	if (!fin) {
		printf("Failed to load %s\n", filename);
		free(filename);
		return;
	}

	skip_comment(fin);
	fscanf(fin, "%s", img->word);
	skip_comment(fin);
	fscanf(fin, "%d %d", &img->width, &img->height);
	skip_comment(fin);
	fscanf(fin, "%d", &img->max);
	skip_comment(fin);

	/**
	 * Allocates the memory for the image
	 */
	if (!strcmp(img->word, "P2") || !strcmp(img->word, "P5")) {
		alloc_matrix(&img->gray, img->height, img->width);

	} else {
		alloc_matrix(&img->red, img->height, img->width);
		alloc_matrix(&img->green, img->height, img->width);
		alloc_matrix(&img->blue, img->height, img->width);
	}

	/**
	 * Checks if the image is ascii or binary and loads it accordingly
	 */
	if ((!strcmp(img->word, "P2") || !strcmp(img->word, "P3")))
		read_ascii(img, fin);
	else
		read_binary(img, fin, filename);

	printf("Loaded %s\n", filename);

	img->loaded = 1;
	img->x1 = 0;
	img->y1 = 0;
	img->x2 = img->width;
	img->y2 = img->height;

	fclose(fin);
	free(filename);
}

void select(format *img)
{
	char s1[MAX_PARAM], s2[MAX_PARAM], s3[MAX_PARAM], s4[MAX_PARAM];

	/**
	 * Checks if there is any image loaded
	 */
	if (no_image(img))
		return;

	/**
	 * Case "SELECT\n" is treated
	 */
	if (end_of_line())
		return;

	scanf("%s", s1);
	if (!strcmp(s1, "ALL")) {
		all(img);
		return;
	}

	/**
	 * For every parameter checks if it's valid
	 * and if there's another one after it
	 */
	if (invalid_param(s1))
		return;

	if (end_of_line())
		return;

	scanf("%s", s2);
	if (invalid_param(s2))
		return;

	if (end_of_line())
		return;

	scanf("%s", s3);
	if (invalid_param(s3))
		return;

	if (end_of_line())
		return;

	scanf("%s", s4);
	if (invalid_param(s4))
		return;

	if (line_not_finished())
		return;

	/**
	 * Converts the strings to integers and select them in the correct order
	 */
	int x1 = atoi(s1), y1 = atoi(s2), x2 = atoi(s3), y2 = atoi(s4);

	if (x1 > x2)
		swap(&x1, &x2);

	if (y1 > y2)
		swap(&y1, &y2);

	if (x1 >= 0 && y1 >= 0 && x2 <= img->width && y2 <= img->height &&
		x1 != x2 && y1 != y2) {
		img->x1 = x1;
		img->y1 = y1;
		img->x2 = x2;
		img->y2 = y2;

		printf("Selected %d %d %d %d\n", x1, y1, x2, y2);
		return;
	}

	printf("Invalid set of coordinates\n");
}

void histogram(format *img)
{
	int x, y;

	/**
	 * Checks if there is any image loaded
	 */
	if (no_image(img))
		return;

	/**
	 * Case "HISTOGRAM\n" is treated
	 */
	if (end_of_line())
		return;

	scanf("%d", &x);

	/**
	 * Case "HISTOGRAM <x>\n" is treated
	 */
	if (end_of_line())
		return;

	scanf("%d", &y);

	/**
	 * Case "HISTOGRAM <x> <y>..." is treated
	 */
	if (line_not_finished())
		return;

	if (strcmp(img->word, "P2") && strcmp(img->word, "P5")) {
		printf("Black and white image needed\n");
		return;
	}

	if (log2(y) != (int)log2(y)) {
		printf("Invalid set of parameters\n");
		return;
	}

	int bins = MAX_POWER2 / y, max = 0;
	int *freq = calloc(MAX_POWER2, sizeof(int));
	DIE(!freq, "Calloc for array failed");

	/**
	 * Calculates the frequency of every bin
	 */
	for (int i = 0; i < img->height; i++)
		for (int j = 0; j < img->width; j++)
			freq[(int)img->gray[i][j] / bins]++;

	/**
	 * Calculates the maximum frequency
	 */
	for (int i = 0; i < y; i++)
		if (freq[i] > max)
			max = freq[i];

	/**
	 * Displays the histogram
	 */
	for (int val = 0; val < y; val++) {
		int stars = round(freq[val] * x / max);
		printf("%d\t|\t", stars);

		for (int i = 0; i < stars; i++)
			printf("*");
		printf("\n");
	}

	free(freq);
}

void equalize(format *img)
{
	/**
	 * Checks if there is any image loaded
	 */
	if (no_image(img))
		return;

	if (strcmp(img->word, "P2") && strcmp(img->word, "P5")) {
		printf("Black and white image needed\n");
		return;
	}

	int *freq = calloc(MAX_POWER2, sizeof(int));
	DIE(!freq, "Calloc for array failed\n");

	/**
	 * Calculates the frequency of every bin
	 */
	for (int i = 0; i < img->height; i++)
		for (int j = 0; j < img->width; j++)
			freq[(int)img->gray[i][j]]++;

	/**
	 * Calculates the equalized level
	 */
	double sum = 0.0;
	for (int i = 0; i < img->height; i++) {
		for (int j = 0; j < img->width; j++) {
			for (int l = 0; l <= img->gray[i][j]; l++)
				sum += freq[l];

			int area = img->height * img->width;
			double level = (MAX_ASCII * sum) / area;
			img->gray[i][j] = clamp(round(level));
			sum = 0.0;
		}
	}
	printf("Equalize done\n");

	free(freq);
}

void rotate(format *img)
{
	int angle, cpy;

	/**
	 * Checks if there is any image loaded
	 */
	if (no_image(img))
		return;

	scanf("%d", &angle);
	cpy = angle;

	if (!(img->x2 - img->x1 == img->width && img->y2 - img->y1 ==
		img->height) && !(img->x2 - img->x1 == img->y2 - img->y1)) {
		printf("The selection must be square\n");
		return;
	}

	/**
	 * If the angle is not a multiple of 90 or
	 * passes a full rotation it is invalid
	 */
	if (angle % (CIRCLE / 4) != 0 || abs(angle) > CIRCLE) {
		printf("Unsupported rotation angle\n");
		return;
	}

	if (angle == -CIRCLE || angle == CIRCLE)
		angle = 0;

	else if (angle < 0)
		angle += CIRCLE;

	/**
	 * Depending on the angle, rotates the image right, down or to the left
	 */
	if (angle == CIRCLE / 4) {
		if (!strcmp(img->word, "P2") || !strcmp(img->word, "P5")) {
			right(img, &img->gray);

		} else {
			right(img, &img->red);
			right(img, &img->green);
			right(img, &img->blue);
		}

	} else if (angle == CIRCLE / 2) {
		if (!strcmp(img->word, "P2") || !strcmp(img->word, "P5")) {
			down(img, &img->gray);

		} else {
			down(img, &img->red);
			down(img, &img->green);
			down(img, &img->blue);
		}

	} else if (angle) {
		if (!strcmp(img->word, "P2") || !strcmp(img->word, "P5")) {
			left(img, &img->gray);

		} else {
			left(img, &img->red);
			left(img, &img->green);
			left(img, &img->blue);
		}
	}

	/**
	 * For right and left rotations on the whole image
	 * the width and height values are swaped
	 */
	if (img->x2 - img->x1 == img->width && img->y2 - img->y1 == img->height &&
		angle % (CIRCLE / 2)) {
		swap(&img->width, &img->height);
		swap(&img->x2, &img->y2);
	}

	printf("Rotated %d\n", cpy);
}

void crop(format *img)
{
	/**
	 * Checks if there is any image loaded
	 */
	if (no_image(img))
		return;

	/**
	 * Resizes the matrices according to current selection
	 */
	if (!strcmp(img->word, "P2") || !strcmp(img->word, "P5")) {
		resize_matrix(img, &img->gray);

	} else {
		resize_matrix(img, &img->red);
		resize_matrix(img, &img->green);
		resize_matrix(img, &img->blue);
	}

	/**
	 * Changes the current selection
	 */
	img->width = img->x2 - img->x1;
	img->height = img->y2 - img->y1;
	img->x1 = 0;
	img->y1 = 0;
	img->x2 = img->width;
	img->y2 = img->height;

	printf("Image cropped\n");
}

void apply(format *img)
{
	int ok = 1;
	char *parameter;

	/**
	 * Checks if there is any image loaded
	 */
	if (no_image(img))
		return;

	/**
	 * Case "APPLY\n" is treated
	 */
	if (end_of_line())
		return;

	/**
	 * Declares EDGE, SHARPEN, BLUR and GAUSSIAN BLUR kernels
	 */
	double kernel1[KERNEL][KERNEL] = {
		{-1, -1, -1},
		{-1,  8, -1},
		{-1, -1, -1}};

	double kernel2[KERNEL][KERNEL] = {
		{0, -1,  0},
		{-1, 5, -1},
		{0, -1,  0}};

	double kernel3[KERNEL][KERNEL] = {
		{1.0 / 9, 1.0 / 9, 1.0 / 9},
		{1.0 / 9, 1.0 / 9, 1.0 / 9},
		{1.0 / 9, 1.0 / 9, 1.0 / 9}};

	double kernel4[KERNEL][KERNEL] = {
		{1.0 / 16, 2.0 / 16, 1.0 / 16},
		{2.0 / 16, 4.0 / 16, 2.0 / 16},
		{1.0 / 16, 2.0 / 16, 1.0 / 16}};

	scanf("%ms", &parameter);

	/**
	 * Treates every case individually
	 */
	if (!strcmp(parameter, "EDGE")) {
		filter(&*img, kernel1, &ok);
		if (ok)
			printf("APPLY EDGE done\n");

	} else if (!strcmp(parameter, "SHARPEN")) {
		filter(&*img, kernel2, &ok);
		if (ok)
			printf("APPLY SHARPEN done\n");

	} else if (!strcmp(parameter, "BLUR")) {
		filter(&*img, kernel3, &ok);
		if (ok)
			printf("APPLY BLUR done\n");

	} else if (!strcmp(parameter, "GAUSSIAN_BLUR")) {
		filter(&*img, kernel4, &ok);
		if (ok)
			printf("APPLY GAUSSIAN_BLUR done\n");

	} else {
		printf("APPLY parameter invalid\n");
	}

	free(parameter);
}

void save(format *img)
{
	/**
	 * Checks if there is any image loaded
	 */
	if (no_image(img))
		return;

	char *filename, chr;
	scanf("%ms", &filename);
	scanf("%c", &chr);

	FILE *fout = fopen(filename, "wt");
	if (!fout) {
		printf("Failed to save %s\n", filename);

		while (chr != '\n')
			scanf("%c", &chr);

		free(filename);
		return;
	}

	/**
	 * Depending on the existence of a word after the file
	 * name, load the image ascii or binary
	 */
	if (chr == ' ')
		scanf("%c", &chr);

	if (chr == '\n')
		write_binary(img, fout, filename);

	else
		write_ascii(img, fout);

	printf("Saved %s\n", filename);
	free(filename);
}

void quit(format *img)
{
	/**
	 * Checks if there is any image loaded
	 */
	if (!img->loaded) {
		printf("No image loaded\n");
		return;
	}

	/**
	 * If so, frees it
	 */
	if (!strcmp(img->word, "P2") || !strcmp(img->word, "P5")) {
		free_matrix(&img->gray, img->height);

	} else {
		free_matrix(&img->red, img->height);
		free_matrix(&img->green, img->height);
		free_matrix(&img->blue, img->height);
	}
}
