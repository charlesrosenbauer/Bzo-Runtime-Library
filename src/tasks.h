#ifndef _TASK_HEADER
#define _TASK_HEADER


#include "stdint.h"
#include "stdlib.h"
#include "bzotypes.h"










int popTask  (TaskQueue*, BzoTask*);
int pushTask (TaskQueue*, BzoTask );
int queueSize(TaskQueue*);
void runTaskUnit(BzoTaskUnit*);



#endif
