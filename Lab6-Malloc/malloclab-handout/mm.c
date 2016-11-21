/*
  * Author: Paarth Lakhani
  * uid: u0936913
  *
  * Description:
  I am dividing the ideal implementation of the malloc design into 7
  different parts. As in I am progressing, I'll describe the design
  phases.

  Part1:
  Part 1 is to implement the malloc using only the block_header and
  allocate accordingly. There are no footers yet.  
 */
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <unistd.h>
#include <string.h>

#include "mm.h"
#include "memlib.h"

/* always use 16-byte alignment */
#define ALIGNMENT 16

/* rounds up to the nearest multiple of ALIGNMENT */
#define ALIGN(size) (((size) + (ALIGNMENT-1)) & ~(ALIGNMENT-1))

/* rounds up to the nearest multiple of mem_pagesize() */
#define PAGE_ALIGN(size) (((size) + (mem_pagesize()-1)) & ~(mem_pagesize()-1))

// Macro for block overhead
#define OVERHEAD sizeof(block_header)

// Macro for getting the header from a payload pointer
// Gets the start of the block
// bp - block payload
#define HDRP(bp) ((char *)(bp) - sizeof(block_header))

// Macros for working with a raw pointer as the header
// Gets the size of block
#define GET_SIZE(p) ((block_header *)(p))->size 

// Is the block allocated or no ?
#define GET_ALLOC(p) ((block_header *)(p))->allocated

// Get the next block payload
#define NEXT_BLOCKP(bp) ((char *)(bp) + GET_SIZE(HDRP(bp))) 

// Defining the block header
typedef struct
{
  size_t size;
  char allocated; 
}block_header;

void *current_avail = NULL;
int current_avail_size = 0;

void *first_block;

 /*
int mm_init(void)
{
  current_avail = NULL;
  current_avail_size = 0;
  
  return 0;
}*/

int mm_init(void)
{
   size_t size_page = mem_pagesize();
   first_block = mem_map(size_page);
  // sbrk(sizeof(block_header));
  //first_block = sbrk(0);
  
   GET_SIZE(first_block)  = 0;
   GET_ALLOC(first_block) = 1;

  return 0;
}

/* 
 * mm_malloc - Allocate a block by using bytes from current_avail,
 *     grabbing a new page if necessary.
 */
void *mm_malloc(size_t size)
{
  int newsize = ALIGN(size);
  void *p;
  
  if (current_avail_size < newsize) {
    current_avail_size = PAGE_ALIGN(newsize);
    current_avail = mem_map(current_avail_size);
    if (current_avail == NULL)
      return NULL;
  }

  p = current_avail;
  current_avail += newsize;
  current_avail_size -= newsize;
  
  return p;
}

/*
 * mm_free - Freeing a block does nothing.
 */
void mm_free(void *ptr)
{
}
