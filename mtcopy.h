/************************************************
* mtcopy:
* 	multi-thread file copy 
* Usage:
* 	mthread <source file> <destination file> <thread number>
* 
* Multi-thread study, By Liang, 2014/03/03
************************************************/
#include <iostream>
#include <string.h>
#include <vector>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>  
#include <fcntl.h> 
#include <unistd.h>
#include <pthread.h>


#define BUFFER_SIZE 1024
#define MAX_THREAD_NO 10  /* set max threads to start */

#ifdef DEBUG
#define _DEBUG DEBUG
#endif

/************************************************
* pre-defined data structure
************************************************/
typedef struct file_info
{
    char *from; 
    char *to;  
	int thread_idx;
	int thread_max;
} file_info;


/************************************************
* get_size:
* 	return size of file to copy
************************************************/
off_t get_size(const char *file_name);


/************************************************
* thread_copy:
* 	do copy operation
************************************************/
void * thread_copy(void *fi);



