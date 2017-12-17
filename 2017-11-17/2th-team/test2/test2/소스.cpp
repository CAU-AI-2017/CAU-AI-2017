#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

#pragma warning(disable: 4996)
#define MAX_CITY 1000

void saveFile(int path[]) {
	FILE *f2;
	f2 = fopen("output.txt", "w");
	//fprintf(f2, "1");
	for (int i = 0; i < MAX_CITY + 1; i++)
		fprintf(f2, "%d \n", path[i] + 1);
	fclose(f2);
}

int main() {
	printf("실행중...");
	clock_t before;
	double time_result;
	before = clock();

	FILE *f = fopen("input.txt", "r+");

	int visited[MAX_CITY] = { 0 };
	int shortest_city;
	int best_length = 999999;
	int path[MAX_CITY + 1] = { 0 };

	int first_distance = 0;
	int best_distance;
	int local_distance;
	int swap_first = 0;
	int swap_last = MAX_CITY;


	char buff[5000];
	char deli[] = ",";
	char *token;
	int **arr = new int*[MAX_CITY];
	for (int i = 0; i < MAX_CITY; i++)
		arr[i] = new int[MAX_CITY];

	if (f == NULL) {
		printf("열지 못했습니다. \n");
		return 0;
	}
	for (int i = 0; i < MAX_CITY; i++) {
		fgets(buff, sizeof(buff), f);
		token = strtok(buff, deli);
		int j = 0;
		while (token != NULL) {
			arr[i][j] = atoi(token);
			token = strtok(NULL, deli);
			j++;
		}
	}
	visited[0] = 1;
	for (int i = 0; i < MAX_CITY - 1; i++) {
		for (int j = 0; j < MAX_CITY; j++) {
			if (arr[path[i]][j] < best_length && visited[j] == 0 && arr[path[i]][j] != 0) {
				best_length = arr[path[i]][j];
				shortest_city = j;
			}
		}
		visited[shortest_city] = 1;
		path[i + 1] = shortest_city;
		best_length = 999999;
	}
	path[MAX_CITY] = 0;	//마지막 목적지는 1로
	for (int i = 0; i < MAX_CITY; i++) {
		first_distance += arr[path[i]][path[i + 1]];
	}
	//printf("knn's first_length: %d \n", first_distance);
	//first_distance += arr[path[MAX_CITY - 1]][0];
	//여기서부터 2opt로
	best_distance = first_distance;

	int invertPathlength;
	int swap_first_index;
	int swap_last_index;
	int temp;
	int noMoreFlag = 0;
	int iterman = 0;

	double temperature = 2500;
	double coolingRate = 0.003;
	srand(time(NULL));
	double random = rand() / RAND_MAX;

	int check = 1;

	//while (noMoreFlag == 0) {
	while (iterman < 60) {	//반복 횟수 조정하자
		noMoreFlag = 1;
		local_distance = first_distance;
		for (int i = 1; i < MAX_CITY - 1; i++) {
			for (int j = i + 1; j < MAX_CITY; j++) {
				local_distance = local_distance - arr[path[i - 1]][path[i]] + arr[path[i - 1]][path[j]]
					- arr[path[j]][path[j + 1]] + arr[path[i]][path[j + 1]];
				if (local_distance < best_distance) {
					best_distance = local_distance;
					swap_first = path[i];
					swap_last = path[j];
					swap_first_index = i;
					swap_last_index = j;
					noMoreFlag = 0;
				}
				else if (exp((best_distance - local_distance) / temperature) > random) {
					best_distance = local_distance;
					swap_first = path[i];
					swap_last = path[j];
					swap_first_index = i;
					swap_last_index = j;
					noMoreFlag = 0;
				}
				local_distance = first_distance;
				temperature *= 1 - coolingRate;
			}
		}
		if (noMoreFlag == 0) {
			invertPathlength = swap_last_index - swap_first_index + 1;
			//path 뒤집기
			for (int k = 0; k < invertPathlength / 2; k++) {
				temp = path[swap_last_index - k];
				path[swap_last_index - k] = path[swap_first_index + k];
				path[swap_first_index + k] = temp;
			}
		}
		first_distance = 0;
		for (int i = 0; i < MAX_CITY; i++)
			first_distance += arr[path[i]][path[i + 1]];
		//printf("%d 번 실행\t거리 = %d\n", check, first_distance);
		check++;
		iterman++;
	}

	//printf("first_length: %d \n", first_distance);
	//printf("length: %d \n", best_distance);
	saveFile(path);
	/*
	int realLength = 0;
	for (int i = 0; i < MAX_CITY; i++) {
	realLength += arr[path[i]][path[i + 1]];
	//printf("길이는: %d \n", arr[path[i]][path[i + 1]]);
	}
	printf("진짜 길이는 %d \n", realLength);
	*/

	fclose(f);

	time_result = (double)(clock() - before) / CLOCKS_PER_SEC;

	printf("\n걸린시간은 %5.2f 초 입니다. \n", time_result);


}