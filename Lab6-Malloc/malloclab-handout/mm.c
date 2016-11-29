/*
  * Author: Paarth Lakhani
  * uid: u0936913
  *
  * Description:
  I almost got the 80 % mark for the malloc implementation. But I could not achieve it because of 
  segmentation fault and potentail errors in the code. I was trying to unmap the functions.
  However, I was not able to do that. 

  The malloc tactics that I have used in this implementation are:
  coalescing, first-fit. I got the unmapping 90 %. I have spent hours trying to debug that 
  part of the code but no achievement.

Potential mm_free function:

void mm_free(void *ptr)
{
  //mem_unmap(HDRP(ptr),original_size);
  GET_ALLOC(HDRP(ptr)) = 0;
  ptr = HDRP(ptr);
  ptr = coalesce(ptr); // after this step ptr is at the payload of the address
  size_t latest_size = GET_SIZE(HDRP(ptr)) + OVERHEAD + OVERHEAD_HEADER; <--- Some error starts here
  void *page_size = return_page_size_pointers(ptr) - OVERHEAD_HEADER;
  size_t original_size = ACTUAL_SIZE(page_size);
  if(latest_size == original_size)
  { 
    page_size = page_size - PAGE_HEADER_SIZE;
    first_block = remove_page(page_size);
    first_block = START_PAYLOAD(first_block);
    mem_unmap(page_size,original_size);
  }
}

The functions are all ready.

Potential improvements that I had planned:
Put and packing
best-first
unmap working Done
explicit free list
 */
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <unistd.h>
#include <string.h>

#include "mm.h"
#include "memlib.h" 

void *first_block;
void *current_block;
void *page_pointer;
void *epilogue_marking;
void *current_block_copy;
void *return_pointer_1;
void *new_page_pointer;
void *ptr2;
void* new_root;
void *page_size;

/* always use 16-byte alignment */
#define ALIGNMENT 16

/* rounds up to the nearest multiple of ALIGNMENT */
#define ALIGN(size) (((size) + (ALIGNMENT-1)) & ~(ALIGNMENT-1))

/* rounds up to the nearest multiple of mem_pagesize() */
#define PAGE_ALIGN(size) (((size) + (mem_pagesize()-1)) & ~(mem_pagesize()-1))

// Macro for block_header
#define OVERHEAD_HEADER (sizeof(block_header))

// Macro for block_footer
#define OVERHEAD_FOOTER (sizeof(block_footer))

// Total overhead
#define OVERHEAD (OVERHEAD_HEADER + OVERHEAD_HEADER + PAGE_SIZE + PAGE_HEADER_SIZE)

// Macro for getting the header from a payload pointer
// Gets the start of the block
// bp - block payload
#define HDRP(bp) ((char *)(bp) - sizeof(block_header))

// Macro for getting the payload from the block header
#define HDRP_NEXT(bp) ((char *)(bp) + sizeof(block_header))

// Macros for working with a raw pointer as the header
// Gets the size of block
#define GET_SIZE(p) ((block_header *)(p))->size

// Is the block allocated or no ?
#define GET_ALLOC(p) ((block_header *)(p))->allocated

// Get the next block payload
// Doubtful about this.
#define NEXT_BLOCKP(bp) ((char *)(bp) + GET_SIZE(bp))

// Macro for getting the previous header
#define PREV_BLK(bp) ((char *)(bp) - GET_SIZE((char *)bp - OVERHEAD_HEADER))

// Macro for getting to the next block
#define FTRP(bp) ((char *)(bp) + GET_SIZE(bp) - OVERHEAD_HEADER)

// Gets the size of the page header
#define PAGE_HEADER_SIZE (sizeof(page_header))

// Gets the prev_page pointer
#define PREV_PAGE(bp) ((page_header *)(bp))->prev_page

// Gets the next_page pointer
#define NEXT_PAGE(bp) ((page_header *)(bp))->next_page

// Gets the size pf page header
#define PAGE_SIZE (sizeof(page))

// Gets the actual size of page
#define ACTUAL_SIZE(bp) ((page *)(bp))->page_size

// Given the starting payload of the page, go to the start of the page
#define START_PAGE(bp) ((char *)(bp) - OVERHEAD_HEADER  - OVERHEAD_HEADER - PAGE_SIZE - PAGE_HEADER_SIZE)

// Start payload
#define START_PAYLOAD(bp) ((char *)(bp) + OVERHEAD_HEADER + OVERHEAD_HEADER + PAGE_SIZE + PAGE_HEADER_SIZE)

// Defining the block header
typedef struct
{
  size_t size;
  char allocated;
}block_header;

// Defining the block footer
typedef struct
{
  size_t size;
  int filler; 
}block_footer;

typedef struct
{
  size_t page_size;
  int filler; 
}page;

// Page header struct
typedef struct
{
  void* next_page; 
  void* prev_page;
}page_header;

void *current_avail = NULL;
size_t current_avail_size = 0;

// This is to clear the bytes
size_t original_size = 0;

int mm_init(void)
{
  first_block = page_layout(2*mem_pagesize());
  return 0;
}

// Initializes the page with the page headers
// given the size of the page, initializes it and gets the all the overhead
void* page_layout(size_t page_size)
{
 // void* page_pointer = mem_map(page_size);
  page_pointer = mem_map(page_size);
  current_avail_size = page_size;
   
  //void *epilogue_marking = (char *)page_pointer + current_avail_size;
  epilogue_marking = (char *)page_pointer + current_avail_size - OVERHEAD_HEADER;

  NEXT_PAGE(page_pointer) = NULL;  // next pointer
  PREV_PAGE(page_pointer) = NULL; // previous page

  page_pointer = (char *)page_pointer + PAGE_HEADER_SIZE;
  ACTUAL_SIZE(page_pointer) = current_avail_size;

  page_pointer = (char *)page_pointer + PAGE_SIZE;

   // Addition of the prologue
  GET_SIZE(page_pointer) = 32;
  GET_ALLOC(page_pointer) = 1;
  GET_SIZE(FTRP(page_pointer)) = 32; // Doubtful about this line

   // Adding for the header and the footer of the prologue
  page_pointer = (char *)page_pointer + OVERHEAD_HEADER + OVERHEAD_HEADER;

   // We are subtracting the OVERHEAD because of the addition of epilogue
  size_t subtract_overhead = (OVERHEAD_HEADER + OVERHEAD_HEADER + OVERHEAD_HEADER + PAGE_SIZE + PAGE_HEADER_SIZE);
  GET_SIZE(page_pointer)  = current_avail_size - subtract_overhead;
  GET_SIZE(FTRP(page_pointer)) = current_avail_size - subtract_overhead;
  GET_ALLOC(page_pointer) = 0;

  GET_SIZE(epilogue_marking) = 0;
  GET_ALLOC(epilogue_marking) = 1;
  
  return (page_pointer);
}

void *mm_malloc(size_t size)
{
  
  size_t new_size = ALIGN(size + OVERHEAD_HEADER + OVERHEAD_HEADER);
  //void *current_block = (first_block);
  current_block = (first_block);
  current_block = START_PAGE(current_block);

  void *pointer_check = (first_block) - OVERHEAD;

  while(1)
  {
    current_block = START_PAYLOAD(current_block);
    //void *current_block_copy = current_block;
    current_block_copy = current_block;
    while(GET_SIZE((current_block))!=0)
      {
        if(!GET_ALLOC((current_block)) && GET_SIZE((current_block))>=new_size)
        {
          set_allocated(current_block, new_size);
          current_block = current_block + OVERHEAD_HEADER ;
          return current_block;
        }
        current_block = NEXT_BLOCKP((current_block));
      }
      current_block = (START_PAGE(current_block_copy));
      if(NEXT_PAGE(current_block)!=NULL)
        current_block = NEXT_PAGE(current_block);
      else
      {
        // At this point, there is no next page.
        // updating the current_block to point to the start of the page.
        break;
      }
  }

  pointer_check = (first_block) - OVERHEAD;
    
  // We have to only extend if we are out of memory
  /*size_t add_overhead = (OVERHEAD_HEADER + OVERHEAD_HEADER + OVERHEAD_HEADER + PAGE_SIZE + PAGE_HEADER_SIZE);
  size_t extend_page_size = PAGE_ALIGN(new_size + add_overhead);
  new_page_pointer = extend(extend_page_size,current_block);
  
  // This is the new page. Attach this page to the beginning.
  first_block = add_page_list((char *)new_page_pointer - OVERHEAD, (char *)first_block - OVERHEAD);
  set_allocated((char *)first_block + OVERHEAD, new_size);
  first_block = (char *)first_block + OVERHEAD ;
  void *return_pointer_1 = (char *)first_block + OVERHEAD_HEADER;
  return return_pointer_1;*/

  // At this point first_block is at the start of the payload of the last page.
  //first_block = START_PAGE(first_block);

  size_t add_overhead = (OVERHEAD_HEADER + OVERHEAD_HEADER + OVERHEAD_HEADER + PAGE_SIZE + PAGE_HEADER_SIZE);
  size_t extend_page_size = PAGE_ALIGN(new_size + add_overhead);
  new_page_pointer = extend(extend_page_size);
  
  // This is the new page. Attach this page to the beginning.
  first_block = add_page_list((char *)new_page_pointer - OVERHEAD, (char *)first_block - OVERHEAD);

  // At this point there is in consistency in the linked list.
  // check here for consistency in the page linked list

  first_block = (char *)first_block + OVERHEAD ;
  //void *return_pointer_1 = first_block;
  return_pointer_1 = first_block; 
  set_allocated(return_pointer_1, new_size);
  return_pointer_1 = (char *)return_pointer_1 + OVERHEAD_HEADER;

  // here also
  // check here for consistency in the page linked list
  return return_pointer_1;
}

void *extend(size_t size)
{
  //void* new_page_pointer = page_layout(size);
  new_page_pointer = page_layout(size);
  return new_page_pointer;
}

void set_allocated(void *bp, size_t size)
{
  size_t extra_size = GET_SIZE(bp) - size;
 
  if(extra_size > ALIGN(1 + OVERHEAD_HEADER + OVERHEAD_HEADER))
    {
      GET_SIZE(bp) = size;
      GET_SIZE(FTRP(bp)) = size;
      GET_SIZE(NEXT_BLOCKP(bp)) = extra_size;
      GET_SIZE(FTRP(NEXT_BLOCKP(bp))) = extra_size;
      GET_ALLOC(NEXT_BLOCKP(bp)) = 0;
    }

  GET_ALLOC(bp) = 1;
}

void *coalesce(void *bp)
{
  size_t prev_alloc = GET_ALLOC(PREV_BLK(bp));
  size_t next_alloc = GET_ALLOC(NEXT_BLOCKP(bp));
  size_t size = GET_SIZE(bp);

  if(prev_alloc && next_alloc) // Do nothing
  {

  }
  else if(prev_alloc && !next_alloc) 
  {
    size+= GET_SIZE(NEXT_BLOCKP(bp));
    GET_SIZE(bp) = size;
    GET_SIZE(FTRP(bp)) = size;
  }
  else if(!prev_alloc && next_alloc) 
  {
    size+= GET_SIZE(PREV_BLK(bp));
    GET_SIZE(PREV_BLK(bp)) = size;
    GET_SIZE(FTRP(bp)) = size;
    bp = PREV_BLK(bp);
  }
  else
  {
    size+=(GET_SIZE(PREV_BLK(bp)))+ (GET_SIZE(NEXT_BLOCKP(bp)));
    GET_SIZE(PREV_BLK(bp)) = size;
    GET_SIZE(FTRP(NEXT_BLOCKP(bp))) = size;
    bp = PREV_BLK(bp);
  }

  bp = HDRP_NEXT(bp);
  return bp;
}

/*
 * mm_free - Freeing a block does nothing.
 */
void mm_free(void *ptr)
{
  //mem_unmap(HDRP(ptr),original_size);
  GET_ALLOC(HDRP(ptr)) = 0;

  ptr = HDRP(ptr);
  ptr = coalesce(ptr); // after this step ptr is at the payload of the address
  size_t latest_size = GET_SIZE(HDRP(ptr)) + OVERHEAD + OVERHEAD_HEADER;
  //void *page_size = return_page_size_pointers(ptr) - OVERHEAD_HEADER;
  page_size = return_page_size_pointers(ptr) - OVERHEAD_HEADER;
  size_t actual_size = ACTUAL_SIZE(page_size);
  

  if(latest_size == actual_size)
  { 
    page_size = (void *)page_size - PAGE_HEADER_SIZE;

    if(PREV_PAGE(page_size)!=NULL || NEXT_PAGE(page_size)!=NULL)
    {
      first_block = remove_page(page_size);
      first_block = START_PAYLOAD(first_block);
      mem_unmap(page_size,actual_size);
    }
  }
}

void *return_page_size_pointers(void *ptr)
{
  // ptr now points to the block header
  //void *ptr2 = HDRP(ptr);
  ptr2 = HDRP(ptr);

  while(GET_SIZE(ptr2) != (OVERHEAD_HEADER+OVERHEAD_HEADER))
  {
    ptr2 = PREV_BLK(ptr2);
  }
  return (void *)ptr2;
}

// Add the page at the beginning
void* add_page_list(void *new_page, void *root)
{
//  PREV_PAGE(new_page) = NULL;
  NEXT_PAGE(new_page) = root;
  PREV_PAGE(root) = new_page; 
  return new_page;
}

// Remove the page from the linked list
// It returns the root node pointer. If the root node pointer points
// to the start of the linked list, then root node pointer has been changed.
// In all the other cases, the root node pointer is the same.
void* remove_page(void *node_removal)
{
  //void* new_root = START_PAGE(first_block);
  new_root = START_PAGE(first_block);

  if(PREV_PAGE(node_removal) == NULL && NEXT_PAGE(node_removal) == NULL) // only one page
  {
    return new_root;
  }
  else if(PREV_PAGE(node_removal) == NULL) // The first node
  {
    new_root = NEXT_PAGE(node_removal); // The first block points here.
    PREV_PAGE(NEXT_PAGE(node_removal)) = NULL;
  }
  else if(NEXT_PAGE(node_removal) == NULL) // The last node 
  {
    NEXT_PAGE(PREV_PAGE(node_removal)) = NEXT_PAGE(node_removal);
  }
  else // middle node
  {
    NEXT_PAGE(PREV_PAGE(node_removal)) = NEXT_PAGE(node_removal);
    PREV_PAGE(NEXT_PAGE(node_removal)) = PREV_PAGE(node_removal);
  }
  return new_root;
}
