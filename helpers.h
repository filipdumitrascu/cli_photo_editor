/* Copyright 2023 < 323CA Dumitrascu Filip Teodor > */
#ifndef HELPERS_H_
#define HELPERS_H_

#include "utils.h"

/**
 * @brief Reads from the file ascii values and stores them in the matrices
 * 
 * @param img the image
 * @param fin the file
 */
void read_ascii(format * img, FILE * fin);

/**
 * @brief Reads from the file binary values and stores them in the matrices
 * 
 * @param img the image
 * @param fin the file
 * @param filename the file name
 */
void read_binary(format *img, FILE *fin, char *filename);

/**
 * @brief Selects the entire image
 * 
 * @param img the image
 */
void all(format *img);

/**
 * @brief The function checks if the input string is an integer
 * 
 * @param string the string
 * @return int 0/1 (sort of boolean)
 */
int is_integer(char *string);

/**
 * @brief If the input string is not an integer, returns from the command
 * 
 * @param string the string
 * @return int 0/1 (sort of boolean)
 */
int invalid_param(char *string);

/**
 * @brief The function rotates a matrix 90 degrees to the right
 * 
 * @param img the image
 * @param matrix the matrix
 */
void right(format *img, double ***matrix);

/**
 * @brief The function rotates a matrix 180 degrees
 * 
 * @param img the image
 * @param matrix the matrix
 */
void down(format *img, double ***matrix);

/**
 * @brief The function rotates a matrix 90 degrees to the left
 * 
 * @param img the image
 * @param matrix the matrix
 */
void left(format *img, double ***matrix);

/**
 * @brief The function aplies the given filter to the image
 * 
 * @param img the image
 * @param kernel the kernel
 * @param ok if can be applied
 */
void filter(format *img, double kernel[KERNEL][KERNEL], int *ok);

/**
 * @brief The function creates an ascii file with the matrix
 * 
 * @param img the image
 * @param fout the file
 */
void write_ascii(format *img, FILE *fout);

/**
 * @brief The function creates a binary file with the matrix
 * 
 * @param img the image
 * @param fout the file
 * @param filename the file name
 */
void write_binary(format *img, FILE *fout, char *filename);

#endif /* HELPERS_H */
