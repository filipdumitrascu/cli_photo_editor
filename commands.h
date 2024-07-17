/* Copyright 2023 < 323CA Dumitrascu Filip Teodor > */
#ifndef FUNCTIONS_H_
#define FUNCTIONS_H_

#include "helpers.h"

/**
 * @brief The function loads in the memory a new image
 * 
 * @param img the image
 */
void load(format * img);

/**
 * @brief The function selects the input coordinates.
 * 
 * @param img the image
 */
void select(format *img);

/**
 * @brief The function calculates the histogram of the image
 * 
 * @param img the image
 */
void histogram(format *img);

/**
 * @brief The function equalizes the image.
 * 
 * @param img the image
 */
void equalize(format *img);

/**
 * @brief The function rotates the image.
 * 
 * @param img the image
 */
void rotate(format *img);

/**
 * @brief The function crops the image.
 * 
 * @param img the image
 */
void crop(format *img);

/**
 * @brief The function aplies a given filter to the image
 * 
 * @param img the image
 */
void apply(format *img);

/**
 * @brief The function saves the image in a file
 * 
 * @param img the image
 */
void save(format *img);

/**
 * @brief The function deallocates the image and stops the program
 * 
 * @param img the image
 */
void quit(format *img);

#endif /* FUNCTIONS_H_ */
