#include "rmsvw.h"

heaps heap;
uint32_t cinit(uint32_t addr)
{
	heap.fblock = 0;
	heap.bsize = addr;
}

/*
void  heap_init(heaps *heap, uint32_t bsize) 
{
	heap->fblock = 0;
	heap->bsize = bsize;
}
int   heap_block(heaps *heap, uint32_t addr, uint32_t size) 
{
	heap_node     		*b;
	uint32_t				x;
	uint32_t				*stack;
	uint32_t				stacksz;
	uint32_t				slotres;
	
	b = (heap_node*)addr;
	b->next = heap->fblock;
	heap->fblock = b;
	
	b->size = size;
	
	size = size - sizeof(heap_node);
	
	b->max = size / (heap->bsize);
	
	stacksz = b->max * 4;
	slotres = (stacksz / heap->bsize) * heap->bsize < stacksz ? (stacksz / heap->bsize) + 1 : stacksz / heap->bsize;
	
	b->top = slotres;
	
	stack = (uint32_t*)&b[1];
	for (x = slotres; x < b->max; ++x) {
		stack[x] = x * heap->bsize;
	}
		
	return 1;
}
void *heap_alloc(heaps *heap, uint32_t size) 
{
	heap_node		*b= heap->fblock;
	uint32_t		ptr;
	uint32_t		*stack;
	

	if (size > heap->bsize) {
		//fatal("stack memory", __FILE__, __LINE__, 0);
		return 0;
	}
	
	for (b = heap->fblock; b; b = b->next) {
		if (b->top != b->max) {
			stack = (uint32_t*)&b[1];
			ptr = stack[b->top++];
			ptr = (uint32_t)&b[1] + ptr;
            //if(debug){
            printf("allocated 1x 1kb block\n");
           // }
			return (void*)ptr;
		}else{
			//fatal("stack memory ", __FILE__, __LINE__, 0);
		}
	}
	
	//fatal("stack memory ", __FILE__, __LINE__, 0);
	
	return 0;
}
int heap_free(heaps *heap, void *ptr) 
{
	heap_node    		*b;
	uint32_t			_ptr;
	uint32_t			*stack;
	
	
	_ptr = (uint32_t)ptr;
	for (b = heap->fblock; b; b = b->next) {
		if (_ptr > (uint32_t)b && _ptr < ((uint32_t)b + b->size))
			break;
	}
	
	
	if (!b)
		return 0;
	
	
	stack = (uint32_t*)&b[1];
	
	

	stack[--b->top] = _ptr - (uint32_t)&b[1];
	return 1;	
}
*/

void* cmalloc(uint32_t s)
{
	void *ptr = NULL; //heap_alloc(&heap,s);
	if(ptr == NULL) ptr= (void *) malloc(s);
	return ptr;
}
void * crealloc (void *ptr, uint32_t size)
{
	void *str = NULL; //heap_block(&heap, (uint32_t)ptr, size);
	if(str == NULL) str = (void *) realloc(ptr, size);
	return str;
}
void cfree(void* addr)
{
	int status = 0;//heap_free(&heap,addr);
	if(!status) free(addr);
    return;
}

