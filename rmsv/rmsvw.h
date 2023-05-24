#ifndef _RMSVW_H_
#define _RMSVW_H_

#include <icore.h>
#include <gcore.h>

#include "crypto/libc.h"

typedef struct _heap_node {
	struct _heap_node	*next;
	uint32_t			top;
	uint32_t			max;
	uint32_t			size;			/* total size in bytes including this header */
} heap_node;

typedef struct _heaps {
	heap_node		  *fblock;
	uint32_t		   bsize;
} heaps;

void  heap_init(heaps *heap, uint32_t bsize) ;
int   heap_block(heaps *heap, uint32_t addr, uint32_t size) ;
void *heap_alloc(heaps *heap, uint32_t size) ;
int heap_free(heaps *heap, void *ptr) ;


#endif
