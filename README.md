# CLI Photo Editor

#### Contributor: Dumitrascu Filip-Teodor

## Project description

> The project's theme is to implement an image editor for gray and color images


## Project structure

Makefile
> Includes build and clean rules.

Readme
> Describes the functionality of the project

*.c and *.h
> Files compiled via makefile

## image_editor.c
    Contains the main of the program that executes a repetitive structure based on multiple queries checked by strcmp function. This determines which operation to be performed on the image and calls the specific function

## utils
    Memory allocation, freeing functions and its resizing are performed for a matrix. Also there are functions to parse the parameters read from the keyboard to check whether the command is valid or not. The data type "format" is defined and contains all necessary information about the loaded image, including the dynamically allocated pixel arrays where the pixels are stored. Among the macros, DIE is called at each memory allocation for defensive programming.

## helpers
    The implementation of some helper functions for performing main operations on the image can be found in this file. A more complicated solution is used for the realization of 'rotate'. Thus, right, down, and left are based on the same principle: the allocation of a temporary matrix with the rotated values and the assignment to this matrix or just the values back into the image. The tricky part was finding a general formula for a simultaneous iteration through matrices to assign the values to the right place so that the program does not go out of the allocated memory.

## functions
    For each operation the function handles all the read cases to validate the command and calls helper functions to solve them. Thus, the functions are:

    LOAD - Loads the image from the given file into memory.
    HISTOGRAM - Make a histogram of the image using stars and bins
    EQUALIZE - Equalizes black and white images
    SELECT ALL - Selects the entire image
    SELECT - Selects a portion of the image between the given parameters
    CROP - Cuts the selection, keeping the new image smaller.
    ROTATE - Rotates the selection by the given angle
    APPLY - Applies one of the filters to the selection
    SAVE - Saves the image to a file
    EXIT - Clears the memory for the loaded image and exits the program
