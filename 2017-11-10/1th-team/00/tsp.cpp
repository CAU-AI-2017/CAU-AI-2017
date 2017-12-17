#include "tsp.h"
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>
#include <time.h>
#include<thread>

#include "tour.h"

#define OPT 1

using namespace std;

tsp::tsp(char* ipt_file_path) {
	string line;
	ifstream inf(ipt_file_path);
	int num, i = 0;

	while (!inf.eof()) {
		getline(inf, line);
		if(i < CITYNUM) tsp::city.push_back(i++);

		std::stringstream ss(line);

		while (ss >> num)
		{
			tsp::distance.push_back(num);

			if (ss.peek() == ',')
				ss.ignore();
		}
	}

	inf.close();
}

void tsp::run(bool debug) {

	clock_t begin, end;
	
	tour default_tour, opt_tour, best_tour;
	best_tour.distance = DISTANCEMAX;

	begin = clock();

	for (int i = 0; i < CITYNUM; i++) {
		default_tour.setNearestNeighbour(i, this->city, this->distance);
		opt_tour.copy(default_tour);
		opt_tour.calcDistance(this->distance);
		this->two_opt(OPT,opt_tour,best_tour.distance);

		if (opt_tour < best_tour) {
			best_tour.copy(opt_tour);
			best_tour.distance = opt_tour.distance;
		}
	}

	end = clock();
	/*cout << this->count << " : " << this->count / 1000 << endl;*/
	best_tour.debugCheckOrder();
	cout << best_tour.distance << endl;
	cout << "알고리즘 수행시간 : " << (end - begin) << "ms" << endl;

	this->write(best_tour);

}

void tsp::write(tour& data) {
	ofstream outFile("tsp_result.txt");

	for (int city: data.order) {
		outFile << city << endl;
	}

	outFile.close();
}


void tsp::two_opt(int original_repeat, tour& data, int hold) {
	int repeat = original_repeat;
	int min_change,change, change_i, change_j;
	//bool discover = false;

	while (repeat--) {
		min_change = 0;
		//discover = false;

		for (int i = 0; i < CITYNUM-2; i++) {
			//if (discover) break;

			for (int j = i + 2; j < CITYNUM; j++) {
				change = this->distance[this->calc_location(data.order[i], data.order[j])]
					+ this->distance[this->calc_location(data.order[i + 1], data.order[j + 1])]
					- this->distance[this->calc_location(data.order[i], data.order[i + 1])] 
					- this->distance[this->calc_location(data.order[j], data.order[j + 1])]; 
					
				if (change < min_change) {
					this->count++;
					min_change = change;
					change_i = i+1; 
					change_j = j;
					//discover = true;
					break;
				}
			}
		}

 		if (min_change < 0) {
			this->two_opt_swap(change_i, change_j, data);
			data.distance += min_change;
			if (data.distance < hold+ CITYNUM/4*9) {
				repeat = original_repeat;
				//cout << data.order[0] << endl;
			}
				
		}
		else break;
	}
}

void tsp::two_opt_swap(int i, int j, tour& src) {
	int for_reverse_idx = 0;
	vector<int> reverse;
	
	for (int k = j; k >= i ; k--) 
		reverse.push_back(src.order[k]);
	
	for (int k = i; k <= j; k++){
		src.order[k] = reverse[for_reverse_idx];
		for_reverse_idx++;
	}
}