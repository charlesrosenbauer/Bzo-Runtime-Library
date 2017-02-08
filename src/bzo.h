#ifndef _BZO_RUNTIME_HEADER
#define _BZO_RUNTIME_HEADER


#include "tasks.h"










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
  BZO_ERR_INV_PTR = 3               //A Pointer Parameter is Invalid
  BZO_ERR_INV_THD = 4;              //Invalid Thread Number
}BzoStatus;










//Initialize Bzo Runtime Environment
BzoStatus bzoInit(int, BzoRuntime*);

//Run a BzoTask in an initialized environment.
BzoStatus bzoRunTask(BzoRuntime*, BzoTask);

//Flush the contents of an initialized environment.
BzoStatus bzoFlush(BzoRuntime*);

//Make BzoTask
BzoTask bzoMakeTask(void*, void(void*), BzoConstraint);

//Make Higher Order BzoTask
BzoTask bzoMakeHigherOrderTask(void*, void(void*), BzoConstraint, int, BzoHOFn);

//Make BzoConstraint to add 4 extra parameters
void bzoSetConstraintPars(BzoConstraint*, uint64_t, uint64_t, uint64_t, uint64_t);

//Make BzoConstraint with thread ID requirement (for NUMA).
//This can be ignored in SMP architectures.
void bzoSetConstraintThreadID(BzoConstraint*, uint32_t);


//Internal Function : Halt Bzo System
void bzoHalt(BzoRuntime*, int, void*);

//Internal Function : Allocate Memory
void* bzoAlloc(BzoTaskUnit*, int);

//Internal Function : Free Memory
void bzoFree(void*);

//Internal Function : Spawn Thread
void bzoSpawn(BzoTaskUnit*, BzoTask);





#endif
