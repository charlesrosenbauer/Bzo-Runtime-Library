#include <thread>










int getThreadNum (){
  return std::thread::hardware_concurrency();
}
