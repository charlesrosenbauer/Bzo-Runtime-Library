#include "bzo.h"
#include "bzotypes.h"
#include "threadmanager.hpp"
#include <cstdio>
#include <cstdlib>










struct FibFrame {
  int a, b;  // a and b are effectively promised integers.
  int x;     // x is the input.
  int* out;  // where to write the result.
  FibFrame* returnFrame;
};









/*
  Extremely inneficient way to implement a fibonacci function. Just for proof of concept.

  Stack frames must be managed in software, as high parallelism necessitates a tree be used instead.
  Allocation is currently using malloc/free. A faster approach would be better, but this is of course
  a proof-of-concept.

  This is part 1 of the function:
    fib(0) = 1
    fib(1) = 1
    fib(x) = fib(x-1) + fib(x-2)
  Here, fib(0) and fib(1) have their results returned. In all other cases,
    fib(x-1) and fib(x-2) functions are spawned to run in parallel.
*/
BzoTask fib0 (void* taskunit, void* data){
  BzoTaskUnit* tu = (BzoTaskUnit*) taskunit;
  FibFrame* f = (FibFrame*)data;

  if(f->x < 2){
    *f->out = 1;
    free(f);
    return BzoTask{NULL, NULL};
  }

  FibFrame* f0 = (FibFrame*)malloc(sizeof(FibFrame));
  *f0 = FibFrame{-1, -1, f->x-1, 0, f};
  BzoEnvironment* env = (BzoEnvironment*)tu->environment;
  int parFail = 0;
  if(env->globalEncourage){
    //Parallelism!
    FibFrame* f1 = (FibFrame*)malloc(sizeof(FibFrame));
    *f1 = FibFrame{-1, -1, f->x-2, 0, f};
    BzoTask task = BzoTask{fib0, f1};
    parFail = spawnTasks(tu, &task, 1);
    fib0(tu, f0);
  }

  if(parFail){
    //Sequentialism!
    fib0(tu, f0);
    *f0 = FibFrame{-1, -1, f->x-2, 0, f};
    fib0(tu, f0);
    *f->out = f->a + f->b;
    free(f);
    return BzoTask{NULL, NULL};
  }

  return BzoTask{NULL, NULL};
}

/*
  After fib(x-1) and fib(x-2) both return, this adds the result and returns it.
*/
BzoTask fib1 (void* taskunit, void* data){
  BzoTaskUnit* tu = (BzoTaskUnit*) taskunit;
  FibFrame* f = (FibFrame*)data;
  if ((f->a == -1) && (f->b == -1)){
    //Everthing is evaluated!
    *f->out = f->a + f->b;
    free(f);
    return BzoTask{NULL, NULL};
  }else{
    //Unevaluated data, try again later!
    return BzoTask{fib1, f};
  }
}










int main(){
  BzoEnvironment env;
  bzoInit(&env, 64);
  for(int it = 0; it < (env.h * env.w); it++){
    printf("%i : (%p %p) (%p %p) (%p %p) (%p %p)\n", it,
      &env.grid[it].neighbors[0],
       env.grid[it].neighbors[0].sibling,
      &env.grid[it].neighbors[1],
       env.grid[it].neighbors[1].sibling,
      &env.grid[it].neighbors[2],
       env.grid[it].neighbors[2].sibling,
      &env.grid[it].neighbors[3],
       env.grid[it].neighbors[3].sibling );
  }
  int ct = 0;
  for(int x = 0; x < env.w; x++){
    for(int y = 0; y < env.h; y++){
      if(env.grid[ct].isActive != 1)
        printf(".");
      else
        printf("#");
      ct++;
    }
    printf("\n");
  }
  startThreads(&env, coreRuntime);
}
