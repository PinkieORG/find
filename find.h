#include <dirent.h>
#include <pwd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include "utils.h"

#define BLOCK_SIZE 50

#ifndef HW05_FIND_H
#define HW05_FIND_H

/**
 * Structure that represents results of result of algorithm that finds desirable paths.
 * @var pathsNames is dynamically allocated array of pointers to dynamically allocated strings.
 * These strings are paths that were found by finding algorithm.
 * @var size is number of elements (pointers to strings) which can fit inside allocated array.
 * @var usedSize is number of elements that are used, meaning they point to some found path.
 * These are aligned to the left with no holes in between.
 */
typedef struct pathArray
{
    char **pathsNames;
    unsigned size;
    unsigned usedSize;
} pathArray;

/**
 * Finds all regular files that are children of inputted originalPath. On input has to be pathArray where pathArray.pathsNames
 * has to be allocated and its pathArray.size and pathArray.usedSize is correctly assigned.
 * Function adds new found elements on index pathArray.usedSize.
 * If there is no enough space for new elements it reallocates pathArray.pathsNames with more space. This process
 * increases pathArray.size by global variable BLOCK_SIZE.
 * @param originalPath string that contains absolute path in which to find files.
 * @param result structure that will be filled with result.
 * @return 0 on success, 1 otherwise.
 */
int find(const char *originalPath, pathArray *result);

/**
 * This will completely deallocate pathArray, meaning all goes through all its elements and frees them, then frees this
 * array itself. It uses pathArray.usedSize to know how many elements it has to free.
 * It also sets pathArray.usedSize and pathArray.size to 0.
 * @param paths pointer onto pathArray.
 */
void freeFileArray(pathArray *paths);

/**
 * Determines if inputted path is directory, regular path or something else.
 * @param path inputted path.
 * @return 1 if path is regular path,
 *         2 if path is directory,
 *         -1 if path is something else.
 */
int type(const char *path);

/**
 * Takes pathArray and removes those paths that do not include inputted substring inside its file's name.
 * These filtered elements of pathArray.pathsNames are freed, whole array shifts accordingly and
 * pathArray.usedSize is changed.
 * @param paths inputted pathArray.
 * @param substring string to be filtered by.
 */
void filterBySubstring(pathArray *paths, char *substring);

/**
 * Takes pathArray and removes those paths where its user permissions aren't same as those on the input.
 * These filtered elements of pathArray.pathsNames are freed, whole array shifts accordingly and
 * pathArray.usedSize is changed.
 * @param paths inputted pathArray.
 * @param mask is number of base 8, which represents user permissions as we know them from unix-like systems.
 */
void filterByPermissions(pathArray *paths, unsigned int mask);

/**
 * Takes pathArray and removes those paths which are not nested enough.
 * These filtered elements of pathArray.pathsNames are freed, whole array shifts accordingly and
 * @param paths inputted pathArray.
 * @param num determines how much a path can be nested to not be filtered.
 *            For example num = 2 means children are filtered but all grand children are not.
 */
void filterByMinimalNesting(pathArray *paths, unsigned num);

/**
 * Takes pathArray and removes those paths which are nested too much.
 * These filtered elements of pathArray.pathsNames are freed, whole array shifts accordingly and
 * @param paths inputted pathArray.
 * @param num determines how much a path can be nested to not be filtered.
 *            For example num = 2 means children and grandchildren are not filtered but everything after that is.
 */
void filterByMaximalNesting(pathArray *paths, unsigned num);

/**
 * Takes pathArray and removes those paths which are not owned by inputted owner.
 * @param paths inputted pathArray.
 * @param owner string containing owner to be filtered by.
 */
void filterByOwner(pathArray *paths, char *owner);

/**
 * Takes pathArray and removes all paths that contains dot files or dot directories.
 * @param paths inputted pathArray.
 */
void filterHiddenFiles(pathArray *paths);

/**
 * Prints paths inside pathArray on stdout.
 * @param paths inputted pathArray.
 * @param separator character to be in between each printed path.
 */
void printFileArray(pathArray *paths, char separator);

/**
 * Sorts paths.
 * @param paths inputted pathArray.
 */
void defaultSort(pathArray *paths);

/**
 * Sorts paths by names.
 * @param paths inputted pathArray.
 */
void sortByPath(pathArray *paths);

/**
 * Sorts paths by sizes of files.
 * @param paths inputted pathArray.
 */
void sortBySize(pathArray *paths);
#endif //HW05_FIND_H
