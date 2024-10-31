# Welcome to My Ls
****************************************************************

## Task
The goal is to implement a simplified version of the `ls` command, called `my_ls`, which lists the contents of a directory, with specific options for including hidden files and sorting based on modification time.

The specifications include:
- Display file and directory contents, excluding hidden files by default.
- Provide an `-a` option to include hidden files (files starting with a dot).
- Provide a `-t` option to sort files by modification time, from most recent to oldest.
- If no operands are given, the contents of the current directory are displayed.
- If multiple operands are provided, non-directory operands are displayed first, followed by directory contents, both sorted lexicographically.

## Description
To achieve the behavior of `my_ls`, we implemented the following steps:

1. **Directory and File Handling**: The program reads the contents of a directory using `opendir` and `readdir`. It processes each entry and collects relevant information like file name and modification time. For non-directory files, their names are printed immediately.

2. **Options Parsing**: The program parses the `-a` and `-t` options from command-line arguments. Based on these options, it adjusts which files are shown (including or excluding hidden files) and how they are sorted (by modification time or name).

3. **Custom Sorting**: The program sorts files lexicographically by default. When the `-t` option is specified, files are sorted by modification time instead, using bubble sort method instead of qsort.

4. **File Metadata**: The modification time is retrieved using `lstat`, and the `st_mtim` field is used for more accurate time resolution, as opposed to `st_mtime`.

## Installation
1. Clone the repository.
2. Compile the program using the Makefile provided:
   make
   OR make re (if you need to recompile it)
3. Run the program

## Usage
1. To list the contents of the current directory:
    ./my_ls
2. To include hidden files:
    ./my_ls -a
3. To sort files by modification time:
    ./my_ls -t
4. To list the contents of a specific directory:
    ./my_ls /path/to/directory
5. To combine options (e.g., show hidden files and sort by time):
    ./my_ls -at /path/to/directory

### The Core Team
Project completed by Nikita Gaidamachenko

<span><i>Made at <a href='https://qwasar.io'>Qwasar SV -- Software Engineering School</a></i></span>
<span><img alt='Qwasar SV -- Software Engineering School's Logo' src='https://storage.googleapis.com/qwasar-public/qwasar-logo_50x50.png' width='20px' /></span>
