#include "bzo.h"
#include "threadmanager.hpp"
#include <cstdio>










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
}
