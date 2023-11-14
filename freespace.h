/**************************************************************
* Class:  CSC-415-02 Fall 2021
* Names: Kendall powell, Parampal Singh, Ashley Ching, Pankuri Khare
* Student IDs:921279668, 922759406, 921824056, 921385202
* GitHub Name: singh-paul, kpowell2, pankurik, aching4
* Group Name: incomplete
* Project: Milestone 1
*
* File: freespace.h
*
* Description: 
*
* 
*
**************************************************************/

#ifndef _FREESPACE_H
#define _FREESPACE_H


#include <stdio.h>
#include <string.h>
#include <sys/types.h>

#include "fsLow.h"
#include "vcb.h"


#define BIT (8*sizeof(byte))
#define BITMAP_NOTFOUND -1

typedef enum{false=0, true} bool;
typedef u_int8_t byte;

// malloc a bitmap
byte * bitmapInstance();
// create the bitmap
void bitmapInit();
// read the bitmap from disk into the passed-in byte array
void bitmapRead      (byte * bitmap, uint64_t blockCount, uint64_t blockSize);
// write the passed-in byte array to the disk as the bitmap
void bitmapWrite     (byte * bitmap, uint64_t blockCount, uint64_t blockSize);
// sets a range of bits in the bitmap to 1, indicating those blocks are
// set in memory
void bitmapRangeSet  (byte * bitmap, uint64_t pos, uint64_t range);
// sets a range of bits in the bitmap to 0, indicating those blocks are
// free in memory
void bitmapRangeReset(byte * bitmap, uint64_t pos, uint64_t range);
// get the value of a single bit from the bitmap
bool bitmapGet       (byte * bitmap, uint64_t pos);
// set a single bit in the bitmap to 1
void bitmapSet       (byte * bitmap, uint64_t pos);
// set a single bit in the bitmap to 0
void bitmapReset     (byte * bitmap, uint64_t pos);
// search the bitmap from position start for the next subsequent bit of value n
int  bitmapSearch    (byte * bitmap, bool n, uint64_t size, uint64_t start);
// return the location of the first span of range length bits set to 0
uint64_t bitmapFirstFreeRange(byte * bitmap, uint64_t blockCount, uint64_t range);
// returns the location of the first span of roundUpDiv(filesize/blockSize) bits set to empty
uint64_t bitmapFirstFreeFilespace(uint64_t fileSize);
// read bitmap from disk, free filespace, write bitmap to disk
int bitmapFreeFileSpace(uint64_t fileSize, uint64_t location);
// read bitmap from disk, allocate filespace, write bitmap to disk
int bitmapAllocFileSpace(uint64_t fileSize, uint64_t location);


#endif
