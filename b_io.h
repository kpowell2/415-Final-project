/**************************************************************
* Class:  CSC-415-02 Fall 2021
* Names: Kendall powell, Parampal Singh, Ashley Ching, Pankuri Khare
* Student IDs:921279668, 922759406, 921824056, 921385202
* GitHub Name: singh-paul, kpowell2, pankurik, aching4
* Group Name: incomplete
* Project: Basic File System
*
* File: b_io.h
*
* Description: Interface of basic I/O functions
*
**************************************************************/

#ifndef _B_IO_H
#define _B_IO_H
#include <fcntl.h>

typedef int b_io_fd;

b_io_fd b_open (char * filename, int flags);
int b_read (b_io_fd fd, char * buffer, int count);
int b_write (b_io_fd fd, char * buffer, int count);
int b_seek (b_io_fd fd, off_t offset, int whence);
int b_close (b_io_fd fd);

#endif

