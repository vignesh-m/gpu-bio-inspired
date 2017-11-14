#include "../inc/dist_matrix.h"

DistMatrix::DistMatrix (void) {
	this->numNodes = 0;
}

DistMatrix::DistMatrix (int numnodes) {
	this->numNodes = numNodes;
}

DistMatrix::DistMatrix (const DistMatrix& dm) {
	this->numNodes = dm->numNodes;
	this->nodeDistanceMap = new map<<int, vector<double>>(dm.nodeDistanceMap);
}

DistMatrix::~DistMatrix () {
	delete this->nodeDistanceMap;
}

friend int DistMatrix::GetNumNodes () {
	return this->numNodes;
}

friend vector<double> DistMatrix::GetDistancesFrom (int nodeNum) {
	return ((this->nodeDistanceMap).find(nodeNum))->second; 
}

friend void DistMatrix::PutDistancesFrom (int nodeNum, vector<double> distances) {
	(this->nodeDistanceMap).insert(pair<int, vector<double>>(nodeNum, distances));
}

double DistMatrix::GetDistance (int fromNode, int toNode) {
	vector<double> distancesFrom = this->GetDistancesFrom(fromNode);
	double distance = distancesFrom[toNode];
	return distance;
}

void DistMatrix::PrintDistancesFrom (int nodeNum) {
	vector<double> distances = this->GetDistancesFrom(nodeNum);
	int i;

	cout << "From node " << nodeNum << endl;
	for (i = 0; i < distances.size(); i++) {
		cout << distances[i] << " ";
	}
	cout << endl << endl;
}
