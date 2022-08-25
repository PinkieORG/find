#include "find.h"

int find(const char *originalPath, pathArray *result)
{
    DIR *dir = NULL;
    if ((dir = opendir(originalPath)) == NULL) {
        return -1;
    }
    struct dirent *dirInfo = NULL;
    while ((dirInfo = readdir(dir)) != NULL) {
        if (result->usedSize >= result->size) {
            result->size += BLOCK_SIZE;
            result->pathsNames = realloc(result->pathsNames, result->size * sizeof(char *));
            if (result->pathsNames == NULL) {
                freeFileArray(result);
                return -1;
            }
        }
        char foundPath[strlen(dirInfo->d_name) + strlen(originalPath) + 2 * sizeof(char)];
        addPathToFileName(foundPath, dirInfo->d_name, originalPath);
        int fileType = type(foundPath);
        if (fileType == 1) {
            result->pathsNames[result->usedSize] = malloc(strlen(foundPath) + 1);
            if (result->pathsNames[result->usedSize] == NULL) {
                freeFileArray(result);
                return -1;
            }
            strcpy(result->pathsNames[result->usedSize], foundPath);
            result->usedSize++;
        } else if (fileType == 2 && strcmp(dirInfo->d_name, ".") != 0 && strcmp(dirInfo->d_name, "..") != 0) {
            find(foundPath, result);
        }
    }
    closedir(dir);
    return 0;
}

void freeFileArray(pathArray *paths)
{
    for (unsigned i = 0; i < paths->usedSize; i++) {
        free(paths->pathsNames[i]);
    }
    free(paths->pathsNames);
    paths->size = 0;
    paths->usedSize = 0;
}

int type(const char *path)
{
    struct stat buf;
    stat(path, &buf);
    if (S_ISREG(buf.st_mode)) {
        return 1;
    }
    if (S_ISDIR(buf.st_mode)) {
        return 2;
    }
    return -1;
}

void filterBySubstring(pathArray *paths, char *substring)
{
    for (unsigned i = 0; i < paths->usedSize; i++) {
        char fileName[strlen(paths->pathsNames[i])];
        removePathFromFileName(fileName, paths->pathsNames[i]);
        if (strstr(fileName, substring) == NULL) {
            free(paths->pathsNames[i]);
            for (unsigned j = i; j < paths->usedSize; j++) {
                paths->pathsNames[j] = paths->pathsNames[j + 1];
            }
            paths->usedSize--;
            i--;
        }
    }
}

void filterByPermissions(pathArray *paths, unsigned int mask)
{
    for (unsigned i = 0; i < paths->usedSize; i++) {
        struct stat buf;
        stat(paths->pathsNames[i], &buf);
        unsigned permissions = buf.st_mode & (unsigned) 0000777;
        if ((permissions | mask) != mask) {
            free(paths->pathsNames[i]);
            for (unsigned j = i; j < paths->usedSize; j++) {
                paths->pathsNames[j] = paths->pathsNames[j + 1];
            }
            paths->usedSize--;
            i--;
        }
    }
}

void filterByMinimalNesting(pathArray *paths, unsigned num)
{
    for (unsigned i = 0; i < paths->usedSize; i++) {
        if (numberOfSlashes(paths->pathsNames[i]) + 1 < num) {
            free(paths->pathsNames[i]);
            for (unsigned j = i; j < paths->usedSize; j++) {
                paths->pathsNames[j] = paths->pathsNames[j + 1];
            }
            paths->usedSize--;
            i--;
        }
    }
}

void filterByMaximalNesting(pathArray *paths, unsigned num)
{
    for (unsigned i = 0; i < paths->usedSize; i++) {
        if (numberOfSlashes(paths->pathsNames[i]) >= num) {
            free(paths->pathsNames[i]);
            for (unsigned j = i; j < paths->usedSize; j++) {
                paths->pathsNames[j] = paths->pathsNames[j + 1];
            }
            paths->usedSize--;
            i--;
        }
    }
}

void filterByOwner(pathArray *paths, char *owner)
{
    struct passwd *ownerInfo;
    ownerInfo = getpwnam(owner);
    unsigned ownerUid = ownerInfo->pw_uid;
    for (unsigned i = 0; i < paths->usedSize; i++) {
        struct stat buf;
        stat(paths->pathsNames[i], &buf);
        if (buf.st_uid != ownerUid) {
            free(paths->pathsNames[i]);
            for (unsigned j = i; j < paths->usedSize; j++) {
                paths->pathsNames[j] = paths->pathsNames[j + 1];
            }
            i--;
            paths->usedSize--;
        }
    }
}

void filterHiddenFiles(pathArray *paths)
{
    for (unsigned i = 0; i < paths->usedSize; i++) {
        if (isHiddenFile(paths->pathsNames[i])) {
            free(paths->pathsNames[i]);
            for (unsigned j = i; j < paths->usedSize; j++) {
                paths->pathsNames[j] = paths->pathsNames[j + 1];
            }
            i--;
            paths->usedSize--;
        }
    }
}

void printFileArray(pathArray *paths, char separator)
{
    for (unsigned i = 0; i < paths->usedSize; i++) {
        char lol[strlen(paths->pathsNames[i]) + 1];
        strcpy(lol, paths->pathsNames[i]);
        printf("%s", paths->pathsNames[i]);
        putchar(separator);
    }
}

void defaultSort(pathArray *paths)
{
    if (paths->usedSize == 0) {
        return;
    }
    for (unsigned i = 0; i < paths->usedSize - 1; i++) {
        for (unsigned j = 0; j < paths->usedSize - i - 1; j++) {
            char fileName1[strlen(paths->pathsNames[j])];
            removePathFromFileName(fileName1, paths->pathsNames[j]);
            stringToLower(fileName1);

            char fileName2[strlen(paths->pathsNames[j + 1])];
            removePathFromFileName(fileName2, paths->pathsNames[j + 1]);
            stringToLower(fileName2);

            if (strcmp(fileName1, fileName2) > 0 || (strcmp(fileName1, fileName2) == 0 && strcmp(paths->pathsNames[j], paths->pathsNames[j + 1]) > 0)) {
                swap(&paths->pathsNames[j], &paths->pathsNames[j + 1]);
            }
        }
    }
}

void sortByPath(pathArray *paths)
{
    if (paths->usedSize == 0) {
        return;
    }
    for (unsigned i = 0; i < paths->usedSize - 1; i++) {
        for (unsigned j = 0; j < paths->usedSize - i - 1; j++) {
            if (strcmp(paths->pathsNames[j], paths->pathsNames[j + 1]) > 0) {
                swap(&paths->pathsNames[j], &paths->pathsNames[j + 1]);
            }
        }
    }
}

void sortBySize(pathArray *paths)
{
    if (paths->usedSize == 0) {
        return;
    }
    for (unsigned i = 0; i < paths->usedSize - 1; i++) {
        for (unsigned j = 0; j < paths->usedSize - i - 1; j++) {
            struct stat buf1;
            stat(paths->pathsNames[j], &buf1);
            struct stat buf2;
            stat(paths->pathsNames[j + 1], &buf2);
            if (buf1.st_size < buf2.st_size) {
                swap(&paths->pathsNames[j], &paths->pathsNames[j + 1]);
            }
        }
    }
}