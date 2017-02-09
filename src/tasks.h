#ifndef _TASK_HEADER
#define _TASK_HEADER


#include "stdint.h"










//Builtin Support for Higher-Order Functions
//This reduces complexity of writing parallel code.
enum BzoHOFn{
  BzHO_MAP,
  BzHO_REDUCE,
  BzHO_ESCAN,
  BzHO_ISCAN,
  BzHO_FILTER
};










typedef struct{
  void(*fnptr)(void*);  //Function Pointer
  void* param;          //Main Parameter(s)

  /*
    Higher-Order Function Specifications

    hoType    : Type of Higher-Order Function
    itemNum   : Number of items to run the task across. If == 1, the task is run as a normal function.
    parStride : If the task is run as a Higher-Order function, param is treated as a pointer to an
                  array of inputs. parStride is the size of each element in the array.
  */
  BzoHOFn hoType;
  int itemNum;
  int parStride;

  //NUMA-centered
  uint32_t threadReq;   //NUMA specific
  uint64_t pars[4];     //Still supported for SMP, but not that useful for SMP.
}BzoTask;










typedef struct{
  int top, bottom;
  BzoTask tasks [8];
  void* sibling;
}TaskQueue;










typedef struct{
  TaskQueue* tQ;    //Topological Queues
  TaskQueue  iQ;    //Internal Queue
  TaskQueue* hQ;    //Higher-Order Queues

  int tqSize;       //Number of Topological Queues
  int hqSize;       //Number of Higher-Order Queues


  void* runtimePtr; //Pointer to the runtime.
}BzoTaskUnit;










void runTaskUnit(BzoTaskUnit*);


#endif
