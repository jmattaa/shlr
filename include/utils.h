#ifndef SHLR_UTILS_H
#define SHLR_UTILS_H

/**
 * check if a string is in an array
 * 
 * @param str The string to check
 * @param arr The array to check in
 * @param len The length of the array
 * 
 * @return 1 if the string is in the array, 0 otherwise
*/
#include <stddef.h>
int shlr_utils_inStrArr(char *str, char **arr, int len);


/**
 * read a file
 * 
 * @param path The path to the file
 * @param len Puts the length of the file in here
 * 
 * @return The contents of the file
 * 
*/
const char *shlr_utils_readFile(char *path, size_t *len);

#endif
