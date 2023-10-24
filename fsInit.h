/**************************************************************
* Class:  CSC-415-02 Fall 2023
* Names :Kendall Powell
* Team Name: incomplete
* GitHub Name:kpowell2
* Project: file system project
*
* File: fsInit.h
*
* Description: 
*
**************************************************************/
#ifndef _FS_INIT_h
#define _FS_INIT_H

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <pthread.h>
#include <string.h>
#include <ctype.h>

#define MAX_NAME_LENGTH 256
#define MAX_FILE_LOCATION_LENGTH 256

typedef struct vcb{
    int magic_num;	// number to identify the disk
    int block_size;	// number of bytes for each block
    int num_free_blocks;	// number of useable block
    int free_block_pointers;	// pointers to each free block
    int fat_start;		// start position of file allocation table
    int fat_size;		// number of blocks for the FAT
    int free_file_block_count;		// number of free directory entry blocks
    int free_file_block_pointers;	// pointers to each free directory entry block
}vcb;

struct DirectoryEntry {
    char name[MAX_NAME_LENGTH]; // Name of the file or directory
    int file_size; // Size of the file
    char file_location[MAX_FILE_LOCATION_LENGTH]; //file location
    time_t creationTime; // Creation timestamp
    time_t modificationTime; // Last modification timestamp
    time_t accesstime; // last access time
};


int initFileSystem(int numOfBlocks, int blockSize);
int initBitMap();
int initRootDirectory();




#endif