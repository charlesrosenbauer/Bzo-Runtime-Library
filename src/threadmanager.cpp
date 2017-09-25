#include <thread>
#include "bzotypes.h"










int getThreadNum (){
  return std::thread::hardware_concurrency();
}










// Starts up a threads with a given function, passes it a parameter, returns when the thread completes.
void startThreads (BzoEnvironment* env, void(*fn)(BzoTaskUnit*)){
  std::thread* threadpool = new std::thread [env->count - 1];
  for(int i = 0; i < env->count-1; i++)
    threadpool[i] = std::thread(fn, &env->grid[i+1]);

  fn(&env->grid[0]);

  for(int i = 0; i < env->count-1; i++)
    threadpool[i].join();
}
