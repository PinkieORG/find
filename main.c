#include <getopt.h>
#include <unistd.h>
#include "find.h"

#define MAX_ROOT_SIZE 200
#define MIN_MASK 111
#define MAX_MASK 777

int main(int argc, char *argv[])
{
    int opt;
    char *substring = NULL;
    char sort = '0';
    long mask = 0;
    char *owner = NULL;
    long from = -1;
    long to = -1;
    int hidden = 1;
    char separator = '\n';

    while ((opt = getopt(argc, argv, "n:s:m:u:f:t:a0h")) != -1) {
        switch (opt) {
        case 'n':
            if (optarg == NULL || optarg[0] == '-') {
                fprintf(stderr, "Invalid substring.\n");
                return -1;
            }
            substring = optarg;
            break;
        case 's':
            if (strcmp(optarg, "f") != 0 && strcmp(optarg, "s") != 0) {
                fprintf(stderr, "Invalid sorting option.\n");
                return -1;
            }
            sort = optarg[0];
            break;
        case 'm':
            mask = strtol(optarg, NULL, 10);
            if (mask < MIN_MASK || mask > MAX_MASK) {
                fprintf(stderr, "Invalid mask.\n");
                return -1;
            }
            break;
        case 'u':
            if (optarg == NULL || optarg[0] == '-') {
                fprintf(stderr, "Invalid owner.\n");
                return -1;
            }
            owner = optarg;
            break;
        case 'f':
            if (strtol(optarg, NULL, 10) < 0) {
                fprintf(stderr, "Invalid minimal nesting.\n");
                return -1;
            }
            from = strtol(optarg, NULL, 10);
            break;
        case 't':
            if (strtol(optarg, NULL, 10) < 0) {
                fprintf(stderr, "Invalid maximal nesting.\n");
                return -1;
            }
            to = strtol(optarg, NULL, 10);
            break;
        case 'a':
            hidden = 0;
            break;
        case '0':
            separator = '\0';
            break;
        case 'h':
            puts("Finding files in directory tree.\n"
                 "-n ARG    filter files by ARG\n"
                 "-s f | s  sort result by name (f) or by size (s)\n"
                 "-m MASK   filter by user permissions given by MASK\n"
                 "-u USER   filter by owner of the file given by USER\n"
                 "-f NUM    filter by minimal nesting given by NUM\n"
                 "-t NUM    filter by maximal nesting given by NUM\n"
                 "-a        include hidden files and files in hidden directories\n"
                 "-0        prints result on one line\n"
                 "-h        prints this help");
            return 0;
        case ':':
            fprintf(stderr, "Missing argument.\n");
            return -1;
        case '?':
            fprintf(stderr, "Invalid argument\n");
            return -1;
        default:
            break;
        }
    }
    pathArray result = { malloc(BLOCK_SIZE * sizeof(char *)), BLOCK_SIZE, 0};
    char root[MAX_ROOT_SIZE];
    getcwd(root, sizeof(root));
    if (optind >= argc) {
        find(".", &result);
    } else {
        find(argv[optind], &result);
    }
    if (hidden) {
        filterHiddenFiles(&result);
    }
    if (substring != NULL) {
        filterBySubstring(&result, substring);
    }
    if (sort == 'f') {
        sortByPath(&result);
    } else if (sort == 's') {
        sortBySize(&result);
    } else {
        defaultSort(&result);
    }
    if (mask != 0) {
        filterByPermissions(&result, mask);
    }
    if (owner != NULL) {
        filterByOwner(&result, owner);
    }
    if (from != -1) {
        filterByMinimalNesting(&result, from);
    }
    if (to != -1) {
        filterByMaximalNesting(&result, to);
    }
    printFileArray(&result, separator);
    freeFileArray(&result);
    return 0;
}

