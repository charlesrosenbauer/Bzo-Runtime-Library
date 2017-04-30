#ifndef _BZO_RUNTIME_HEADER
#define _BZO_RUNTIME_HEADER


#include "tasks.h"










//Initialize Bzo Runtime Environment
extern BzoStatus bzoInit(BzoRuntime*, int);

//Run a BzoTask in an initialized environment.
extern BzoStatus bzoRunTask(BzoRuntime*, BzoTask);

//Flush the contents of an initialized environment.
extern BzoStatus bzoFlush(BzoRuntime*);

//Make BzoTask
extern BzoTask bzoMakeTask(void(void*), void*, void(void*), void*);

//Internal Function : Halt Bzo System
extern void bzoHalt(BzoRuntime*, int, void*);

//Internal Function : Allocate Memory
extern void* bzoAlloc(BzoTaskUnit*, int);

//Internal Function : Free Memory
extern void bzoFree(void*);

//Internal Function : Spawn Thread
extern void bzoSpawn(BzoTaskUnit*, BzoTask);





#endif
