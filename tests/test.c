//#include "../src/bzo.h"
#include "taskunit.h"
#include "stdio.h"










int main(){
  BzoEnvironment env;
  int tnum = 11;
  initEnvironment(&env, tnum);
  printf("%i, %i, %i\n", env.h, env.w, env.count);
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

  BzoTask t;
  t.fptr = NULL;
  t.data = NULL;
  BzoTask ts[16];
  for(int it = 0; it < 16; it++) ts[it] = t;

  printf("%i \n", pushPrivateTasks(&(env.grid[0]), (BzoTask*)&ts, 16));
  printf("%i \n", env.grid[0].size);
}
