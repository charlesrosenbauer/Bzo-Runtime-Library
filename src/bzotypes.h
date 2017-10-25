#ifndef __BZO_TYPE_HEADER__
#define __BZO_TYPE_HEADER__


#include "stdint.h"










//Add more parameters as needed
typedef struct{
  void (*fptr)(void*, void*);   // First parameter is the current Task Unit, second is data.
  void*        data;
}BzoTask;










typedef struct{
  BzoTask data[32];
  int8_t top, end;
  void* sibling;
}BzoTaskQueue;










typedef struct{
  BzoTaskQueue neighbors[4];
  BzoTask data[1024]; // Local queue
  int base, size;     // Local queue base and size
  void* environment;
  int isActive;       //1 means the unit is active
  int pushIndex;
  int id;
}BzoTaskUnit;










typedef struct{
  BzoTaskUnit* grid;
  int h, w, count;
  int globalEncourage;    // 1 means to spawn threads opportunistically
  int globalHeuristic;    // 1 means that cores should pay closer attention to the distribution of threads
  uint64_t globalState;
  void* globalReturn;
}BzoEnvironment;










typedef enum{
  BZ_SUCCESS = 0,
  BZ_FAIL    = 1
}BzoStatus;













#endif
