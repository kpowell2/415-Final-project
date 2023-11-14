/**************************************************************
* Class:  CSC-415-02 Fall 2021
* Names: Kendall powell, Parampal Singh, Ashley Ching, Pankuri Khare
* Student IDs:921279668, 922759406, 921824056, 921385202
* GitHub Name: singh-paul, kpowell2, pankurik, aching4
* Group Name: incomplete
* Project: Milestone 1
*
* File: directory.h
*
* Description: 
*
* 
*
**************************************************************/
#ifndef DIRECTORY_H
#define DIRECTORY_H

#include <inttypes.h>
#include <time.h>

// Assuming each directory entry can have a name of up to 255 characters
#define MAX_NAME_LENGTH 255
#define MAX_ENTRIES_PER_DIRECTORY (BLOCK_SIZE / sizeof(DirectoryEntry))


typedef struct DirectoryEntry {
    char name[MAX_NAME_LENGTH + 1]; // Name of the file or directory
    uint64_t start_block;           // Starting block of the file/directory data
    uint64_t size;                  // Size of the file or directory
    time_t creation_time;           // Time of creation
    time_t last_access_time;        // Last accessed time
    time_t last_modified_time;      // Last modified time
    uint8_t is_directory;           // 1 if directory, 0 if file
    
} DirectoryEntry;

// Initialize a directory entry with given details
void initDirectoryEntry(DirectoryEntry* entry, const char* name, uint64_t start_block, uint64_t size, uint8_t is_directory);


// Initialize the root directory with "." and ".." entries
int initRootDirectory(uint64_t start_block, int block_size);

#endif
