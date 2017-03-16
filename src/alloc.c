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
  a->lastDel = NULL;
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

    // Clean up time?
    if((*it != 0) && (*it != (uint64_t)a)){
      //Is there a valid delete point?
      if(a->deletePt == NULL)
        a->deletePt = it;
      else if(*(a->lastDel) != (uint64_t)a){
        *(a->lastDel) = (uint64_t)it;
        a->lastDel = it;
      }
      a->fill--;
    }

    ct++;
  }

  // Start from the beginning next time!
  a->lastScanPt = a->block;
}










void* bzo_palloc(Bzo_MemPool* a, int runFast){

  //Every so often, run cleanup.
  if(a->delFlag){
    a->scanCtr++;
    if(a->scanCtr >= 512){
      a->scanCtr = 0;
      cleanMemPool(a);
    }
  }

  if(a->fill < a->capacity){
    //There is some space available somewhere!

    if((a->deletePt != NULL) && (*(a->deletePt) != 0) && (*(a->deletePt) != (uint64_t)a)){
      //deletePt is valid!
      void* ptr = (void*)(a->deletePt);
      a->deletePt = (uint64_t*)*(a->deletePt);
      return ptr;
    }else if(a->stackpt < a->size){
      //Let's add to the stack!
      void* ptr = (void*)(a->block + a->stackpt);
      a->stackpt += a->stride;
      return ptr;
    }else if(!runFast){
      //Run linear scan to find the elusive empty space


    }
  }
  return NULL;
}
