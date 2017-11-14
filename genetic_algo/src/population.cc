#include "../inc/population.h"
#include <climits>

Population::Population (void) {
	this->size = 0;
	this->invalidTour = Tour();
}

Population::Population (int size, int tourSize, bool generateTours) {
	int i;

	this->size = size;
	this->invalidTour = Tour();

	if (generateTours) {
		for (i = 0; i < size; i++) {
			(this->tours).push_back(Tour(tourSize));
		}
	}
	else {
		for (i = 0; i < size; i++) {
			(this->tours).push_back(invalidTour);
		}
	}	
}

Population::~Population () {
	delete this->tours;
}

friend int Population::GetSize (void) {
	return this->size;
} 

friend Tour Population::Get (int i) {
	if (i >= 0 && i < this->size) {
		return (this->tours)[i];
	}
	return this->invalidTour;
}

friend void Population::Set (int i, Tour t) {
	(this->tours)[i] = Tour(t);
}

friend Tour Population::GetFittest (void) {
	vector<Tour>::iterator iter;
	Tour fittest;
	int currFitness = INT_MIN;
	int fitness;

	for (iter = (this->tours).begin(); iter != (this->tours).end(); ++iter) {
		fitness = (*iter).GetFitness();
		if (fitness > currFitness) {
			currFitness = fitness;
			fittest = *iter;
		}
	}
	return fittest;
}
