#include <iostream>
#include <fstream>
#include <string>
#include <algorithm>
#include <sstream>
#include "time.h"
using namespace std;
const int num_city = 1000; // 도시개수
int temp_seq[num_city] = { 0 }; // 도시 방문 순서 임시저장
int min_seq[num_city] = { 0 }; // 최소 경로 도시 방문 순서 저장
int** cost = new int*[num_city]; // 2차원 배열에 cost저장

void optSwap(int start, int end) {
	for (int i = 0; i < start; i++) {
		temp_seq[i] = min_seq[i];
	}
	for (int i = start, j = 0; i < end + 1; i++, j++) {
		temp_seq[i] = min_seq[end - j];
	}
	for (int i = end + 1; i < num_city; i++) {
		temp_seq[i] = min_seq[i];
	}
	copy(temp_seq, temp_seq + num_city, min_seq);
}
void zeroswap() {
	int point;
	for (int i = 0; i < num_city; i++) {
		if (min_seq[i] == 0) {
			point = i;
			break;
		}
	}
	for (int i = point, j = 0; i < num_city; i++, j++) {
		temp_seq[j] = min_seq[i];
	}
	for (int i = 0, j = num_city - point; i < point; i++, j++) {
		temp_seq[j] = min_seq[i];
	}
	copy(temp_seq, temp_seq + num_city, min_seq);
}

void interswap(int* seq1, int* seq2) {
	int point, val;
	int temp[num_city];
	int visit[num_city] = { 0 };
	for (int i = 0; i < num_city; i++) {
		if (seq2[i] == 0) {
			point = i;
			break;
		}
	}
	for (int i = point, j = 0; i < num_city; i++, j++) {
		temp[j] = seq2[i];
		visit[seq2[i]] = 1; // 방문함
	}
	val = num_city - point;
	for (int i = 0; i < num_city; i++) {
		if (visit[seq1[i]] == 0) {
			temp[val] = seq1[i];
			val++;
			visit[seq1[i]] = 1;
		}
	}
	copy(temp, temp + num_city, min_seq);
}
int calTotalDistance(int* temp) {
	int total_distance = 0;
	for (int i = 0; i < num_city - 1; i++) {
		total_distance = total_distance + cost[temp[i]][temp[i + 1]];
	}
	return total_distance;
}

bool calculate() {
	int before, after;
	int total;
	for (int i = 0; i < num_city - 1; i++) {
		for (int j = i + 1; j < num_city; j++) {
			//optSwap(i, j);
			if (i == 0 && j != num_city - 1) {
				before = cost[min_seq[j]][min_seq[j + 1]];
				after = cost[min_seq[i]][min_seq[j + 1]];
			}
			else if (j == num_city - 1 && i != 0) {
				before = cost[min_seq[i - 1]][min_seq[i]] + cost[min_seq[j]][min_seq[0]];
				after = cost[min_seq[i - 1]][min_seq[j]] + cost[min_seq[i]][min_seq[0]];;
			}
			else if (i == 0 && j == num_city - 1) {
				continue;
			}
			else {
				before = cost[min_seq[i - 1]][min_seq[i]] + cost[min_seq[j]][min_seq[j + 1]];
				after = cost[min_seq[i - 1]][min_seq[j]] + cost[min_seq[i]][min_seq[j + 1]];
			}
			if (before > after) {
				optSwap(i, j);
				total = calTotalDistance(min_seq);
				return true;
			}
		}
	}
	return false;
}
int main() {
	srand((unsigned int)time(NULL));
	int temp2_seq[num_city] = { 0 }; // 도시 방문 순서 임시저장
	int temp3_seq[num_city] = { 0 }; // 도시 방문 순서 임시저장
	clock_t before = clock();
	char* token = NULL;
	char* context = NULL;
	int result_seq[num_city] = { 0 };
	int route[num_city];
	int final_cost = 999999;
	int total1, total2;
	char toke[] = ",\n";
	for (int i = 0; i < num_city; i++) {
		cost[i] = new int[num_city];
	}
	FILE* pFile = NULL;
	fopen_s(&pFile, "tsp_exp.txt", "r");
	if (pFile != NULL) {
		char buf[10000];
		for (int i = 0; i < num_city; i++) {
			fgets(buf, sizeof(buf), pFile);
			token = strtok_s(buf, toke, &context);
			cost[i][0] = atoi(token);
			for (int j = 1; j < num_city; j++) {
				token = strtok_s(NULL, toke, &context);
				cost[i][j] = atoi(token);
			}
		}
	}
	for (int k = 0; k < num_city; k++) {
		route[k] = k;
	}
	for (int i = 0; i < 70; i++){
		//무작위대입

		int n = num_city;
		int temp_val;
		for (int k = 0; k < num_city; k++) {
			int r = rand() % n;
			min_seq[k] = route[r];
			temp_val = route[r];
			route[r] = route[--n];
			route[n] = temp_val;
		}
		n = num_city;
		temp_val = 0;
		for (int k = 0; k < num_city; k++) {
			int r = rand() % n;
			temp2_seq[k] = route[r];
			temp_val = route[r];
			route[r] = route[--n];
			route[n] = temp_val;
		}
		for (int j = 0; j < 26; j++) {
			while (calculate()) {
				if (calTotalDistance(min_seq) < 6000)break;
			}
			total1 = calTotalDistance(min_seq);
			total2 = calTotalDistance(temp2_seq);
			if (total1 > total2) {
				interswap(min_seq, temp2_seq);
			}
			else if (total1 == total2) {
				continue;
			}
			else {
				copy(min_seq, min_seq + num_city, temp3_seq);
				interswap(temp2_seq, min_seq);
				copy(temp3_seq, temp3_seq + num_city, temp2_seq);
			}
		}
		for (int i = 0; i < 15; i++) {
			while (calculate()) {}
			zeroswap();
			if (final_cost >(calTotalDistance(min_seq) + cost[min_seq[num_city - 1]][0])) {
				final_cost = calTotalDistance(min_seq) + cost[min_seq[num_city - 1]][0];
				copy(min_seq, min_seq + num_city, result_seq);
				cout << final_cost << endl;
			}
		}
	}
	cout << "optimal solution : " << final_cost;

	ofstream outFile("result.txt");
	for (int i = 0; i < 1000; i++) {
		outFile << result_seq[i] + 1 << endl;
	}
	outFile << "1" << endl;
	outFile.close();

	for (int i = 1; i < num_city; i++) {
		delete[] cost[i];
	}
	delete[] cost;
	double result = (double)(clock() - before) / CLOCKS_PER_SEC;
	cout << "실행시간 : " << result << endl;
	return 0;
}
