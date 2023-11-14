/**************************************************************
* Class:  CSC-415-02 Fall 2021
* Names: Kendall powell, Parampal Singh, Ashley Ching, Pankuri Khare
* Student IDs:921279668, 922759406, 921824056, 921385202
* GitHub Name: singh-paul, kpowell2, pankurik, aching4
* Group Name: incomplete
* Project: Basic File System
*
* File: vcb.c
*
*
**************************************************************/
#include "vcb.h"
#include "freespace.h"

// Global block size variable
uint64_t globalBlockSize;

uint64_t isVCBSet(uint64_t blockSize) {
    vcb *tempVCB = (vcb *)malloc(blockSize);
    LBAread(tempVCB, 1, 0); // Assuming VCB is at the start of the disk

    uint64_t signature = tempVCB->signature;
    free(tempVCB);

    if (signature == VCB_MAGIC_NUM) {
        return 1; // VCB is set
    }
    return 0; // VCB is not set
}

uint64_t setVCB(int block_size, int num_blocks) {
    vcb newVCB;
    newVCB.signature = VCB_MAGIC_NUM;
    newVCB.block_size = block_size;
    newVCB.num_free_blocks = num_blocks;

    // Assuming block 1 to block 5 (inclusive) is reserved for the free space bitmap
    newVCB.bitmap_start = 1;
    newVCB.bitmap_blocks = 5; // Adjust based on your actual design

    newVCB.free_file_block_count = 0; // Initialize with 0, modify as required
    newVCB.free_file_block_pointers = 0; // Initialize with 0, modify as required

    LBAwrite(&newVCB, 1, 0); // Writing VCB to the start of the disk

    return 1; // Successfully set VCB
}

vcb* getVCBG() {
    return getVCB(globalBlockSize);
}

vcb* getVCB(uint64_t blockSize) {
    vcb *tempVCB = (vcb *)malloc(blockSize);
    LBAread(tempVCB, 1, 0); // Reading VCB from the start of the disk
    return tempVCB;
}
