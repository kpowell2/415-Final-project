/**************************************************************
* Class:  CSC-415-02 Fall 2021
* Names: Kendall powell, Parampal Singh, Ashley Ching, Pankuri Khare
* Student IDs:921279668, 922759406, 921824056, 921385202
* GitHub Name: singh-paul, kpowell2, pankurik, aching4
* Group Name: incomplete
* Project: Milestone 1
*
* File: directory.c
*
* Description: 
*
* 
*
**************************************************************/

#include <string.h>
#include <stdlib.h>

#include "directory.h"
#include "fsLow.h"

void initDirectoryEntry(DirectoryEntry* entry, const char* name, uint64_t start_block, uint64_t size, uint8_t is_directory) {
    strncpy(entry->name, name, MAX_NAME_LENGTH);
    entry->name[MAX_NAME_LENGTH] = '\0'; // Ensure null-termination
    entry->start_block = start_block;
    entry->size = size;
    entry->creation_time = time(NULL);
    entry->last_access_time = entry->creation_time;
    entry->last_modified_time = entry->creation_time;
    entry->is_directory = is_directory;
}

int initRootDirectory(uint64_t start_block, int block_size) {
    int entries_per_block = block_size / sizeof(DirectoryEntry);

    DirectoryEntry* dir_block = malloc(block_size);
    if (!dir_block) {
        // handle memory allocation failure, perhaps return an error code
        return -1; 
    }

    initDirectoryEntry(&dir_block[0], ".", start_block, block_size * entries_per_block, 1);
    initDirectoryEntry(&dir_block[1], "..", start_block, block_size * entries_per_block, 1);

    int writeStatus = LBAwrite(dir_block, 1, start_block);
    free(dir_block);

    return writeStatus;  // Assuming LBAwrite returns 0 on success, otherwise modify accordingly
}

