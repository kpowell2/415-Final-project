/**************************************************************
* Class:  CSC-415-02 Fall 2021
* Names: Kendall powell, Parampal Singh, Ashley Ching, Pankuri Khare
* Student IDs:921279668, 922759406, 921824056, 921385202
* GitHub Name: singh-paul, kpowell2, pankurik, aching4
* Group Name: incomplete
* Project: Milestone 1
*
* File: freespace.c
*
* Description: 
*
* 
*
**************************************************************/

#include "freespace.h"

// Utility function to get a specific bit from a byte
static bool getBit(byte b, int position) {
    return (b >> position) & 1;
}

// Utility function to set a specific bit in a byte
static void setBit(byte *b, int position) {
    *b |= (1 << position);
}

// Utility function to reset a specific bit in a byte
static void resetBit(byte *b, int position) {
    *b &= ~(1 << position);
}

byte *bitmapInstance() {
    return (byte *)malloc(BIT);
}

void bitmapInit() {
    byte *bitmap = bitmapInstance();
    memset(bitmap, 0, BIT);
    // Write the initial bitmap to block 1 since block 0 is the VCB
    LBAwrite(bitmap, 1, 1);
    free(bitmap);
}

void bitmapRead(byte *bitmap, uint64_t blockCount, uint64_t blockSize) {
    LBAread(bitmap, blockCount, blockSize);
}

void bitmapWrite(byte *bitmap, uint64_t blockCount, uint64_t blockSize) {
    LBAwrite(bitmap, blockCount, blockSize);
}

void bitmapRangeSet(byte *bitmap, uint64_t pos, uint64_t range) {
    for (uint64_t i = pos; i < (pos + range); i++) {
        bitmapSet(bitmap, i);
    }
}

void bitmapRangeReset(byte *bitmap, uint64_t pos, uint64_t range) {
    for (uint64_t i = pos; i < (pos + range); i++) {
        bitmapReset(bitmap, i);
    }
}

bool bitmapGet(byte *bitmap, uint64_t pos) {
    uint64_t bytePosition = pos / 8;
    int bitPosition = pos % 8;
    return getBit(bitmap[bytePosition], bitPosition);
}

void bitmapSet(byte *bitmap, uint64_t pos) {
    uint64_t bytePosition = pos / 8;
    int bitPosition = pos % 8;
    setBit(&bitmap[bytePosition], bitPosition);
}

void bitmapReset(byte *bitmap, uint64_t pos) {
    uint64_t bytePosition = pos / 8;
    int bitPosition = pos % 8;
    resetBit(&bitmap[bytePosition], bitPosition);
}

int bitmapSearch(byte *bitmap, bool n, uint64_t size, uint64_t start) {
    for (uint64_t i = start; i < size; i++) {
        if (bitmapGet(bitmap, i) == n) {
            return i;
        }
    }
    return BITMAP_NOTFOUND;
}

uint64_t bitmapFirstFreeRange(byte *bitmap, uint64_t blockCount, uint64_t range) {
    for (uint64_t i = 0; i < blockCount - range; i++) {
        bool found = true;
        for (uint64_t j = i; j < i + range; j++) {
            if (bitmapGet(bitmap, j)) {
                found = false;
                break;
            }
        }
        if (found) {
            return i;
        }
    }
    return BITMAP_NOTFOUND;
}

uint64_t bitmapFirstFreeFilespace(uint64_t fileSize) {
    // Assuming one block per file
    byte *bitmap = bitmapInstance();
    bitmapRead(bitmap, BIT, 1); // This might need adjustment based on your storage specs
    uint64_t position = bitmapFirstFreeRange(bitmap, BIT, fileSize);
    free(bitmap);
    return position;
}

int bitmapFreeFileSpace(uint64_t fileSize, uint64_t location) {
    byte *bitmap = bitmapInstance();
    bitmapRead(bitmap, BIT, 1);
    bitmapRangeReset(bitmap, location, fileSize);
    bitmapWrite(bitmap, BIT, 1);
    free(bitmap);
    return 0;  // Return some error code if needed
}

int bitmapAllocFileSpace(uint64_t fileSize, uint64_t location) {
    byte *bitmap = bitmapInstance();
    bitmapRead(bitmap, BIT, 1);
    bitmapRangeSet(bitmap, location, fileSize);
    bitmapWrite(bitmap, BIT, 1);
    free(bitmap);
    return 0;  // Return some error code if needed
}

