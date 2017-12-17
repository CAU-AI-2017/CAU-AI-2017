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

	for (int i = 0; i < 600; i++) {
		temp[i] = seq2[i];
		visit[seq2[i]] = 1; // 방문함
	}

	val = 600;
	for (int i = 0; i < num_city; i++) {
		if (visit[seq1[i]] == 0) {
			temp[val] = seq1[i];
			val++;
			visit[seq1[i]] = 1;
		}
	}
	/*
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
	*/

	copy(temp, temp + num_city, min_seq);
}

int calTotalDistance(int* temp) {
	int total_distance = 0;
	for (int i = 0; i < num_city; i++) {
		if (i == num_city - 1) {
			total_distance = total_distance + cost[temp[num_city - 1]][0];
		}
		else {
			total_distance = total_distance + cost[temp[i]][temp[i + 1]];
		}
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
	int temp1_seq[num_city] = { 0 }; // 도시 방문 순서 임시저장
	int temp2_seq[num_city] = { 0 }; // 도시 방문 순서 임시저장
	int temp3_seq[num_city] = { 0 }; // 도시 방문 순서 임시저장
	ifstream in("data.txt"); // 파일 읽어오기
	clock_t before = clock();
	char* token = NULL;
	char* context = NULL;
	int result_seq[num_city] = { 0 };
	int final_cost = 999999;
	int total1, total2;
	int val = 0;
	int visited[num_city] = { 0 };
	int visited2[num_city] = { 0 };
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

	// 변수 랜덤 초기화//

	for (int i = 0; i < 700; i++) {
		int num = rand() * 999 % 997;
		if (visited[num] == 0 && val < num_city) {
			visited[num] = 1;
			visited2[val] = 1;
			min_seq[val] = num;
			val = val + 2;
		}
	}

	for (int i = 0; i < 500; i++) {
		int num = rand() * rand() % 997;
		if (visited[num] == 0) {
			for (int j = 0; j < num_city; j++) {
				if (visited2[j] == 0) {
					visited[num] = 1;
					min_seq[j] = num;
					visited2[j] = 1;
					break;
				}
			}
		}
	}

	for (int i = 0; i < num_city; i++) {
		if (visited[i] == 0) {
			for (int j = 0; j < num_city; j++) {
				if (visited2[j] == 0) {
					visited[i] = 1;
					min_seq[j] = i;
					visited2[j] = 1;
					break;
				}
			}
		}
	}

	for (int i = 0; i < num_city; i++) {
		visited[i] = 0;
		visited2[i] = 0;
	}
	val = 0;


	for (int i = 0; i < 700; i++) {
		int num = rand() * 999 % 997;
		if (visited[num] == 0 && val < num_city) {
			visited[num] = 1;
			visited2[val] = 1;
			temp1_seq[val] = num;
			val = val + 2;
		}
	}

	for (int i = 0; i < 500; i++) {
		int num = rand() * rand() % 997;
		if (visited[num] == 0) {
			for (int j = 0; j < num_city; j++) {
				if (visited2[j] == 0) {
					visited[num] = 1;
					temp1_seq[j] = num;
					visited2[j] = 1;
					break;
				}
			}
		}
	}

	for (int i = 0; i < num_city; i++) {
		if (visited[i] == 0) {
			for (int j = 0; j < num_city; j++) {
				if (visited2[j] == 0) {
					visited[i] = 1;
					temp1_seq[j] = i;
					visited2[j] = 1;
					break;
				}
			}
		}
	}

	// 변수 랜덤 초기화//

	for (int count = 0; count < 10; count++) {
		for (int j = 0; j < 23; j++) {
			while (calculate()) {
				//if (calTotalDistance(min_seq) < 6000)break;
			}
			total1 = calTotalDistance(min_seq);
			total2 = calTotalDistance(temp1_seq);

			if (total1 > total2) {
				if (j > 15 && calTotalDistance(result_seq) > 4500) {
					for (int i = 0; i < num_city; i++) {
						visited[i] = 0;
						visited2[i] = 0;
					}
					val = 0;


					for (int i = 0; i < 700; i++) {
						int num = rand() * 999 % 997;
						if (visited[num] == 0 && val < num_city) {
							visited[num] = 1;
							visited2[val] = 1;
							temp1_seq[val] = num;
							val = val + 2;
						}
					}

					for (int i = 0; i < 500; i++) {
						int num = rand() * rand() % 997;
						if (visited[num] == 0) {
							for (int j = 0; j < num_city; j++) {
								if (visited2[j] == 0) {
									visited[num] = 1;
									temp1_seq[j] = num;
									visited2[j] = 1;
									break;
								}
							}
						}
					}

					for (int i = 0; i < num_city; i++) {
						if (visited[i] == 0) {
							for (int j = 0; j < num_city; j++) {
								if (visited2[j] == 0) {
									visited[i] = 1;
									temp1_seq[j] = i;
									visited2[j] = 1;
									break;
								}
							}
						}
					}
				}
				interswap(min_seq, temp1_seq);
			}
			else {
				copy(min_seq, min_seq + num_city, temp3_seq);
				interswap(temp1_seq, min_seq);
				copy(temp3_seq, temp3_seq + num_city, temp1_seq);
			}
		}

		for (int j = 0; j < 6; j++) {
			while (calculate()) {
				if (calTotalDistance(min_seq) < (8500 - (j * 500)))break;
			}
			copy(min_seq, min_seq + num_city, temp3_seq);
			zeroswap();

			for (int i = 0; i < 15; i++) {
				while (calculate()) {}
				zeroswap();
				if (final_cost > calTotalDistance(min_seq)) {
					final_cost = calTotalDistance(min_seq);
					copy(min_seq, min_seq + num_city, result_seq);
					cout << calTotalDistance(min_seq) << endl;
				}
			}
			copy(temp3_seq, temp3_seq + num_city, min_seq);
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
