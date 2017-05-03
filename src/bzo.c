#include "bzo.h"
#include "pthread.h"










void computeSize(int target, int* h, int* w){

  // Doesn't need to be fast. Running only once, terminates in sqrt(target) iterations.
  int num  = 1;
  while(1){
    int sqr = (num * num);
    if(sqr == target){
      *h = num;
      *w = num;
      return;
    }else if(sqr - num == target){
      *h = num;
      *w = num - 1;
      return;
    }else if(sqr > target){
      if(sqr - num > target){
        *h = num;
        *w = num - 1;
        return;
      }
      *h = num;
      *w = num;
      return;
    }else{
      num++;
    }
  }
}










BzoStatus bzoInit(BzoEnvironment* env, int tnum){

  if(env  == NULL) return BZO_ERR_INV_PTR;
  if(tnum <  1   ) return BZO_ERR_INV_THD;

  // Get dimensions of TaskUnit Grid
  int h, w;
  computeSize(tnum, &h, &w);
  env->unitgrid = malloc(sizeof(BzoTaskUnit*) *  h * w);
  env->h  = h;
  env->w  = w;
  env->sz = (h*w);
  env->n  = tnum;

  // Initialize TaskUnit Grid
  for(int it = 0; it < (h * w); it++){
    if(it < tnum)
      env->unitgrid[it] = malloc(sizeof(BzoTaskUnit));
    else
      env->unitgrid[it] = NULL;
  }

  // Allocate Map Queues
  // Not currently working properly, also not necessarily needed yet
  /*
  int stride = 1;
  int toplevelnum;
  do{
    stride *= 4;
    int cont = (stride < tnum);
    if(cont){   // At top level, might be fewer than 4 nodes below
      for(int it = 0; it < tnum; it += stride)
        env->unitgrid[it]->nmq+=toplevelnum;
    }else{
      toplevelnum = 0;
      int maxIt = 0;
      for(int it = 0; it < (tnum - stride); it += stride){
        env->unitgrid[it]->nmq+=4;
        maxIt = it;
      }
      env->unitgrid[maxIt]->nmq += (tnum % stride);
    }
  }while(stride < tnum);
  for(int it = 0; it < tnum; it++)
    env->unitgrid[it]->mp = malloc(sizeof(TaskQueue) * (++env->unitgrid[it]->nmq));
  */


  return BZO_SUCCESS;
}
