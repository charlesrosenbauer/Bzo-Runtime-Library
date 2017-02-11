#include "bzo.h"
#include "pthread.h"










BzoStatus bzoInit(BzoRuntime* brt, int tnum){

  if(brt  == NULL) return BZO_ERR_INV_PTR;
  if(tnum <  1   ) return BZO_ERR_INV_THD;

  //If there are fewer than 16 threads, a cluster topology is used.
  //Else, a grid topology is used.
  //When adding NUMA support, alternate topologies may be used
  //A grid topology may change the number of threads
  if(tnum <= 16){
    brt->taskUnits = malloc(sizeof(BzoTaskUnit) * tnum);
    for(int it = 0; it < tnum; it++){
      //brt->taskUnits[it]
    }
  }else{

  }

  brt->threadNum = tnum;
  brt->sysStatus = 0;
  brt->returnState = NULL;

  return BZO_SUCCESS;
}
