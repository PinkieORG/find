#include "utils.h"

void addPathToFileName(char *newPathName, const char *oldPathName, const char *root)
{
    strcpy(newPathName, root);
    strcat(newPathName, "/");
    strcat(newPathName, oldPathName);
}

void removePathFromFileName(char *fileName, char *pathName)
{
    strcpy(fileName, strrchr(pathName, '/') + 1);
}

unsigned int numberOfSlashes(char *path)
{
    if (strchr(path, '/') == NULL) {
        return 0;
    }
    unsigned numberOfSlashes = 1;
    char *substring = strchr(path, '/') + 1;
    while (substring != NULL) {
        if (strchr(substring, '/') == NULL) {
            break;
        }
        substring = strchr(substring, '/') + 1;
        numberOfSlashes++;
    }
    return numberOfSlashes;
}

void stringToLower(char *str)
{
    for (unsigned i = 0; i < strlen(str); i++) {
        str[i] = (char) tolower(str[i]);
    }
}

void swap(char **str1, char **str2)
{
    char *cmp = *str1;
    *str1 = *str2;
    *str2 = cmp;
}

int isHiddenFile(const char *path)
{
    if (strstr(path, "/.")) {
        return 1;
    }
    return 0;
}