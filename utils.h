/* Copyright 2023 < 323CA Dumitrascu Filip Teodor > */
#ifndef UTILS_H_
#define UTILS_H_

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>

#define MAGIC 3
#define KERNEL 3
#define MAX_PARAM 10
#define MAX_ASCII 255
#define MAX_POWER2 256
#define CIRCLE 360

/**
 * Useful macro for handling error codes
 */
#define DIE(assertion, call_description)                                       \
	do {                                                                       \
		if (assertion) {                                                       \
			fprintf(stderr, "(%s, %d): ", __FILE__, __LINE__);                 \
			perror(call_description);                                          \
			exit(errno);                                                       \
		}                                                                      \
	} while (0)

typedef unsigned char _uc;

typedef struct format {
	/**
	 *  Matrices for gray/rgb type
	 */
	double **gray, **red, **green, **blue;
	/**
	 * Width & height of the image, x1...y2 current selection, a flag to check
	 * if the matrix is loaded or not and the maximum ascii value of the pixels
	 */
	int width, height, x1, y1, x2, y2, loaded, max;
	/**
	 * P2, P3, P5 or P6, the type of the load
	 */
	char word[MAGIC];
} format;

/**
 * @brief Allocates memory for a matrix
 * 
 * @param matrix the matrix
 * @param rows how many rows
 * @param columns how many columns
 */
void alloc_matrix(double ***matrix, int rows, int columns);

/**
 * @brief Deallocates the memory of a matrix
 * 
 * @param matrix the matrix
 * @param rows how many rows
 */
void free_matrix(double ***matrix, int rows);

/**
 * @brief Resizes the dimensions of a matrix
 * 
 * @param img the image
 * @param matrix the matix 
 */
void resize_matrix(format *img, double ***matrix);

/**
 * @brief Skips the commented line from the file
 * 
 * @param fin the file
 */
void skip_comment(FILE *fin);

/**
 * @brief The function cheks if there is any image loaded
 * 
 * @param img the image 
 * @return int 
 */
int no_image(format *img);

/**
 * @brief The function returns from the command if next char is
 * the end of the line
 * 
 * @return int 0/1 (sort of boolean)
 */
int end_of_line(void);

/**
 * @brief The function returns from the command if the line is not finished
 * 
 * @return int 0/1 (sort of boolean)
 */
int line_not_finished(void);

/**
 * @brief The function swaps the values of two integers
 * 
 * @param a the first value
 * @param b the second value
 */
void swap(int *a, int *b);

/**
 * @brief The function fits the sum between 0 and 255
 * 
 * @param sum the sum
 * @return double 
 */
double clamp(double sum);

#endif /* UTILS_H_ */
