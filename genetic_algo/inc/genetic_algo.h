#ifndef GENETIC_ALGO_H
#define GENETIC_ALGO_H

#include "genetic_help.h"

using namespace std;

double mutationRate = 0.1;
int tournamentSize = 5;
bool selectFittest = true;
int elitismOffset = 0;
int numIterations = 20;

Population Genetic_Euclidean (Population, int);

#endif
