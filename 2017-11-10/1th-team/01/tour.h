#ifndef __TOUR_H__
#define __TOUR_H__

#include <iostream>
#include <vector>

using namespace std;

class tour{
  public:
	int distance;
	vector<int> order;

    void setNearestNeighbour(int start, vector<int> city, vector<int> distance);
	void debugCheckOrder();
	void calcDistance(vector<int> distance);
	void inline copy(tour& ref) {
		this->order.clear();
		this->order.assign(ref.order.begin(), ref.order.end());
	}

	bool operator<	(tour & ref) {
		return this->distance < ref.distance;
	}
	bool operator> (tour & ref) {
		return this->distance > ref.distance;
	}
	bool operator<	(int distance) {
		return this->distance < distance;
	}
	bool operator>	(int distance) {
		return this->distance > distance;
	}
};

#endif
