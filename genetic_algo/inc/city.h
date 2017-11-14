#ifndef CITY_H
#define CITY_H

#include <vector>
#include <iostream>

using namespace std;

class City {
	private:
		int nodeNum;

	public:
		City (void);
		City (int);

		friend int GetNode (void);
		friend voidSetNode (int);
}

ostream& operator<< (std::ostream, const City&);
bool operator== (const City&, const City&);

#endif
