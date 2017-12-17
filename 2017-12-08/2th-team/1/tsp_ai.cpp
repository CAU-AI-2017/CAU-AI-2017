#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <vector>
#include <time.h>

#define NODE_NUM 1000
#define MAX_DISTANCE_NUM 4

using namespace std;

int distanceMap[NODE_NUM][NODE_NUM];
int cost = 0;

typedef struct node {
	int index;
	bool isVisited;
} NODE;

int calCost(vector<NODE> sequence) {
	int tempCost = 0;

	for (int i = 0; i < sequence.size() - 1; i++) {
		tempCost += distanceMap[sequence[i].index][sequence[i + 1].index];
	}

	tempCost += distanceMap[sequence[sequence.size() - 1].index][sequence[0].index];

	return tempCost;
}

void readMap(char* fileName) {
	FILE* fp = fopen(fileName, "r");

	char* line = (char*)malloc(sizeof(char)*MAX_DISTANCE_NUM*NODE_NUM);
	char* strNum;
	int num;
	int i = 0, j = 0;

	while (!feof(fp)) {
		fscanf(fp, "%s\n", line);
		strNum = strtok(line, ",");

		while (strNum != NULL) {
			distanceMap[i][j] = atoi(strNum);
			j++;
			strNum = strtok(NULL, ",");
		}
		i++;
		j = 0;
	}

	fclose(fp);
}

void writeResult(vector<NODE> result) {
	FILE* fp = fopen("result.txt", "w");

	for (int i = 0; i < result.size(); i++) {
		fprintf(fp, "%d\n", result[i].index + 1);
	}

	fprintf(fp, "%d\n", result[0].index + 1);

	fclose(fp);
}

vector<NODE> findSequence(vector<NODE> remainedNodeSet, int startIndex) {
	vector<NODE> result;
	int minDistance;
	int minIndex;
	int searchDistance;

	NODE currentNode;
	currentNode.index = startIndex;
	currentNode.isVisited = true;
	result.push_back(currentNode);
	remainedNodeSet[currentNode.index].isVisited = true;

	minIndex = currentNode.index;

	while (true) {
		minDistance = 10000;

		for (int i = 0; i < remainedNodeSet.size(); i++) {
			if (i != currentNode.index && remainedNodeSet[i].isVisited == false) {
				searchDistance = distanceMap[currentNode.index][remainedNodeSet[i].index];

				if (searchDistance < minDistance) {
					minDistance = searchDistance;
					minIndex = i;
				}
			}
		}

		if (minIndex != currentNode.index) {
			currentNode.index = minIndex;
			currentNode.isVisited = true;
			result.push_back(currentNode);
			cost += searchDistance;
			remainedNodeSet[minIndex].isVisited = true;
		}
		else {
			break;
		}
	}

	cost += distanceMap[result[0].index][(result[result.size() - 1].index)];

	return result;
}

vector<NODE> twoOptSwap(vector<NODE> route, int i, int k) {
	vector<NODE> newRoute;
	NODE temp;

	for (int idx = 0; idx < i; idx++) {
		temp.index = route[idx].index;
		newRoute.push_back(temp);
	}

	for (int idx = k; idx >= i; idx--) {
		temp.index = route[idx].index;
		newRoute.push_back(temp);
	}

	for (int idx = k + 1; idx < route.size(); idx++) {
		temp.index = route[idx].index;
		newRoute.push_back(temp);
	}

	return newRoute;
}

vector<NODE> twoOptAlgorithm(vector<NODE> route) {
	bool isImproved = true;
	int bestDistance = calCost(route);
	int newDistance;
	vector<NODE> existingRoute = route;
	vector<NODE> newRoute;

	int searchSize = NODE_NUM;

	int i = 1;

	while (isImproved) {
	restart:
		isImproved = false;

		for (; i < searchSize - 1; i++) {
			for (int k = i + 1; k < searchSize; k++) {
				newRoute = twoOptSwap(existingRoute, i, k);
				newDistance = calCost(newRoute);

				if (newDistance < bestDistance) {
					existingRoute = newRoute;
					//printf("%-6s: %d", "######", newDistance);
					bestDistance = newDistance;
					isImproved = true;
					goto restart;
				}
			}
		}
	}

	return existingRoute;
}

vector<NODE> rearrangeRoute(vector<NODE> route) {
	vector<NODE> returnRoute;
	NODE newNode;

	int start = 0;
	int idx = 0;

	for (int i = 0; i < route.size(); i++) {
		if (route[i].index == 0) {
			start = i;
		}
	}

	idx = start;

	for (int i = 0; i < route.size(); i++) {
		if (idx == NODE_NUM) {
			idx = 0;
		}

		newNode.index = route[idx].index;
		returnRoute.push_back(newNode);
		idx++;
	}

	return returnRoute;
}

int main(void) {
	char* fileName = "tsp_exp.txt";
	readMap(fileName);

	srand((unsigned int)time(NULL));

	vector<NODE> nodeSet;

	for (int i = 0; i < NODE_NUM; i++) {
		NODE temp;
		temp.index = i;
		temp.isVisited = false;
		nodeSet.push_back(temp);
	}

	time_t start = 0, end = 0;
	float exeTime;

	vector<NODE> result;
	vector<NODE> minResult;
	int minCost = -1;

	start = clock();

	for (int i = 0; i < NODE_NUM; i++) {
		cost = 0;
		result = findSequence(nodeSet, i);
		if (cost < minCost || minCost < 0) {
			minResult = result;
			minCost = cost;
		}
	}
	result = twoOptAlgorithm(minResult);
	result = twoOptAlgorithm(result);

	for (int i = 0; i < 5; i++) result = twoOptAlgorithm(result);

	result = rearrangeRoute(result);
	minCost = calCost(result);

	end = clock();
	exeTime = (float)(end - start) / CLOCKS_PER_SEC;

	printf("%-6s: %fs\n", "Time", exeTime);
	printf("%-6s: %d\n", "Cost", minCost);

	printf("%-6s: %d\n", "Start", result[0].index + 1);
	printf("%-6s: %d\n", "End", result[NODE_NUM - 1].index + 1);

	/*for (int i = 0; i < result.size(); i++) {
	printf("%d\n", result[i].index);
	}*/

	writeResult(result);
	return 0;
}