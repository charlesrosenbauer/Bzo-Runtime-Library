#include "tasks.h"










int pushTask(TaskQueue* q, Task t){
  int next = (q->bottom + 1) % 16;
  if(next != q->top){
    q->tasks[q->bottom] = t;
    q->bottom = next;
    return 1; //Success!
  }
  return 0;   //Queue is full!
}










int popTask(TaskQueue* q, Task* t){
  if(q->top != q->bottom){
    *t = q->tasks[q->top];
    q->top = (q->top + 1) % 16;
    return 1; //Success!
  }
  return 0;   //Queue is empty!
}










int queueSize(TaskQueue* q){
  return ((16 + q->top) - q->bottom) % 16;
}










void runTaskUnit (BzoTaskUnit* tu){
  BzoRuntime* runtime = (BzoRuntime*)(tu->runtimePtr);

  BzoTask tsk;

  int queueMode = 0;
  int hqLimit = tu->hqSize + 1;
  int tqLimit = tu->tqSize + hqLimit;

  // Check if allowed to continue
  while(runtime->sysStatus == 0){
    if(queueMode == 0){
      //Load a task from internal queue
    }else if(queueMode <= tu->hqSize){
      //Load a task from a Higher-Order queue
      int hqueuemode = queueMode - 1;

    }else if(queueMode <= tqLimit){
      //Load a task from a Topological queue
      int tqueuemode = queueMode - (tu->hqSize + 1);

    }else{
      //No task is available. Sleep for a little bit.
    }

    // A task is loaded; run the task.
  }
}
