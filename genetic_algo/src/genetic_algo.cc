#include "../inc/genetic_algo.h"

Population Genetic_Euclidean (Population population, int numNodes) {
	int size = population.GetSize();
	Population newPopulation = Population(size, numNodes, false);
	int i, j;

	for (j = 0; j < numIterations; j++) {
		if (selectFittest) {
			newPopulation.Set(0, population.GetFittest());
			elitismOffset = 1;
		}
	
		for (i = elitismOffset; i < size; i++) {
			Tour t1 = TournamentSelect(population, tournamentSize, numNodes);
			Tour t2 = TournamentSelect(population, tournamentSize, numNodes);
			Tour t = CrossOver(t1, t2, numNodes);
			newPopulation.Set(i, t);
		}
	
		for (i = 0; i < size; i++) {
			Mutate(newPopulation.Get(i), mutationRate);
		}
	}

	return newPopulation;
}
