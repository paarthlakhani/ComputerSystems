#include <stdio.h>

extern int mm_init (void);
extern void *mm_malloc (size_t size);
extern void mm_free (void *ptr);
extern void *extend(size_t size, void *current_block);
extern void set_allocated(void *bp, size_t size);
extern void *coalesce(void *bp);
