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
#include "fsInit.h"
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

#define VCB_MAGIC_NUMBER 1234

vcb *VolumeControlBlock = NULL;

int initFileSystem(int numOfBlocks, int BlockSize){
    VolumeControlBlock = malloc(sizeof(vcb));
    if(VolumeControlBlock->magic_num == VCB_MAGIC_NUMBER){
        //volume already initialized
    }else{
        //initalize volume contorl block parameters and write to memory
    }



}