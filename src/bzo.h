#ifndef _BZO_RUNTIME_HEADER
#define _BZO_RUNTIME_HEADER


#include "tasks.h"










//Initialize Bzo Runtime Environment
BzoStatus bzoInit(BzoRuntime*, int);

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
