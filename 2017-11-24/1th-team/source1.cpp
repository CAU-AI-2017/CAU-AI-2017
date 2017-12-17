#include <iostream>

#include <fstream>

#include <string>

#include <sstream>

#include "time.h"

using namespace std;



int main() {



	ifstream in("tsp_exp.txt"); // ���� �о����

	const int num_city = 1000; // ���ð���



	int total_cost = 0; // ����� 1���� ���� ���

	int min_total_cost = 9999999; // ��� ������߿� �ּڰ� ����

	int minCost = 999999; // �ּҰ�θ� ����

	int value = 1, count = 0;

	int tPath = 0, tPath2; //��� �ӽ����� ����

	int val1 = 2; // while�� ����

	int** cost = new int*[num_city]; // 2���� �迭�� cost����

	int visited[num_city] = { 1, 0 }; // ���� �湮 ����, �湮 = 1;

	int seq[num_city] = { 0 }; // ���� �湮 ���� �ӽ�����

	int min_seq[num_city] = { 0 }; // �ּ� ��� ���� �湮 ���� ����

	string line;

	clock_t before = clock();



	for (int i = 0; i < num_city; i++) {

		cost[i] = new int[num_city];

	}



	// ���� �о����

	if (in.is_open()) {

		int x = 0, y = 0, i;

		while (!in.eof()) {

			getline(in, line);

			line += ",";

			stringstream ss(line);



			char ch;

			while (ss >> i >> ch) {



				cost[x][y] = i;

				y++;

			}

			y = 0;

			x++;

		}

	}

	else {

		cout << "������ ã�� �� �����ϴ�!" << endl;

	}





	while (val1 < 300) {

		while (count < 999) {

			for (int i = 0; i < num_city; i++) {

				if (visited[i] == 0) {

					//�ּҰ����� �湮

					if (minCost > cost[seq[value - 1]][i] && seq[value - 1] != i) {

						minCost = cost[seq[value - 1]][i];

						tPath2 = i;

					}

				}

			}

			tPath = tPath2;



			if (count % val1 == 0) {

				minCost = 999999;

				for (int i = 0; i < num_city; i++) {

					if (visited[i] == 0) {

						//�ּҰ����� �湮

						if (minCost > cost[seq[value - 1]][i] && seq[value - 1] != i && i != tPath) {

							minCost = cost[seq[value - 1]][i];

							tPath2 = i;

						}

					}

				}

			}

			seq[value++] = tPath2;

			visited[tPath2] = 1;

			total_cost = total_cost + minCost;

			minCost = 999999;

			count++;

		}

		total_cost = total_cost + cost[seq[999]][0];

		if (min_total_cost > total_cost) {

			min_total_cost = total_cost;

			for (int i = 0; i < num_city; i++) {

				min_seq[i] = seq[i];

			}

		}



		//�ʱ�ȭ

		total_cost = 0;

		value = 1;

		count = 0;

		for (int i = 0; i < num_city; i++) {

			seq[i] = 0;

			visited[i] = 0;

		}

		visited[0] = 1;

		val1++;

	}



	cout << "�ּҰ�� ��� : " << min_total_cost << endl;



	ofstream outFile("result.txt");

	for (int i = 0; i < 1000; i++) {

		outFile << min_seq[i] + 1 << endl;

	}

	outFile << "1" << endl;

	outFile.close();







	for (int i = 1; i < num_city; i++) {

		delete[] cost[i];

	}

	delete[] cost;



	double result = (double)(clock() - before) / CLOCKS_PER_SEC;



	cout << "����ð� : " << result << endl;



	return 0;

}
