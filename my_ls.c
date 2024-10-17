#include "my_ls.h"

int my_strcmp(const char *a, const char *b) {
    while (*a && (*a == *b)) {
        a++;
        b++;
    }
    return *(unsigned char *)a - *(unsigned char *)b;
}

size_t my_strlen(const char *s) {
    size_t len = 0;
    while (*s++) len++;
    return len;
}

void my_strcpy(char *dest, const char *src) {
    while ((*dest++ = *src++) != '\0');
}

void my_strcat(char *dest, const char *src) {
    while (*dest) {
        dest++;  
    }
    while ((*dest++ = *src++) != '\0');  //Copy the source string to the destination
}

//Insert files into the list sorted by modification time or name
FileEntry* insert_sorted(FileEntry *head, FileEntry *new_file, int sort_by_time) {
    if (!head || (sort_by_time && new_file->mtime > head->mtime) || 
        (!sort_by_time && my_strcmp(new_file->name, head->name) < 0)) {
        new_file->next = head;
        return new_file;
    }

    FileEntry *current = head;
    while (current->next && 
           ((sort_by_time && new_file->mtime <= current->next->mtime) || 
            (!sort_by_time && my_strcmp(new_file->name, current->next->name) >= 0))) {
        current = current->next;
    }
    new_file->next = current->next;
    current->next = new_file;

    return head;
}

//List the contents of a directory
void list_directory(const char *path, int show_all, int sort_by_time) {
    DIR *dir;
    struct dirent *entry;
    struct stat file_stat;
    FileEntry *files = NULL;

    //Open the directory
    if (!(dir = opendir(path))) {
        perror("opendir");
        exit(EXIT_FAILURE);
    }

    //Read directory entries
    while ((entry = readdir(dir)) != NULL) {
        if (!show_all && entry->d_name[0] == '.') {
            continue;  // Skip hidden files unless -a is specified
        }

        //Get file info
        char filepath[MAX_FILENAME];
        my_strcpy(filepath, path); //Copy path
        my_strcat(filepath, "/");  //Append separator
        my_strcat(filepath, entry->d_name);  //Append name

        if (lstat(filepath, &file_stat) == -1) {
            perror("lstat");
            continue; 
        }

        //Create a new FileEntry and fill it with the info
        FileEntry *new_file = malloc(sizeof(FileEntry));
        if (!new_file) {
            perror("malloc");
            exit(EXIT_FAILURE);
        }
        new_file->name = malloc(my_strlen(entry->d_name) + 1); 
        if (!new_file->name) {
            perror("malloc");
            free(new_file);
            exit(EXIT_FAILURE);
        }
        my_strcpy(new_file->name, entry->d_name);
        new_file->mtime = file_stat.st_mtim.tv_sec;
        new_file->is_dir = S_ISDIR(file_stat.st_mode);
        new_file->next = NULL;

        //Insert the new file into the sorted linked list
        files = insert_sorted(files, new_file, sort_by_time);
    }

    closedir(dir);

    //Print the file names
    FileEntry *current = files;
    while (current) {
        write(1, current->name, my_strlen(current->name)); 
        write(1, "\n", 1); 
        FileEntry *next = current->next;
        free(current->name);
        free(current);
        current = next;
    }
}

int main(int argc, char *argv[]) {
    int show_all = 0;   //Flag for showing all files (including hidden)
    int sort_by_time = 0; //Flag for sorting by modification time

    //Command line arguments
    for (int i = 1; i < argc; i++) {
        if (argv[i][0] == '-') { // Check for options
            for (int j = 1; argv[i][j]; j++) {
                if (argv[i][j] == 'a') {
                    show_all = 1; // Set flag for showing all files
                } else if (argv[i][j] == 't') {
                    sort_by_time = 1; // Set flag for sorting by time
                }
            }
        }
    }

    //If no operands are given, list current directory
    if (argc == 1 || (argc == 2 && argv[1][0] == '-' && argv[1][1] == '\0')) {
        list_directory(".", show_all, sort_by_time);
    } else {
        for (int i = 1; i < argc; i++) {
            if (argv[i][0] != '-') { //Ignore options
                struct stat path_stat;
                //Get statistics for the specified file or directory
                if (lstat(argv[i], &path_stat) == -1) {
                    perror("lstat");
                    exit(EXIT_FAILURE);
                }

                //If it's a directory, list its contents
                if (S_ISDIR(path_stat.st_mode)) {
                    write(1, "Contents of directory: ", 22);
                    write(1, argv[i], my_strlen(argv[i])); 
                    write(1, ":\n", 2);
                    list_directory(argv[i], show_all, sort_by_time);
                } 
                else {// If it's a regular file, just print its name
                    write(1, argv[i], my_strlen(argv[i])); 
                    write(1, "\n", 1);
                }
            }
        }
    }

    return 0;
}
