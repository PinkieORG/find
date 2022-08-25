#include <ctype.h>
#include <string.h>

#ifndef HW05_UTILS_H
#define HW05_UTILS_H

/**
 * Appends path (root) to path's name from left with '/' in between them.
 * @param newPathName string which will be result of this operation. There has to be enough space in it to hold
 * original path's name, root, null character and also '/'.
 * @param oldPathName string which holds original path's name.
 * @param root string which holds path to be added.
 */
void addPathToFileName(char *newPathName, const char *oldPathName, const char *root);

/**
 * Removes path from path's name, in sense that, it find last occurrence of '/' so that result will be pointer to the
 * character after that.
 * @param fileName string which will be result of this operation. There has to be enough space in it to hold the
 * result.
 * @param pathName string which holds original file's name.
 */
void removePathFromFileName(char *fileName, char *pathName);

/**
 * Determines number of '/' inside inputted path.
 * @param path inputted path.
 * @return number of slashes inside path.
 */
unsigned int numberOfSlashes(char *path);

/**
 * Takes string and changes it to lowercase.
 * @param str inputted string.
 */
void stringToLower(char *str);

/**
 * Swaps two strings.
 * @param str1 string number one.
 * @param str2 string number two.
 */
void swap(char **str1, char **str2);

/**
 * Determines if there is hidden directory in inputted path or if file at the end of path is hidden.
 * Function is looking for substring "/.".
 * @param path inputted path
 * @return 1 if condition is met, 0 otherwise.
 */
int isHiddenFile(const char *path);
#endif //HW05_UTILS_H
