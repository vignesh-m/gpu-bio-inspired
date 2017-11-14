#ifndef GENETIC_HELP_H
#define GENETIC_HELP_H

#include "population.h"

using namespace std;

Tour TournamentSelect (Population, int, int);
void Mutate (Tour&, double);
Tour CrossOver (Tour, Tour, int);

#endif
