#include "my_ls.h"

// Bubble sort implementation for sorting by modification time
void bubble_sort_mtime(FileEntry *files, int count) {
    for (int i = 0; i < count - 1; i++) {
        for (int j = 0; j < count - i - 1; j++) {
            if (files[j].mtime < files[j + 1].mtime) {
                FileEntry temp = files[j];
                files[j] = files[j + 1];
                files[j + 1] = temp;
            }
        }
    }
}

// Bubble sort implementation for sorting by name (lexicographical order)
void bubble_sort_name(FileEntry *files, int count) {
    for (int i = 0; i < count - 1; i++) {
        for (int j = 0; j < count - i - 1; j++) {
            if (strcmp(files[j].name, files[j + 1].name) > 0) {
                FileEntry temp = files[j];
                files[j] = files[j + 1];
                files[j + 1] = temp;
            }
        }
    }
}

// List the contents of a directory
void list_directory(const char *path, int show_all, int sort_by_time) {
    DIR *dir;
    struct dirent *entry;
    FileEntry *files = NULL;
    int file_count = 0;
    struct stat file_stat;

    if (!(dir = opendir(path))) {
        perror("opendir");
        exit(EXIT_FAILURE);
    }

    while ((entry = readdir(dir)) != NULL) {
        if (!show_all && entry->d_name[0] == '.') {
            continue;
        }

        char filepath[MAX_FILENAME];
        snprintf(filepath, MAX_FILENAME, "%s/%s", path, entry->d_name);
        if (lstat(filepath, &file_stat) == -1) {
            perror("lstat");
            exit(EXIT_FAILURE);
        }

        files = realloc(files, sizeof(FileEntry) * (file_count + 1));
        if (!files) {
            perror("realloc");
            exit(EXIT_FAILURE);
        }

        files[file_count].name = malloc(strlen(entry->d_name) + 1);
        if (!files[file_count].name) {
            perror("malloc");
            exit(EXIT_FAILURE);
        }
        strcpy(files[file_count].name, entry->d_name);

        files[file_count].mtime = file_stat.st_mtim.tv_sec;
        files[file_count].is_dir = S_ISDIR(file_stat.st_mode);
        file_count++;
    }

    closedir(dir);

    // Sort by modification time if -t option is specified
    if (sort_by_time) {
        bubble_sort_mtime(files, file_count);
    } else {
        // Sort by name (lexicographical order)
        bubble_sort_name(files, file_count);
    }

    for (int i = 0; i < file_count; i++) {
        write(1, files[i].name, strlen(files[i].name));
        write(1, "\n", 1);
        free(files[i].name);
    }

    free(files);
}

int main(int argc, char *argv[]) {
    int show_all = 0;
    int sort_by_time = 0;

    // Parse the options -a and -t
    for (int i = 1; i < argc; i++) {
        if (argv[i][0] == '-') {
            for (int j = 1; argv[i][j]; j++) {
                if (argv[i][j] == 'a') {
                    show_all = 1;
                } else if (argv[i][j] == 't') {
                    sort_by_time = 1;
                } else {
                    printf("Unknown option: %c\n", argv[i][j]);
                    return EXIT_FAILURE;
                }
            }
        }
    }

    // If no directories are passed, list current directory
    if (argc == 1 || (argc == 2 && argv[1][0] == '-')) {
        list_directory(".", show_all, sort_by_time);
    } else {
        for (int i = 1; i < argc; i++) {
            if (argv[i][0] != '-') {
                struct stat path_stat;
                if (lstat(argv[i], &path_stat) == -1) {
                    perror("lstat");
                    return EXIT_FAILURE;
                }

                if (S_ISDIR(path_stat.st_mode)) {
                    printf("Contents of directory: %s:\n", argv[i]);
                    list_directory(argv[i], show_all, sort_by_time);
                } else {
                    write(1, argv[i], strlen(argv[i]));
                    write(1, "\n", 1);
                }
            }
        }
    }

    return 0;
}
