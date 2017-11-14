#ifndef DIST_MATRIX_H
#define DIST_MATRIX_H

#include <iostream>
#include <vector>
#include <map>

using namespace std;

class DistMatrix {
	private:
		int numNodes;
		map<int, vector<double>> nodeDistanceMap;

	public:
		DistMatrix (void);
		DistMatrix (int);
		DistMatrix (const DistMatrix&);
		~DistMatrix ();
		
		friend int GetNumNodes ();
		friend vector<double> GetDistancesFrom (int);
		friend void PutDistancesFrom (int, vector<double>);

		double GetDistance (int, int);
		void PrintDistancesFrom (int);
}

#endif
