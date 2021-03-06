#include "taskunit.h"
#include "threadmanager.hpp"
#include <stdlib.h>
#include <stdio.h>










int pushTask(BzoTaskQueue* q, BzoTask t){
  int next = (q->end + 1) % 32;
  if(next != q->top){
    q->data[q->end] = t;
    q->end = next;
    return 1; //Success!
  }
  return 0;   //Queue is full!
}










int popTask(BzoTaskQueue* q, BzoTask* t){
  if(q->top != q->end){
    *t = q->data[q->top];
    q->top = (q->top + 1) % 32;
    return 1; //Success!
  }
  return 0;   //Queue is empty!
}










int queueSize(BzoTaskQueue* q){
  return ((32 + q->top) - q->end) % 32;
}










void initTaskUnit0(BzoTaskUnit* t, void* env, int isActive){
  t->environment = env;
  t->base = 0;
  t->size = 0;
  t->pushIndex = 0;
  t->isActive = isActive;
}










void initTaskUnit1(BzoTaskUnit* t, BzoEnvironment* env, int id){

  int coords [4][2];
  coords[0][0] = -1; coords[0][1] =  0;
  coords[1][0] =  0; coords[1][1] =  1;
  coords[2][0] =  1; coords[2][1] =  0;
  coords[3][0] =  0; coords[3][1] = -1;

  int x = id % env->w;
  int y = id / env->w;

  t->id = id;

  for(int it = 0; it < 4; it++){

    int x0, y0, indexTo;
    x0 = x; y0 = y;
    do{
      x0 = x0 + coords[it][0];
      y0 = y0 + coords[it][1];
      x0 = (x0 + env->w) % env->w;
      y0 = (y0 + env->h) % env->h;
      indexTo = (y0 * env->w) + x0;
    }while(env->grid[indexTo].isActive != 1);

    // Link queues to corresponding taskunit.
    int linkMode = coords[it][0] + coords[it][1];
    int relIt    = (it + 4) % 4;  // Relative Iteration; what pipeline in the neighbor to connect to
    void* ptr    = &(t->neighbors[it]);
    env->grid[indexTo].neighbors[relIt].sibling = ptr;
    t->neighbors[it].top = 0;
    t->neighbors[it].end = 0;
  }

}










/*
  env  -> Environment to initialize
  tnum -> Maximum number of threads to run. -1 results in no maximum value.

  -- Add Later --
  prop -> Proportion of threads to use. For example, 0.5 means the system will only use 50% of threads available.
  tmax -> Maximum number of threads to run. -1 results in no maximum value.
  tmin -> Minimum number of threads to run, provided there are enough hardware threads.
*/
BzoStatus bzoInit(BzoEnvironment* env, int tnum){

  int sysThreads = getThreadNum();
  if(tnum == -1)
    tnum = sysThreads;
  tnum = (tnum < sysThreads)? tnum : sysThreads;

  {
    //Figure out a height and width for the taskunit grid, set them as h & w in env.
    env->count = tnum;
    int h = 1, w = 1;
    for(int i = 1; i < tnum; i++){
      int ct = h * w;
      if((ct - tnum) < 0) h++;
      else break;
      ct = h * w;
      if((ct - tnum) < 0) w++;
      else break;
    }
    env->h = h;
    env->w = w;
  }

  {
    env->globalState = 0;
    env->globalReturn = 0;
    env->globalEncourage = 0;
    env->grid = (BzoTaskUnit*)malloc(sizeof(BzoTaskUnit) * env->h * env->w);
    int ct = env->h * env->w;
    for(int it = 0; it < tnum; it++)
      initTaskUnit0(&env->grid[it], env, (it <= tnum)? 1 : 0);

    for(int it = 0; it < tnum; it++)
      initTaskUnit1(&env->grid[it], env, it);

  }

  return BZ_SUCCESS;
}










/*
  tq    -> Task Queue
  t     -> Pointer to Task Array
  tnum  -> Number of Tasks in Array
  <ret> -> Number of Tasks pushed

  This may later be good to optimize; pulling multiple Tasks at once without multiple calls to pushTask would likely run faster.
*/
int pushTasks(BzoTaskQueue* tq, BzoTask* t, int tnum){
  int ct = 0;
  for(int it = 0; it < tnum; it++)
    ct += pushTask(tq, t[it]);
  return ct;
}










/*
  tq    -> Task Queue
  t     -> Pointer to Task Array
  limit -> Maximum number of Tasks to pull
  <ret> -> Number of Tasks pulled

  This may later be good to optimize; pulling multiple Tasks at once without multiple calls to popTask would likely run faster.
*/
int pullTasks(BzoTaskQueue* tq, BzoTask* t, int limit){
  BzoTaskQueue* tx = (BzoTaskQueue*)tq->sibling;
  int ct = 0;
  for(int it = 0; it < limit; it++)
    ct += popTask(tx, &(t[it]));
  return ct;
}










/*
  tu    -> Task unit
  t     -> Pointer to Task Array
  tnum  -> Number of Tasks
  <ret> -> Number of Tasks Remaining. Will have to change this later.
*/
int pushPrivateTasks(BzoTaskUnit* tu, BzoTask* t, int tnum){
  if(tnum + tu->size >= 1024){
    int ct = 0;
    for(int it = tu->size; it < 1024; it++){
      tu->data[(it + tu->base) % 1024] = t[ct];
      ct++;
    }
    tu->size = 1024;
    return ct;
  }
  for(int it = 0; it < tnum; it++)
    tu->data[(it + tu->base + tu->size) % 1024] = t[it];
  tu->size += tnum;

  return 0;
}










int getLocalCapacity(BzoTaskUnit* tu) {
  int accum = 0;
  for (int i = 0; i < 4; i++)
    accum += queueSize(&tu->neighbors[i]);

  return accum;
}










void setEncourageVal(BzoEnvironment* env) {
  const float IDEALPASSING  = 0.4;
  const float IDEALCAPACITY = 0.3;
  const float IDEALSPREAD   = 0.3;

  float lowPass  = 0;
  float highPass = 0;
  int count = 0;
  float passCapacity = 0;
  for (int i = 0; i < env->count; i++){
    if (env->grid[i].isActive) {
      float capacity = ((float)getLocalCapacity(&env->grid[i])) / 128.0;
      passCapacity += capacity;
      count++;
      if (capacity > IDEALCAPACITY)
        highPass += capacity;
      else
        lowPass  += capacity;
    }
  }

  float diffPass = highPass - lowPass;
  diffPass = (diffPass > 0)? diffPass : -diffPass;

  env->globalHeuristic = (diffPass > IDEALSPREAD)? 1 : 0;
  env->globalEncourage = ((passCapacity / count) < IDEALPASSING)? 1 : 0;

}










/*
  tu   -> Task unit
  t    -> Pointer to Task Array
  tnum -> Number of Tasks

  Returns 0 on success, N on failure, where N is the number of tasks that failed to spawn.
*/
int spawnTasks(BzoTaskUnit* tu, BzoTask* t, int tnum){
  BzoEnvironment* env = (BzoEnvironment*)tu->environment;
  if(env->globalEncourage == 0) return tnum;

  // Add slow, smart mode (if globalHeuristic)

  // Fast, dumb mode
  int tcount = 0;
  int qIndex = 0;
  while((tcount < tnum) && (qIndex < 4)){
    if(pushTask(&tu->neighbors[qIndex], t[tcount]))
      tcount++;
    else
      qIndex++;
  }

  return (tcount = tnum);
}










int privateQueuePop(BzoTaskUnit* tu, BzoTask* task){
  if(tu->size == 0) return 0;   // FAILURE! Queue is empty!

  *task = tu->data[tu->base];
  tu->base = (tu->base + 1) % 1024;
  tu->size--;

  return 1; // Queue has more left!
}










int privateQueuePush(BzoTaskUnit* tu, BzoTask task){
  if(tu->size == 1024) return 0;    // FAILURE! Queue is full! This is not good!

  tu->size++;
  tu->data[(tu->base + tu->size) % 1024] = task;

  return 1; // Queue has more space!
}










void coreRuntime(BzoTaskUnit* tu){

  int counter = 0;
  int cyclesUnemployed = 0;

  BzoEnvironment* env = (BzoEnvironment*)tu->environment;
  while(env->globalState == 0){
    if(tu->id == 0){
      setEncourageVal(env);
    }
    if(cyclesUnemployed > 15){
      microSleep(250);
    }else if(cyclesUnemployed > 7){
      microSleep(50);
    }else if(cyclesUnemployed > 3){
      microSleep(5);
    }

    BzoTask task = {NULL, NULL};

    /*
      Should probably tweak this earlier to start in a random (or cycling) queue.
      Otherwise, the runtime will be biased toward some queues as opposed to others.
      This will lead to poor spread over the thread mesh.
    */
    if(privateQueuePop(tu, &task))
      goto startTask; // Jump out of the switch statement. Otherwise, continue.

    if(popTask(&tu->neighbors[0], &task))
      goto startTask; // Jump out of the switch statement. Otherwise, continue.

    if(popTask(&tu->neighbors[1], &task))
      goto startTask; // Jump out of the switch statement. Otherwise, continue.

    if(popTask(&tu->neighbors[2], &task))
      goto startTask; // Jump out of the switch statement. Otherwise, continue.

    if(popTask(&tu->neighbors[3], &task))
      goto startTask; // Jump out of the switch statement. Otherwise, continue.


    startTask:
    if(task.fptr != NULL){
      cyclesUnemployed = 0;
      task.fptr((void*)tu, task.data);
    }else{
      cyclesUnemployed++;
    }


    counter++;
  }
}










void bzoHalt(BzoTaskUnit* tu, uint64_t finishId, void* outputData){
  BzoEnvironment* env = (BzoEnvironment*)tu->environment;

  // A finishId of 0 is not an error.
  if (finishId != 0){
    env->globalState  = finishId;
    env->globalReturn = outputData;
  }

  // 1 is standard error
  if (finishId == 1)
    printf("Application Error: %s\n", (char*)outputData);

}
