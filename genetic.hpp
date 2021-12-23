#ifndef GENETIC_H
#define GENETIC_H

#include "common.hpp"
#define TOURNAMENT_SIZE 3  //Selection Method 
#define GEN_SIZE 50
#define SURVIVORS 3 
#define PARENT_PAIRS GEN_SIZE - SURVIVORS
#define MUTANTS 5

typedef struct pair{
  int father;
  int mother;
}pair;

class Individual{ //def del individuo para el genetico 
  public:
    float fitness;
    float fitness_fold;
    float fitness_torque;
    int torque;
    int fold;

    Individual();
    float calc_fitness(terrain_log T); //fitness function
    void print_individual();
    void mutate();
    int tournament_selection(Individual generation[]);
    void do_generation();

};

Individual cross_over(int father, int mother, Individual generation []); // f  y m indices

int get_torque_grip(int grip);
int get_torque_steadiness(int steadiness);
int get_torque_humidity(int humidity);
int get_fold_steadiness(int steadiness);
int get_fold_humidity(int humidity);
int get_fold_grip(int grip);
float get_normalized_value(int proposed_value, int firmness_value, int humidity_value, int grip_value);


void consume();
int consumer();
void print_terrain(terrain_log t);
void calc_remaing_battery(int best_individual, Individual Population[], terrain_log T);
#endif
