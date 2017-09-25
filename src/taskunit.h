#ifndef __TASKUNIT_HEADER__
#define __TASKUNIT_HEADER__


#include "bzotypes.h"










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
