//my_ls.h

#ifndef MY_LS_H
#define MT_LS_H

#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <sys/stat.h>
#include <unistd.h>
#include <time.h>

#define MAX_FILENAME 512

typedef struct FileEntry {
    char *name;
    time_t mtime;
    int is_dir;
    struct FileEntry *next;
} FileEntry;

int manual_strcmp(const char *a, const char *b);
size_t manual_strlen(const char *s);
void manual_strcpy(char *dest, const char *src);
void manual_strcat(char *dest, const char *src);
FileEntry* insert_sorted(FileEntry *head, FileEntry *new_file, int sort_by_time);
void list_directory(const char *path, int show_all, int sort_by_time);

#endif 
