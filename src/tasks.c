#include "tasks.h"










int pushTask(TaskQueue* q, BzoTask t){
  int next = (q->bot + 1) % 16;
  if(next != q->top){
    q->data[q->bot] = t;
    q->bot = next;
    return 1; //Success!
  }
  return 0;   //Queue is full!
}










int popTask(TaskQueue* q, BzoTask* t){
  if(q->top != q->bot){
    *t = q->data[q->top];
    q->top = (q->top + 1) % 16;
    return 1; //Success!
  }
  return 0;   //Queue is empty!
}










int queueSize(TaskQueue* q){
  return ((16 + q->top) - q->bot) % 16;
}










void runTaskUnit (BzoTaskUnit* tu){

}
