#include "my_ls.h"

int compare_mtime(const void *a, const void *b) {
    time_t time_a = ((FileEntry *)a)->mtime;
    time_t time_b = ((FileEntry *)b)->mtime;
    return (time_b - time_a);
}

int compare_name(const void *a, const void *b) {
    return strcmp(((FileEntry *)a)->name, ((FileEntry *)b)->name);
}

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

    if (sort_by_time) {
        qsort(files, file_count, sizeof(FileEntry), compare_mtime);
    }

    qsort(files, file_count, sizeof(FileEntry), compare_name);

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

    for (int i = 1; i < argc; i++) {
        if (argv[i][0] == '-') {
            for (int j = 1; argv[i][j]; j++) {
                if (argv[i][j] == 'a') {
                    show_all = 1;
                } else if (argv[i][j] == 't') {
                    sort_by_time = 1;
                }
            }
        }
    }

    if (argc == 1 || (argc == 2 && argv[1][0] == '-' && argv[1][1] == '\0')) {
        list_directory(".", show_all, sort_by_time);
    } else {
        for (int i = 1; i < argc; i++) {
            if (argv[i][0] != '-') {
                struct stat path_stat;
                if (lstat(argv[i], &path_stat) == -1) {
                    perror("lstat");
                    exit(EXIT_FAILURE);
                }

                if (S_ISDIR(path_stat.st_mode)) {
                    write(1, "Contents of directory: ", 22);
                    write(1, argv[i], strlen(argv[i]));
                    write(1, ":\n", 2);
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
