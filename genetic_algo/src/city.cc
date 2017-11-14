#include "../inc/city.h"

City::City (void) {
	this->nodeNum = -1;
}

City::City (int nodenum) {
	this->nodeNum = nodenum;
}

friend int City::GetNode (void) {
	return this->nodeNum;
}

friend void City::SetNode (int nodenum) {
	this->nodeNum = nodenum;
}

ostream& operator<< (ostream &stream, const City& c) {
  	return stream << c.GetNode() << " ";
}

bool operator== (const City& c1, const City& c2) {
	return (c1.GetNode() == c2.GetNode());
}
