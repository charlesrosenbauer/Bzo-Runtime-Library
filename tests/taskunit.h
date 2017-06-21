#include "stdint.h"










//Add more parameters as needed
typedef struct{
  void*(*fptr)(void*);
  void*        data;
}Task;










typedef struct{
  Task data[32];
  int8_t top, end;
  void* sibling;
}TaskQueue;










typedef struct{
  TaskQueue nieghbors[4];
  Task data[1024];    // Local queue
  int base, size;     // Local queue base and size
  void* environment;
}TaskUnit;










typedef struct{
  TaskUnit* grid;
  int h, w, count;
  uint64_t globalState;
  void* globalReturn;
}BzoEnvironment;
