#include <stdint.h>










typedef struct{
	void* data;
	void* nextChain;	// FastAllocator*
	void* head;		// FastAllocator*
	void* top;		// FastAllocator*
	int size;
	void* limit;
}FastAllocator;










FastAllocator* makeFalloc();
void freeFalloc(FastAllocator*);
void fallocAppend(FastAllocator*);
void* fastAlloc(FastAllocator*, uint8_t, uint8_t);
