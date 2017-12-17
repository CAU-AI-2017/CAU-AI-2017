#include "tsp.h"
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>
#include <time.h>
#include<thread>
#include<windows.h>

#include "tour.h"

#define OPT 1
#define RANDOMOPT 3000

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


		this->two_opt_1(OPT,opt_tour,best_tour.distance);

		if (opt_tour < best_tour) {
			best_tour.copy(opt_tour);
			best_tour.distance = opt_tour.distance;
		}

		if (best_tour > default_tour) {
			best_tour.copy(default_tour);
			best_tour.calcDistance(this->distance);
		}
	}


	//for (int i = 0; i < RANDOMOPT; i++) {
	//	srand(GetTickCount());
	//	int a = rand() % 1000 + 1;
	//	int b = rand() % 1000 + 1;

	//	opt_tour.copy(best_tour);

	//	int swap = opt_tour.order[a];
	//	opt_tour.order[a] = opt_tour.order[b];
	//	opt_tour.order[b] = swap;
	//	cout << a << ":" << b << endl;

	//	opt_tour.calcDistance(this->distance);
	//	this->two_opt_1(OPT, opt_tour, best_tour.distance);
	//}

	//if (best_tour > opt_tour) {
	//	best_tour.copy(opt_tour);
	//	best_tour.distance = opt_tour.distance;
	//}
	

	end = clock();
	cout << best_tour.distance << endl;
	cout << "알고리즘 수행시간 : " << (end - begin) << "ms" << endl;

	this->write(best_tour);

}

void tsp::write(tour& data) {
	ofstream outFile("tsp_result.txt");
	int city;

	if (data.order.back() != 0) {
		data.order.pop_back();

		while ((city = data.order.front()) != 0) {
			data.order.push_back(city);
			data.order.erase(data.order.begin());
		}
	}
	data.order.push_back(0);
	

	for (int city : data.order) {
		outFile << city+1 << endl;
	}

	outFile.close();
}

void tsp::two_opt_swap(int i, int j, tour& src) {
	int for_reverse_idx = 0;
	vector<int> reverse;

	for (int k = j; k >= i; k--)
		reverse.push_back(src.order[k]);

	for (int k = i; k <= j; k++) {
		src.order[k] = reverse[for_reverse_idx];
		for_reverse_idx++;
	}
}

// 선택한 도시가 맨 처음 최적값을 발견했을때 멈추고, 1000개중에 제일 좋은 최적화를 선택, 기존 최적화된 투어보다 +2500 안에 있을경우는 더 최적화 시도
void tsp::two_opt_1(int original_repeat, tour& data, int hold) {
	int repeat = original_repeat;
	int min_change,change, change_i, change_j;
	bool discover = false;

	while (repeat--) {
		min_change = 0;
		discover = false;

		for (int i = 0; i < CITYNUM-2; i++) {
			if (discover) break;

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
					discover = true;
					//break;
				}
			}
		}

 		if (min_change < 0) {
			this->two_opt_swap(change_i, change_j, data);
			data.distance += min_change;
			if (data.distance < hold + CITYNUM/4*10) {
				repeat = original_repeat;
				//cout << data.order[0] << endl;
			}
				
		}
		else break;
	}
}

