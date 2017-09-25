#ifndef __TASKUNIT_HEADER__
#define __TASKUNIT_HEADER__

#include "stdint.h"










//Add more parameters as needed
typedef struct{
  void*(*fptr)(void*);
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
}BzoTaskUnit;










typedef struct{
  BzoTaskUnit* grid;
  int h, w, count;
  uint64_t globalState;
  void* globalReturn;
}BzoEnvironment;










typedef enum{
  BZ_SUCCESS = 0,
  BZ_FAIL    = 1
}BzoStatus;










void spawnTasks(BzoTaskUnit*, BzoTask*, int);
int pushPrivateTasks(BzoTaskUnit*, BzoTask*, int);
int pushTasks(BzoTaskQueue*, BzoTask*, int);
int pullTasks(BzoTaskQueue*, BzoTask*, int);
int pushTask(BzoTaskQueue*, BzoTask);
int popTask(BzoTaskQueue*, BzoTask*);
int queueSize(BzoTaskQueue*);
BzoStatus initEnvironment(BzoEnvironment*, int);
BzoStatus runBzoTask(BzoEnvironment*, BzoTask, uint64_t*, void**);

#endif
