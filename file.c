#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>

/* This function is used to find the root directory of a file system. It accomplishes this by traversing 
the directory hierarchy from the current directory to the root directory. The function prints out the 
path of each directory it visits until it reaches the root directory. 

Arguments:
path (const char *): A string representing the path of the directory to start the search from. 
This argument can be set to "." if you want to start the search from the current directory.

Return Value:
This function does not return a value. It simply prints out the path of each directory it 
visits until it reaches the root directory.

Usage:
root_directory("/path/to/directory"); // Start the search from the given directory path
root_directory("."); // Start the search from the current directory

This function is useful when you need to find the root directory of a file system. For example, 
if you have a script that needs to operate on a file or directory, but the path to the file or 
directory is not absolute, this function can be used to find the absolute path.

Note: This function assumes that the current directory is part of a valid file system, and it may 
not work correctly if the current directory is not part of a valid file system, such as when 
running the function from a deleted or unmounted file system.
*/

void root_directory(const char *path) {
    char current_dir[1024];
    char parent_dir[1024];

    while (1) {
        if (getcwd(current_dir, sizeof(current_dir)) == NULL) {
            perror("getcwd");
            exit(EXIT_FAILURE);
        }

        if (strcmp(current_dir, "/") == 0) {
            printf("Root directory: %s\n", current_dir);
            break;
        }

        struct stat statbuf;
        if (stat(".", &statbuf) == -1) {
            perror("stat");
            exit(EXIT_FAILURE);
        }

        if (S_ISDIR(statbuf.st_mode)) {
            printf("Given path is a directory: %s\n", current_dir);
            break;
        }

        strcpy(parent_dir, "..");
        if (chdir(parent_dir) == -1) {
            perror("chdir");
            exit(EXIT_FAILURE);
        }
    }
}