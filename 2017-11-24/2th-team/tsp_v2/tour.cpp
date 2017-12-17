#include "tour.h"
#include "tsp.h"
#include <iostream>
#include <vector>


using namespace std;

void tour::setNearestNeighbour(int start, vector<int> origin_city, vector<int> distance){
	int cur_distance;
	int cur_city = start;
	int nei_city_idx;
	int nei_distance;
	int total_distance = 0;

	// tour 초기화
	this->distance = 0;
	this->order.clear();
	
	// 도시 설정(tsp에서 복사)
	vector<int> city;
	city.assign(origin_city.begin(), origin_city.end());

	// 시작도시 설정, 탐색할 도시에서 시작 도시 제거
	this->order.push_back(start);
	city.erase(city.begin() + start);

	while (city.size()) {
		nei_distance = DISTANCEMAX;

		for (unsigned int i = 0; i < city.size(); i++) {
			cur_distance = distance.at(
				tsp::calc_location(cur_city, city.at(i))
			);

			// 최근 이웃도시보다 더 거리가 짧은 도시 탐색
			if (cur_distance != 0 && cur_distance < nei_distance) {
				nei_city_idx = i;
				nei_distance = cur_distance;
			}
		}

		// 탐색한 도시 저장 및 남아있는 도시 목록에서 제거
		cur_city = city.at(nei_city_idx);
		this->order.push_back(cur_city);
		city.erase(city.begin() + nei_city_idx);
		// 탐색 거리 업데이트
		total_distance += nei_distance;
	}

	// 마지막 탐색 도시에서 원래 도시간의 이동 추가
	total_distance += distance.at(
		tsp::calc_location(cur_city, start)
	);


	// tour 정보 저장
	this->distance = total_distance;
	this->order.push_back(start);
}

void tour::debugCheckOrder() {
	bool dup = false;
	for (int i = 0; i < 1000; i++) {
		dup = false;

		if (i == this->order.at(0)) continue;

		for (int city : this->order) {
			if (dup && city == i) perror("dup city");
			if (city == i) dup = true;
		}
	}
}

void tour::calcDistance(vector<int> distance) {
	int from, to;
	this->distance = 0;

	for (int i = 1; i <= CITYNUM; i++) {
		
		from = this->order[i - 1];
		to = this->order[i];
		//cout << from << " " << to << " " << distance[tsp::calc_location(from, to)] << endl;
		this->distance += distance[tsp::calc_location(from, to)];
		//cout << this->distance << endl;
	}
}