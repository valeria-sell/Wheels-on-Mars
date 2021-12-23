// C++ program to illustrate the 
// iterators in vector 
#include <iostream> 
#include <vector> 
#include <thread>
#include <mutex>

#include <condition_variable>
#include <time.h> 

#include "../include/common.hpp"
#include "../include/producer.hpp"
#include "../include/genetic.hpp"



std::vector<terrain_log> terrain_report; 
std::mutex g_i_mutex;  // protects g_i
std::condition_variable cv;
std::mutex mtx;

int count = 0;
int finished_parsing = 0;

int main()
{
    std::thread t1(producer);
    std::thread t2(consumer);
 
    t1.join();
    t2.join();
}