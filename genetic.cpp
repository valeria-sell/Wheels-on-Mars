#include "../include/genetic.hpp"
#include <iostream>

#include <time.h>
#include <unistd.h>

int remaining_battery = 7000;
int torque_energy_km [] = {12, 10, 9, 8, 7, 6, 4};
int fold_energy_km [] = {15, 13, 11, 10, 8, 7, 6};

int get_torque_steadiness(int steadiness){
  if (steadiness >= 80){
    return 1;
  }
  else if (steadiness >= 65){
    return 2;    
  }
  else if (steadiness >= 60){
    return 3;
  }
  else if (steadiness >= 40){
    return 4;
  }
  else if (steadiness >= 0){
    return 5;    
  }else {
    return 1;
  }
}

int get_torque_humidity(int humidity){
  if (humidity <= 20){
    return 7;
  }
  else if (humidity <= 40){
    return 6;    
  }
  else if (humidity <= 55){
    return 5;
  }
  else if (humidity <= 60){
    return 4;
  }
  else if (humidity <= 70){
    return 3;    
  }
  else if (humidity > 70){
    return 2;    
  }
  else {
    return 1;
  }
}

int get_torque_grip(int grip){
  if (grip >= 90){
    return 7;
  }
  else if (grip >= 70){
    return 6;    
  }
  else if (grip >= 50){
    return 5;
  }
  else if (grip >= 30){
    return 4;
  }
  else if (grip < 30){
    return 3;    
  }else {
    return 1;
  }
}

int get_fold_steadiness(int steadiness){
    if (steadiness >= 75){
        return 7;
    }
    else if (steadiness >= 50){
        return 6;
    }
    else if (steadiness >= 25){
        return 5;
    }
    else if (steadiness >= 20){
        return 4;
    }
    else if (steadiness >= 10){
        return 3;
    }
    else if (steadiness >= 0){
        return 1;
    }else {
    return 1;
  }
}

int get_fold_humidity(int humidity){
    if (humidity <= 20){
        return 7;
    }
    else if (humidity <= 45){
        return 6;
    }
    else if (humidity <= 60){
        return 5;
    }
    else if (humidity <= 75){
        return 4;
    }
    else if (humidity <= 80){
        return 3;
    }
    else if (humidity > 80){
        return 2;
    }else {
     return 1;
  }

}

int get_fold_grip(int grip){
    if (grip >= 80){
        return 7;
    }
    else if (grip >= 50){
        return 6;
    }
    else if (grip >= 30){
        return 5;
    }
    else if (grip >= 20){
        return 4;
    }
    else if (grip >= 10){
        return 3;
    }
    else if (grip >= 5){
        return 2;
    }
    else if (grip >= 0){
        return 1;
    }else {
    return 1;
  }
}


float get_normalized_value(int proposed_value, int firmness_value, int humidity_value, int grip_value){
    float normalized_value;
    proposed_value = proposed_value*3;
    float sum_value = (float)(firmness_value+humidity_value+grip_value);
    proposed_value = abs(proposed_value-sum_value);
    normalized_value = ((float)proposed_value)/21.0; // 3 from the *3 , 7 for normalization
    return normalized_value;
}

Individual::Individual(){
  fitness = 1.0;
  fitness_torque = 1.0;
  fitness_torque = 1.0;
  torque = rand()%7 +1 ;
  fold = rand()%7 + 1;
}

// Calculates fitness depending on how far was the guess from the correct values of torque and fold
float Individual::calc_fitness(terrain_log T){ 

    int torque_humidity = get_torque_humidity(T.humidity);
    int torque_grip = get_torque_grip(T.grip);
    int torque_steadiness = get_torque_steadiness(T.steadiness);
    fitness_torque = get_normalized_value(torque, torque_steadiness, torque_humidity, torque_grip);

    int fold_humidity = get_fold_humidity(T.humidity);
    int fold_grip = get_fold_grip(T.grip);
    int fold_steadiness = get_fold_steadiness(T.steadiness);
    fitness_fold = get_normalized_value(fold, fold_steadiness, fold_humidity, fold_grip);  
    // Valores del terreno mapeados
    //std::cout << "TH " << torque_humidity << " | TG " << torque_grip << " | TS " << torque_steadiness << "\n";
    //std::cout << "FH " << fold_humidity << " | FG " << fold_grip << " | FS " << fold_steadiness << "\n";

    fitness = (fitness_fold + fitness_torque) / 2.0;
    return fitness;
}

void Individual::print_individual(){
  std::cout << "torque: " << torque << " | fold: " << 11-fold << "\n";
  std::cout << "fitness: " << fitness_torque << "(T) & " << fitness_fold << 
    "(F) --> " << fitness << "\n";

  // 11 - fold = 8 - fold + 3
  // 8 - fold = invertir los valores de 1-7 a 7-1
  // + 3 = offset para que vayan de 10-4
}

//This method selects the best parent from the population
int tournament_selection(Individual generation []){
    
    int best_parent = rand()%GEN_SIZE;
    float best_fit = generation[best_parent].fitness;
    float current_fit = best_fit; //for comparisson only
    for(int i = 0; i < TOURNAMENT_SIZE-1; i++){ 
        if (current_fit<best_parent){
            best_parent=current_fit;
        }
        current_fit = generation[rand()%GEN_SIZE].fitness;  
    }
    return best_fit;
}

// This method mutates the individual itself, generates a new value of torque or fold
void Individual::mutate(){
  int select_torque_fold = rand()%2;  
  int rand_value = rand()%7 +1;
  if (select_torque_fold){
    torque = rand_value;
  }else{
    fold = rand_value;
  }
}

// This function receives the index position of the array where each parent is 
// The function returns a new Individual with random values from parents
Individual cross_over(int father, int mother, Individual generation []){
  int rand_torque = rand() % 2;
  int rand_fold = rand() % 2;
  Individual newInd;
  newInd.torque = rand_torque ? generation[father].torque : generation[mother].torque ; //operador ternario
  newInd.fold = rand_fold ? generation[father].fold : generation[mother].fold ;
  return newInd;
}

void calc_remaing_battery(int best_individual, Individual Population[], terrain_log T){
    int km = T.endKm - T.startKm; //km recorridos
    int torque_energy = torque_energy_km[(Population[best_individual].torque -1)]*km;
    int fold_energy = fold_energy_km[(Population[best_individual].fold -1)]*km;
    int energy_used = torque_energy+fold_energy;
    remaining_battery -= energy_used;
}

void consume(Individual Population[GEN_SIZE]){
    std::unique_lock<std::mutex> lck(mtx); //lo pone en lock

    while (count == 0 && finished_parsing == 0) {
        //std::cout << "waiting" << count << '-' << finished_parsing << '\n';
        cv.wait(lck); //revisa hasta que count 1
    }

    if (finished_parsing && count == 0){ //cond necesaria para que termine una vez que ya no queden elem
      //std::cout << "finishd\n";
      return ;
    }

    terrain_log current_T = terrain_report[0];

    print_terrain(current_T); //imprime datos


    int best_individual;
    float best_fitness = 1.0;
    for(int i = 0; i<GEN_SIZE; i++){
        float current_fit = Population[i].calc_fitness(current_T);
        if (best_fitness < current_fit){
            best_fitness = current_fit;
            best_individual = i;
        }
    }


    calc_remaing_battery(best_individual, Population, current_T);
    std::cout << "Best combination found: ";
    Population[best_individual].print_individual();
    std::cout << "Remaining Battery: " << remaining_battery << "\n---------------\n";

    pair parents [PARENT_PAIRS];
    
    for(int i = 0; i<PARENT_PAIRS; i++){
        parents[i].father = tournament_selection(Population);
        parents[i].mother = tournament_selection(Population);
    } 

    Individual newGen [GEN_SIZE];

    for (int i = 0; i<PARENT_PAIRS; i++){
      newGen[i] = cross_over(parents[i].father, parents[i].mother, Population);}    
    for (int i = PARENT_PAIRS; i < GEN_SIZE; i++){ //survivors
      newGen[i] = Population[rand()%GEN_SIZE]; }


    for (int i = 0; i < MUTANTS ; i++){
      newGen[rand()%GEN_SIZE].mutate();}
      
    for (int i = 0; i<GEN_SIZE; i++){
      Population[i] = newGen[i];}

    terrain_report.erase(terrain_report.begin()); //lo borra de la "cola"
    count--;
} 

int consumer(){ 
    srand (time(NULL));

    
    Individual Population[GEN_SIZE]; //la poblacion no crece por temas de control

    while (!finished_parsing || count != 0) {   
      //std::cout << "w_out" << count << '-' << finished_parsing << '\n'; 
      consume(Population);
      usleep(9000);
      //std::cout<<"consumed! " << std::endl;
      }


    return 0; 
} 

void print_terrain(terrain_log t){
  std::cout << "start km: " << t.startKm << "\n";
  std::cout << "end km: " << t.endKm << "\n";
  std::cout << "steadiness: " << t.steadiness << "\n";
  std::cout << "humidity: " << t.humidity << "\n";
  std::cout << "grip: " << t.grip << "\n--\n";
}
