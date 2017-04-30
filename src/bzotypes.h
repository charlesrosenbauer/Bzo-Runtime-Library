#ifndef _BZO_TYPES_HEADER_
#define _BZO_TYPES_HEADER_
#include "stdint.h"










typedef enum{
  Bzo_TNRM,   // Normal Mode
  Bzo_TCSR,   // Normal Mode with Thread Constraint
  Bzo_TMAP,   // Standard Map Mode
  Bzo_TMPW,   // Map Mode with Exit Write
  Bzo_TMPI,   // Map Index Mode
  Bzo_TMIW,   // Map Index Mode with Exit Write
  Bzo_TMPC,   // Map Mode with Child
  Bzo_TMCI    // Map Index Mode with Child
}TaskMode;
typedef struct{
  void*    fp;   // Generic Function Pointer
  uint64_t dp;   // Generic Data
  void*    sz;   // Map Data Length
  uint64_t st;   // Map Array Stride, or Specific Thread
  uint64_t init; // Initial Index
  uint64_t ext;  // Either Exit write, or Child Parameter
  void*    chld; // Generic Child Function Pointer
  uint32_t cst;  // Cost per iteration
  TaskMode ty;   // Task Mode
}BzoTask;










typedef struct{     // Small Lock-Free Queue for Tasks.
  Task data[64];    // 64 Entries | 63 usable | 4kB block
  int top, bot;
  void* partner;    // Partner for two-way communication
  void* parent;     // Parent Task Unit
}TaskQueue;










typedef struct{       // Large Task Queue. Not thread Safe
  Task data[1024];    // 1024 entries | 1024 usable | 64kB block
  int base, size;
}PrivateTaskQueue;










typedef struct{
  PrivateTaskQueue pq;      // Private Queue
  TaskQueue        tq[4];   // Directional
  TaskQueue*       mp;      // Map Queues
  int              nmq;     // Number of Map Queues
  int              demand;  // Demand for Tasks. Normally 0.
  void*            alc;     // Allocator <not implemented yet>
  void*            env;     // Bzo Environment
}BzoTaskUnit;










typedef struct{
  TaskUnit** unitgrid;      // Pointer to pointers for grid
  int        h, w, n, sz;   // Height, width, number of active threads, size of grid
  void*      state;         // State for returning data upon failure or halt
}BzoEnvironment;










#endif
