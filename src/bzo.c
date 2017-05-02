#include "bzo.h"
#include "pthread.h"










void computeSize(int target, int* h, int* w){

  int num = 1024;
  int max = 1024;
  int min = 0;
  int cont = 1;
  while(cont){
    if((num * num) > target){
      max = num;
      num = (num + min) / 2;
    }else if((num * num) == target){
      cont = 0;
    }else if(num == (min+1)){
      cont = 0;
    }else{
      num = (max + num) / 2;
    }
  }

  if((num * (num - 1)) >= target){
    *h = num;
    *w = num - 1;
  }else{
    *h = num;
    *w = num;
  }
}










BzoStatus bzoInit(BzoEnvironment* env, int tnum){

  if(env  == NULL) return BZO_ERR_INV_PTR;
  if(tnum <  1   ) return BZO_ERR_INV_THD;

  // Get dimensions of TaskUnit Grid
  int h, w;
  computeSize(tnum, &h, &w);
  env->unitgrid = malloc(sizeof(BzoTaskUnit*) *  h * w);

  // Initialize TaskUnit Grid
  for(int it = 0; it < (h * w); it++){
    if(it < tnum)
      env->unitgrid[it] = malloc(sizeof(BzoTaskUnit));
    else
      env->unitgrid[it] = NULL;
  }

  // Allocate Map Queues
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

  return BZO_SUCCESS;
}
