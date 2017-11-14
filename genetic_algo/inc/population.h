#ifndef POPULATION_H
#define POPULATION_H

#include "tour.h"
#include "dist_matrix.h"

using namespace std;

class Population {
	private:
		vector<Tour> tours;
		Tour invalidTour;
		int size;

	public:
		Population (void);
		Population (int, int, bool, DistMatrix);
		~Population ();

		friend int GetSize (void);
		friend Tour Get (int);
		friend void Set (int, Tour);
		friend Tour GetFittest (void);
}

#endif
