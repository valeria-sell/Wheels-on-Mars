#ifndef COMMON_H
#define COMMON_H
#include <mutex>
#include <vector> 
#include <condition_variable>

typedef struct terrain_log{ //para que lo reconozca como tipo de dato
  int startKm; 
  int endKm;
  int steadiness;
  int humidity;
  int grip;
}terrain_log;

extern std::vector<terrain_log> terrain_report; 
extern std::mutex g_i_mutex;  // protects g_i
extern std::condition_variable cv;
extern std::mutex mtx;
extern int count;
extern int finished_parsing;

#endif