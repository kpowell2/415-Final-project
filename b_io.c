/**************************************************************
* Class:  CSC-415-0# Fall 2021
* Names: 
* Student IDs:
* GitHub Name:
* Group Name:
* Project: Basic File System
*
* File: b_io.c
*
* Description: Basic File System - Key File I/O Operations
*
**************************************************************/

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>			// for malloc
#include <string.h>			// for memcpy
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include "b_io.h"
#include "mfs.h"

#define MAXFCBS 20
#define B_CHUNK_SIZE 512

typedef struct b_fcb
	{
	/** TODO add al the information you need in the file control block **/
	fdDir* file;
	char * buf;		//holds the open file buffer
	int index;		//holds the current position in the buffer
	int buflen;		//holds how many valid bytes are in the buffer
	int eof;		//reach end of file
	int block;		//blocks used
	} b_fcb;
	
b_fcb fcbArray[MAXFCBS];

int startup = 0;	//Indicates that this has not been initialized

//Method to initialize our file system
void b_init ()
	{
	//init fcbArray to all free
	for (int i = 0; i < MAXFCBS; i++)
		{
		fcbArray[i].buf = NULL; //indicates a free fcbArray
		}
		
	startup = 1;
	}

//Method to get a free FCB element
b_io_fd b_getFCB ()
	{
	for (int i = 0; i < MAXFCBS; i++)
		{
		if (fcbArray[i].buf == NULL)
			{
			return i;		//Not thread safe (But do not worry about it for this assignment)
			}
		}
	return (-1);  //all in use
	}
	
// Interface to open a buffered file
// Modification of interface for this assignment, flags match the Linux flags for open
// O_RDONLY, O_WRONLY, or O_RDWR
b_io_fd b_open (char * filename, int flags)
	{
	b_io_fd returnFd;

	//*** TODO ***:  Modify to save or set any information needed
	//
	//
		
	if (startup == 0) b_init();  //Initialize our system
	
	returnFd = b_getFCB();				// get our own file descriptor
	if (returnFd == -1) {				// check for error - all used FCB's
		printf("b_open Error: All FCB in use\n");
		return -1;
	}

	fdDir* file = getDir(filename); // FIXME: create method
	if (file == NULL) {
		printf("b_open: file not found, attempting to create %s.", filename);

		if (flags & O_CREAT) {
			file = createDir(filename, 0777); // FIXME: create method
			char* parentPath = getParentPath(filename); // FIXME: create method
			fdDir* parent = getDir(parentPath);
			setParent(file, parent); // FIXME: create method
			writeDir(file); // FIXME: create method
		} else {
			printf("b_open Error: failed creating %s.", filename);
			return -1;
		}
	}

	fcbArray[returnFd].file = file;

	fcbArray[returnFd].buf = (char*) malloc(sizeof(char) * B_CHUNK_SIZE);
	if (fcbArray[returnFd].buf == NULL) {
		printf("b_open Error: buffer initialization failed.\n");
		return -1;
	}

	fcbArray[returnFd].buflen = 0;
	fcbArray[returnFd].index = 0;
	
	return (returnFd);						// all set
	}


// Interface to seek function	
int b_seek (b_io_fd fd, off_t offset, int whence)
	{
	if (startup == 0) b_init();  //Initialize our system

	// check that fd is between 0 and (MAXFCBS-1)
	if ((fd < 0) || (fd >= MAXFCBS))
		{
		return (-1); 					//invalid file descriptor
		}
		
		
	return (0); //Change this
	}



// Interface to write function	
int b_write (b_io_fd fd, char * buffer, int count)
	{
	if (startup == 0) b_init();  //Initialize our system

	// check that fd is between 0 and (MAXFCBS-1)
	if ((fd < 0) || (fd >= MAXFCBS))
		{
		return (-1); 					//invalid file descriptor
		}
	
	if (fcbArray[fd].buf == NULL) {
		return -1;
	}

	int bytesleft = count;

	while (bytesleft >= B_CHUNK_SIZE) {
		memcpy(fcbArray[fd].buf + fcbArray[fd].index, buffer + (count - bytesleft), freespace);
		bytesleft -= freespace;
		fcbArray[fd].index = 0;
		freespace = B_CHUNK_SIZE;
		fcbArray[fd].block++;
		writeBuffer(fcbArray[fd].file, fcbArray[fd].buf); // FIXME: implement method
	}

	if (bytesleft > 0) {
		memcpy(fcbArray[fd].buf + fcbArray[fd].index, buffer + (count - bytesleft), bytesleft);
		writeBuffer(fcbArray[fd].file, fcbArray[fd].buf); // FIXME: implement method
	}
		
	return count;
	}



// Interface to read a buffer

// Filling the callers request is broken into three parts
// Part 1 is what can be filled from the current buffer, which may or may not be enough
// Part 2 is after using what was left in our buffer there is still 1 or more block
//        size chunks needed to fill the callers request.  This represents the number of
//        bytes in multiples of the blocksize.
// Part 3 is a value less than blocksize which is what remains to copy to the callers buffer
//        after fulfilling part 1 and part 2.  This would always be filled from a refill 
//        of our buffer.
//  +-------------+------------------------------------------------+--------+
//  |             |                                                |        |
//  | filled from |  filled direct in multiples of the block size  | filled |
//  | existing    |                                                | from   |
//  | buffer      |                                                |refilled|
//  |             |                                                | buffer |
//  |             |                                                |        |
//  | Part1       |  Part 2                                        | Part3  |
//  +-------------+------------------------------------------------+--------+
int b_read (b_io_fd fd, char * buffer, int count)
	{

	if (startup == 0) b_init();  //Initialize our system

	// check that fd is between 0 and (MAXFCBS-1)
	if ((fd < 0) || (fd >= MAXFCBS))
		{
		return (-1); 					//invalid file descriptor
		}
		
	//---------------------------------------------------------------
	if (fcbArray[fd].file == NULL)		//File not open for this descriptor
		{
		return -1;
		}	

	// Your Read code here - the only function you call to get data is LBAread.
	// Track which byte in the buffer you are at, and which block in the file	
	if (fcbArray[fd].eof == 1) {
		return 0;
	}

	int totalBytesRead = (fcbArray[fd].block - 1) * B_CHUNK_SIZE + fcbArray[fd].index;

	// if reaching end of file, set count to remaining bytes
	if (totalBytesRead + count > fcbArray[fd].file->di->d_reclen) {
		count = fcbArray[fd].file->di->d_reclen - totalBytesRead;
		fcbArray[fd].eof = 1;
	}

	// if the buffer can provide the bytes needed
	if (fcbArray[fd].index + count < B_CHUNK_SIZE) {
		memcpy(buffer, fcbArray[fd].buf + fcbArray[fd].index, count);
		fcbArray[fd].index += count;
	// else we need to get more blocks of data
	} else {
		// get remaining data from current buffer
		int bytesRead = B_CHUNK_SIZE - fcbArray[fd].index;
		memcpy(buffer, fcbArray[fd].buf + fcbArray[fd].index, bytesRead);
		LBAread(fcbArray[fd].buf, 1, fcbArray[fd].file->dirEntryPosition + fcbArray[fd].block);
		fcbArray[fd].block++;

		// get blocks of data at a time
		while (bytesRead + B_CHUNK_SIZE <= count) {
			memcpy(buffer + bytesRead, fcbArray[fd].index, B_CHUNK_SIZE);
			LBAread(fcbArray[fd].buf, 1, fcbArray[fd].file->dirEntryPosition + fcbArray[fd].block);
			fcbArray[fd].block++;
			bytesRead += B_CHUNK_SIZE;
		}

		// get remaining bytes from new buffer
		if (count == 0) {
			fcbArray[fd].index = 0;
		} else {
			memcpy(buffer + bytesRead, fcbArray[fd].buf, count - bytesRead);
			fcbArray[fd].index = count - bytesRead;
		}
	}
	buffer[count] = '\0'; // null terminate the string at the requested byte marker
	return count;
	}
	//----------------------------------------------------------------
	
// Interface to Close the file	
int b_close (b_io_fd fd)
	{

	}
