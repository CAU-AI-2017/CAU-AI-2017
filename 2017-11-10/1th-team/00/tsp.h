#ifndef __TSP_H__
#define __TSP_H__

#define DISTANCEMAX 99999999
#define CITYNUM 1000
#include <vector>
#include "tour.h"

using namespace std;

class tsp {
public:
	int count = 0;
	vector<int> city;
	vector<int> distance;

	static inline int calc_location(int r, int c) { return r * CITYNUM + c; };

	tsp(char* ipt_file_path);
	void run(bool debug);
	void write(tour& data);


	void two_opt(int repeat, tour& data, int hold);
	void tsp::two_opt_swap(int i, int j, tour& src);

};

#endif
