#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#pragma warning(disable: 4996)
#define MAX_CITY 1000

int main() {
	clock_t before;
	double result;
	before = clock();

	FILE *f = fopen("input.txt", "r+");

	int visited[MAX_CITY] = { 0 };

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
	int shortest_city;
	int best_length = 99999;
	int path[MAX_CITY + 1] = { 0 };
	visited[0] = 1;

	for (int i = 0; i < MAX_CITY; i++) {
		for (int j = 0; j < MAX_CITY; j++) {
			if (arr[path[i]][j] < best_length && visited[j] == 0 && arr[path[i]][j] != 0) {
				best_length = arr[path[i]][j];
				shortest_city = j;
			}
		}
		visited[shortest_city] = 1;
		path[i + 1] = shortest_city;
		best_length = 99999;
	}
	FILE *f2;
	f2 = fopen("output.txt", "w");

	//int total_length = 0;
	//printf("경로는\n");
	for (int i = 0; i < MAX_CITY; i++) {
		//printf("%d->", path[i]+1);
		fprintf(f2, "%d \n", path[i] + 1);
	}
	//printf("1 \n");
	fprintf(f2, "1");
	/*
	for (int i = 0; i < MAX_CITY-1; i++) {
	total_length += arr[path[i]][path[i+1]];
	}
	total_length += arr[path[MAX_CITY-1]][0];
	printf("총 길이는 %d 입니다. \n", total_length);
	for (int i = 0; i < MAX_CITY - 1; i++) {
	total_length += arr[i][i + 1];
	}
	total_length += arr[MAX_CITY-1][0];
	printf("1부터 1000까지 순서대로 방문하면 길이는 %d 입니다. \n", total_length);
	*/

	fclose(f);
	fclose(f2);

	result = (double)(clock() - before) / CLOCKS_PER_SEC;

	printf("걸린시간은 %5.2f 초 입니다.\n", result);
}