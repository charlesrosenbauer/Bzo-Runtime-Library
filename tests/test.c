#include "../src/bzo.h"
#include "stdio.h"










int main(){
  BzoEnvironment e;
  bzoInit(&e, 8);
  printf("Hello World!\n");
  for(int it = 0; it < e.sz; it++){
    if(e.unitgrid[it])
      printf("Thread %i : MapQueues: %i\n", it, e.unitgrid[it]->nmq);
    else
      printf("Thread %i : Undefined thread\n", it);
  }
}
