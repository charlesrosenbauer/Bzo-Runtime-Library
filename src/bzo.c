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

  int h, w;
  computeSize(tnum, &h, &w);

  env->unitgrid = malloc(sizeof(BzoTaskUnit) *  h * w);

  return BZO_SUCCESS;
}
