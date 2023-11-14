/**************************************************************
*Class:  CSC-415-02 Fall 2021
* Names: Kendall powell, Parampal Singh, Ashley Ching, Pankuri Khare
* Student IDs:921279668, 922759406, 921824056, 921385202
* GitHub Name: singh-paul, kpowell2, pankurik, aching4
* Group Name: incomplete
* Project: Milestone 1
*
* File: vcb.h
*
* Description: 
*
* 
*
**************************************************************/

#ifndef _VCB_H
#define _VCB_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>

#include "fsLow.h"

#define VCB_MAGIC_NUM 0x12345678  // You can modify this to another value if you want

typedef struct vcb {
    uint64_t signature;  // Signature to identify the disk
    int block_size;     // Number of bytes for each block
    int num_free_blocks;    // Number of usable blocks

    uint64_t bitmap_start;   // Start position of the bitmap for free space
    uint64_t bitmap_blocks;  // Number of blocks occupied by the bitmap

    uint64_t free_file_block_count;    // Number of free directory entry blocks
    uint64_t free_file_block_pointers; // Pointers to each free directory entry block
} vcb;

// We set this at initialization then use it when we need
// to get the vcb
extern uint64_t globalBlockSize;

// This checks whether the vcb is set, using the signature
uint64_t isVCBSet(uint64_t blockSize);

// This sets or updates the vcb
uint64_t setVCB(int block_size, int num_blocks);

// This gets the vcb using the global block size variable
vcb* getVCBG();

// This gets the vcb
vcb* getVCB(uint64_t blockSize);


#endif
