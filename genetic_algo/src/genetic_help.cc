#include "../inc/genetic_help.h"
#include <cstdlib>

Tour TournamentSelect (Population population, int tournamentSize, int numNodes) {
	Population tournPopulation = Population(tournamentSize, numNodes, false);
	int populationSize = population.GetCurrentSize();
	int i, p;

	for (i = 0; i < tournamentSize; i++) {
		p = rand() % populationSize;
		tournPopulation.Set(i, population.Get(p));
	}

	return tournPopulation.GetFittest();
}

void Mutate (Tour& tour, double mutationRate) {
	uint64_t timeSeed = std::chrono::high_resolution_clock::now().time_since_epoch().count();
    	seed_seq ss{uint32_t(timeSeed & 0xffffffff), uint32_t(timeSeed>>32)};
	
    	// initialize the random number generator with time-dependent seed
	mt19937_64 rng;
    	rng.seed(ss);

    	// initialize a uniform distribution between 0 and 1
    	uniform_real_distribution<double> unif(0, 1);
	double mutationChoice = unif(rng);

	int startPos = rand() % tour.GetCurrentSize();
	int endPos = rand() % tour.GetCurrentSize();

	if (startPos > endPos) {
		int temp = startPos;
		startPos = endPos;
		endPos = temp;
	}
	
	if (mutationChoice < mutationRate) { 
		cout << "Mutation: " << startPos << " to " << endPos << endl;
		int i = startPos;
		int j = endPos;

		while (i < j) {
			City c = tour.GetCityAtIndex(i);
			City d = tour.GetCityAtIndex(j);
			tour.SetIndexToCity(i, d);
			tour.SetIndexToCity(j, c);
			i++;
			j--;
		}
	
		cout << "New tour: " << tour << endl;
	}
}

Tour CrossOver (Tour parent1, Tour parent2, int numNodes) {
	Tour child = Tour(parent1);

	int startPos = rand() % tour.GetCurrentSize();
	int endPos = rand() % tour.GetCurrentSize();

	if (startPos > endPos) {
		int temp = startPos;
		startPos = endPos;
		endPos = temp;
	}

	cout << "Crossover: " << startPos << " to " << endPos << endl;

	int i;
	for (i = startPos; i <= endPos; i++) {
		City c1 = child.GetCityAtIndex(i);
		City c2 = parent2.GetCityAtIndex(i);

		if (! (c1 == c2)) {
			cout << "Searching for " << c2 << " in " << child << endl;
			int j = child.GetIndexOfCity(c2);
			cout << "Position " << j << endl;
			child.SetIndexToCity(j, c1);
		}
		parent.SetIndexToCity(i, c2);
	}

	return child;
}
