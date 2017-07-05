//#include "../src/bzo.h"
#include "taskunit.h"
#include "stdio.h"










int main(){
  BzoEnvironment env;
  initEnvironment(&env, 4);
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
}
