#ifndef MY_LS_H
#define MY_LS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <time.h>
#include <sys/stat.h>
#include <dirent.h>
#include <unistd.h>

#define MAX_FILENAME 1024

typedef struct FileEntry {
    char *name;
    time_t mtime;
    int is_dir;
} FileEntry;

void list_directory(const char *path, int show_all, int sort_by_time);
int compare_mtime(const void *a, const void *b);
int compare_name(const void *a, const void *b);

#endif
