#include <stdlib.h>
#include <stdint.h>
#include "fastalloc.h"










// When the default allocator is implemented, this will have to be adjusted to
// use it to spawn the data.
FastAllocator* makeFalloc(){
	void* data = malloc(16384);	// Allocate 16kB block
	FastAllocator* ret = (FastAllocator*) data;
	ret->data = ret + (sizeof(FastAllocator));
	ret->limit = ret + 16384;
	ret->nextChain = NULL;
	ret->head = ret;
	ret->top = ret;
	ret->size = 0;
	return ret;
}










// Adjust when default allocator is implemented
void freeFalloc(FastAllocator* a){
	FastAllocator* tmp = a;
	while(tmp != NULL){
		FastAllocator* current = tmp;
		tmp = tmp->nextChain;
		free(current);
	}
}










void fallocAppend(FastAllocator* h){
	if(h->top == h){
		h->top = (void*)makeFalloc();
		h->nextChain = h->top;
		FastAllocator* t0 = h->top;
		t0->head = h;
		return;
	}
	FastAllocator* top = h->top;
	h->top = makeFalloc();
	top->head = h;
}










void* fastAlloc(FastAllocator* a, uint8_t size, uint8_t align){
	if(a->top != a){
		//Allocate to the actual top
	}

	long base = ((long)a->data) + a->size;
	int alignVal0, alignVal1;
	if(align < 8)
		alignVal0 = (1 << align);
	else
		alignVal0 = 0x80;
	
	alignVal1 = alignVal0 - 1;
	long top  = base + size;
	if((top & alignVal1) != 0){
		top +=  alignVal0;
		top &= ~alignVal1;
	}
	
	if(base + top >= (long)a->limit){
		// Allocate next block
	}
	
}













