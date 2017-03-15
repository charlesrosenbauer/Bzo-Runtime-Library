#include "alloc.h"
#include <stdlib.h>










BzoAllocErr configMemPool(Bzo_MemPool* a, int stride, int size){
  a->block = malloc(size);
  if(a->block == NULL) return BZ_ALC_INSF;
  a->stride = stride;
  a->size = size;

  a->capacity = (a->size / stride);
  a->fill = 0;
  a->stackpt = 0;

  a->scanCtr = 0;
  a->lastScanPt = a->block;
  a->deletePt = NULL;
  a->delFlag = 0;

  return BZ_ALC_PASS;
}










BzoAllocErr configMemPoolPages(Bzo_MemPool* a, int stride, int pages){
  return configMemPool(a, stride, 2097152 * pages);    //2MB blocks
}










void cleanMemPool(Bzo_MemPool* a){
  a->scanCtr = 0;
  uint64_t* maximum = a->block + a->size;
  int ct = 0;
  for(uint64_t* it = a->lastScanPt; it < maximum; it += a->stride){

    if(ct == 256){
      // Let's stop cleanup for now, and pick up where we left off later
      a->lastScanPt = it;
      return;
    }

    if((*it != 0) && (*it != (uint64_t)a)){
      // Clean up time!

    }
    ct++;
  }

  // Start from the beginning next time!
  a->lastScanPt = a->block;
}










void* bzo_palloc(Bzo_MemPool* a, BzoAllocErr* err){
  if(a->fill < a->capacity){
    //There is some space available somewhere!
    if(a->stackpt < a->capacity){
      //Let's add to the stack!
    }
  }

}
