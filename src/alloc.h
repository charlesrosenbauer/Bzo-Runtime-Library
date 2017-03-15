#ifndef _BZO_ALLOCATOR_HEADER_
#define _BZO_ALLOCATOR_HEADER_


#include <stdint.h>










typedef struct{
  // These first few parameters are for managing the memory owned by the
  // MemPool. The pointer of course is for the memory block, size stores the
  // size of the memory block, and stride stores the size of each object.
  uint64_t* block;
  int64_t stride;
  int64_t size;

  // Capacity stores the number of potential objects that can be stored. Fill
  // is the number currently in use (as of last update). Stackpt is used only
  // in early stages of the mempool, when very little of it has been used.
  // In this case, the mempool acts a bit like a stack, and stackpt determines
  // the top.
  int64_t capacity;
  int64_t fill;
  int64_t stackpt;

  // These variables are for managing deletions. Any thread can write to the
  // PWRITEs at any time, completely asynchronously. DeletePt stores a pointer
  // to any recently deleted object. Object tags can store previous values of
  // DeletePt to create a chain of deletions. In the case of a race condition,
  // chains may be lost, however they will inevitably be found again, as the
  // allocator runs occasional scans. DelFlag simply determines if any deletions
  // have occured since the last time the allocator checked. LastScanPt stores
  // a pointer to the last scanned object, and scanCtr is a counter to keep
  // track of when to perform the routine scans.
  uint64_t* lastScanPt;
  uint64_t* deletePt;   // PWRITE
  int16_t   scanCtr;
  uint8_t delFlag;      // PWRITE
}Bzo_MemPool;

typedef enum{
  BZ_ALC_PASS = 0,    //Allocation Success
  BZ_ALC_INSF = 1,    //Allocation Fail : Insufficient Memory
  BZ_ALC_FULL = 2     //Allocation Fail : Mem Pool is Full
}BzoAllocErr;










BzoAllocErr configMemPool(Bzo_MemPool*, int, int);
BzoAllocErr configMemPoolPages(Bzo_MemPool*, int, int);
void*    bzo_palloc(Bzo_MemPool*, BzoAllocErr*);



#endif
