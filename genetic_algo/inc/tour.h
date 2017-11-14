#ifndef TOUR_H
#define TOUR_H

#include "city.h"
#include "dist_matrix.h"

using namespace std;

class Tour {
	private:
		vector<City> tour;
		DistMatrix dm;
		double fitness;
		double distance;
		int size;

	public:
		Tour (void);
		Tour (int, DistMatrix);
		Tour (const Tour&);
		~Tour ();

		friend City GetCityAtIndex (int);
		friend void SetIndexToCity (int, const City&);
		friend double GetFitness (void);
		friend double GetDistance (void);
		friend int GetCurrentSize (void);
		friend int GetIndexOfCity (const City&);
		friend void SetTour (vector<City>);
		friend void SetDistanceMatrix (DistMatrix);
		friend DistMatrix GetDistanceMatrix (void);

		void ShuffleTour (void);
		vector<City> GetOrdinalRepresentation (void);
}

ostream& operator<< (ostream&, const Tour&);

#endif
