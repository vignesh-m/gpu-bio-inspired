#include "../inc/tour.h"
#include <algorithm>    // std::shuffle
#include <random>       // std::default_random_engine

DistMatrix defaultDistMatrix = DistMatrix();

Tour::Tour (void) {
	this->size = 0;
	this->fitness = 0.0;
	this->distance = 0.0;
	this->dm = new DistMatrix();
}

Tour::Tour (int numNodes, DistMatrix distanceMatrix = defaultDistMatrix) {
	int i;

	this->fitness = 0.0;
	this->distance = 0.0;
	this->dm = new DistMatrix(distanceMatrix);
	this->size = numNodes;

	for (i = 0; i < this->size; i++) {
		(this->tour).push_back(new City(i));
	}
	this->ShuffleTour();
}

Tour::~Tour () {
	delete this->tour;
	delete this->distanceMatrix;
}

friend City Tour::GetCityAtIndex (int index) {
	if (index >= 0 && index < this->size) {
		return this->tour[index];
	}
	return this->tour[0];
}

friend void Tour::SetIndexToCity (int index, const City& city) {
	if (index >= 0 && index < this->size) {
		this->tour[index] = city;
		this->fitness = 0.0;
		this->distance = 0.0;
	}
}

friend double Tour::GetFitness (void) {
	if (this->fitness == 0.0) {
		return (1.0 / (double) GetDistance());
	} 
	return this->fitness;
}

friend double Tour::GetDistance (void) {
	if (this->distance == 0.0) {
		int i;
		for (i = 0; i < this->size-1; i++) {
			this->distance += (this->dm).GetDistance(i, i+1);
		}
		this->distance += (this->dm).GetDistance(this->size-1, 0);
	}
	return this->distance;
}

friend int Tour::GetCurrentSize (void) {
	return this->size;
}

friend int Tour::GetIndexOfCity (const City& city) {
	int i;
	for (i = 0; i < this->size; i++) {
		if (city == this->tour[i]) {
			return i;
		}
	}
	return -1;
}

friend void Tour::SetTour (vector<City> newTour) {
	this->tour = newTour;
} 

friend void Tour::SetDistanceMatrix (DistMatrix m) {
	this->dm = DistMatrix(m);
}

friend DistMatrix Tour::GetDistanceMatrix (void) {
	return this->dm;
}

void Tour::ShuffleTour (void) {
	auto rng = default_random_engine {};
	shuffle(begin(this->tour), end(this->tour), rng);
}

vector<City> Tour::GetOrdinalRepresentation (void) {
	vector<int> order(this->size);
	vector<City> ordinalTour(this->size);
	int i, j;

	for (i = 0; i < order.size(); i++) {
		order[i] = i+1;
	}

	for (i = 0; i < ordinalTour.size(); i++) {
		int nodeNum = this->GetCityAtIndex(i);
		ordinalTour[nodeNum] = new City(order[i]);

		order[nodeNum] = -1;
		for (j = nodeNum+1; j < size; j++) {
			temp[j] -= 1;
		}
	}
	return ordinalTour;
}


ostream& operator<< (ostream &stream, const Tour& t) {
  	int i;
	ostream << "["
	for (i = 0; i < t.GetCurrentSize(); i++) {
		ostream = ostream << t.Get(i) << " ";
	}
	ostream << "]" << endl;
	return ostream;
}	
