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

void *first_block;

/* always use 16-byte alignment */
#define ALIGNMENT 16

/* rounds up to the nearest multiple of ALIGNMENT */
#define ALIGN(size) (((size) + (ALIGNMENT-1)) & ~(ALIGNMENT-1))

/* rounds up to the nearest multiple of mem_pagesize() */
#define PAGE_ALIGN(size) (((size) + (mem_pagesize()-1)) & ~(mem_pagesize()-1))

// Macro for block overhead
#define OVERHEAD (sizeof(block_header) + sizeof(block_footer))

// Macro for getting the header from a payload pointer
// Gets the start of the block
// bp - block payload
#define HDRP(bp) ((char *)(bp) - sizeof(block_header))

// Macros for working with a raw pointer as the header
// Gets the size of block
#define GET_SIZE(p) ((block_header *)(p))->size

// Gets the size of block from the block footer.
#define GET_SIZE_FOOTER(p) ((block_footer *)(p))->size 

// Is the block allocated or no ?
#define GET_ALLOC(p) ((block_header *)(p))->allocated

// Get the next block payload
// Doubtful about this.
#define NEXT_BLOCKP(bp) ((char *)(bp) + GET_SIZE(bp))

// Defining the block header
typedef struct
{
  size_t size;
  char allocated; 
}block_header;

typedef struct
{
  size_t size;
  int filler; // What is this used for ? 
}block_footer;

void *current_avail = NULL;
size_t current_avail_size = 0;

// This is to clear the bytes
size_t original_size = 0;


int mm_init(void)
{
   current_avail_size = mem_pagesize();
   original_size = current_avail_size;
   first_block = mem_map(current_avail_size);
  
   void *epilogue_marking = first_block + current_avail_size;

   // We are subtracting the OVERHEAD because of the addition of epilogue
   GET_SIZE(first_block)  = current_avail_size - OVERHEAD;
   GET_ALLOC(first_block) = 0;

   GET_SIZE(epilogue_marking) = 0;
   GET_ALLOC(epilogue_marking) = 1;

   return 0;
}

void *mm_malloc(size_t size)
{
  size_t new_size = ALIGN(size + OVERHEAD);
  void *latest_bp;
  void *current_block = first_block;
  
  while(GET_SIZE(current_block)!=0)
    {
      if(!GET_ALLOC(current_block) && GET_SIZE(current_block)>=new_size)
      {
        set_allocated(current_block, new_size);
        current_block = current_block + OVERHEAD; 
	      return current_block;
      }
      current_block = NEXT_BLOCKP(current_block);
    }

  // We have to only extend if we are out of memory
  latest_bp = extend(new_size,current_block);
  set_allocated(latest_bp, new_size);
  return latest_bp;
}

void *extend(size_t size, void *current_block)
{
  size_t chunk_size =  ALIGN(size); // Blocks to be assigned
  void *new_block = current_block + chunk_size;
  
  GET_SIZE(current_block) = chunk_size;
  GET_ALLOC(current_block)  = 0;
  
  GET_SIZE(new_block) = 0;
  GET_ALLOC(new_block) = 1;
  
  return current_block;
}

void set_allocated(void *bp, size_t size)
{
  size_t extra_size = GET_SIZE(bp) - size;
 
  if(extra_size > ALIGN(1 + OVERHEAD))
    {
      GET_SIZE(bp) = size;
      GET_SIZE(NEXT_BLOCKP(bp)) = extra_size;
      GET_ALLOC(NEXT_BLOCKP(bp)) = 0;
    }

  GET_ALLOC(bp) = 1;
}


/*
 * mm_free - Freeing a block does nothing.
 */
void mm_free(void *ptr)
{
  mem_unmap(HDRP(ptr),original_size);
}
