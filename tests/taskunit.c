#include "taskunit.h"
#include <stdlib.h>










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
  return ((16 + q->top) - q->end) % 32;
}










void initTaskUnit(BzoTaskUnit* t, void* env, int isActive){
  t->environment = env;
  t->base = 0;
  t->size = 0;
  t->isActive = (isActive == 1)? 1 : 0;
}










/*
  env  -> Environment to initialize
  tnum -> Thread Number to aim for
*/
BzoStatus initEnvironment(BzoEnvironment* env, int tnum){

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
    env->grid = (BzoTaskUnit*)malloc(sizeof(BzoTaskUnit) * env->h * env->w);
    int ct = env->h * env->w;
    for(int it = 0; it < ct; it++){
      initTaskUnit(&env->grid[it], &env, (it < tnum)? 1 : 0);
    }
  }

  return BZ_SUCCESS;
}
