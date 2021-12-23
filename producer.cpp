#include <mutex>
#include <unistd.h>
#include <condition_variable>
#include <iostream> 
#include <fstream>
#include <string>

#include <vector> 

#include "../include/producer.hpp"
#include "../include/common.hpp"

void producer(){

  int KmStart = 0;
  int KmEnd = 0;
  int steadiness = 0;
  int humidity = 0;
  int grip = 0;

  int current_ptr = 0;

  std::string line;
  std::ifstream myfile ("../misc/report.json");
  if (myfile.is_open())
  {
    terrain_log temp;

    while ( std::getline (myfile,line) )
    {
      //std::cout << current_ptr++ << line << '\n';
      current_ptr = line.find("KmStart"); 


      if(current_ptr != -1){
        KmStart = stoi(line.substr(current_ptr+9));
      }
      current_ptr = line.find("KmEnd");
      if(current_ptr != -1){
        KmEnd = stoi(line.substr(current_ptr+7));
      }
      current_ptr = line.find("Firmeza");
      if(current_ptr != -1){
        steadiness = stoi(line.substr(current_ptr+9));
      }
      current_ptr = line.find("Humedad");
      if(current_ptr != -1){
        humidity = stoi(line.substr(current_ptr+9));
      }
      current_ptr = line.find("Agarre");
      if(current_ptr != -1){
        grip = stoi(line.substr(current_ptr+8));
        temp = {KmStart,KmEnd,steadiness,humidity,grip}; //asigna todo
        std::unique_lock<std::mutex> lck(mtx); // auto lock
        terrain_report.push_back(temp); 
        //std::cout << "pushed" << count << '-' << finished_parsing << '\n';
        count++;
        cv.notify_one(); //notifica que ya hay uno listo
        //std::cout << "notify" << count << '-' << finished_parsing << '\n';
        usleep(10000);
      }
      
      //std::cout<<"pushed! " << std::endl;
    } // unlocks lck
    myfile.close();

    finished_parsing++;
    cv.notify_one();

  }

  else std::cout << "Unable to open file"; 
}