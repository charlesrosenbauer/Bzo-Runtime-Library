#ifndef _BZO_TYPES_HEADER_
#define _BZO_TYPES_HEADER_










//Builtin Support for Higher-Order Functions
//This reduces complexity of writing parallel code.
typedef enum{
  BzHO_MAP,
  BzHO_REDUCE,
  BzHO_ESCAN,
  BzHO_ISCAN,
  BzHO_FILTER
}BzoHOFn;










//Stores constraints for Task Generation
typedef struct{
  uint64_t pars[4];
  uint32_t coreReq;
}BzoConstraint;










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










//Task queues can hold N-1 items, where N is the number of elements in the tasks array.
typedef struct{
  int top, bottom;
  BzoTask tasks [16];
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










typedef struct{
  BzoTaskUnit* taskUnits;

  int threadNum;
  int sysStatus;

  void* returnState;
}BzoRuntime;










//Error Reporting
typedef enum{
  BZO_SUCCESS = 0,                  //Everything went smoothly!
  BZO_ERR_RUNTIME_BUSY = 1,         //Runtime needs to be flushed
  BZO_ERR_RUNTIME_UNITIALIZED = 2,  //Runtime needs to be initialized
  BZO_ERR_INV_PTR = 3,              //A Pointer Parameter is Invalid
  BZO_ERR_INV_THD = 4               //Invalid Thread Number
}BzoStatus;
















#endif
