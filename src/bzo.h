#ifndef _BZO_RUNTIME_HEADER
#define _BZO_RUNTIME_HEADER


#include "taskunit.h"










//Initialize Bzo Runtime Environment
extern BzoStatus bzoInit(BzoEnvironment*, int);

//Run a BzoTask in an initialized environment.
extern BzoStatus bzoRunTask(BzoEnvironment*, BzoTask);

//Flush the contents of an initialized environment.
extern BzoStatus bzoFlush(BzoEnvironment*);

//Make BzoTask
extern BzoTask bzoMakeTask(void(void*), void*, void(void*), void*);

//Internal Function : Halt Bzo System
extern void bzoHalt(BzoTaskUnit*, uint64_t, void*);

//Internal Function : Allocate Memory
extern void* bzoAlloc(BzoTaskUnit*, int);

//Internal Function : Free Memory
extern void bzoFree(void*);

//Internal Function : Spawn Thread
extern void bzoSpawn(BzoTaskUnit*, BzoTask);


void computeSize(int, int*, int*);


#endif
