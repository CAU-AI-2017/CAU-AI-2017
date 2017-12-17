#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#pragma warning(disable: 4996)
#define MAX_CITY 1000

int main() {
	FILE *f = fopen("input.txt", "r+");
	FILE *f2 = fopen("output.txt", "w");
	int j;
	char buff[5000];
	char deli[] = ",";
	char *token;
	int **arr = new int*[MAX_CITY];
	for (int i = 0; i < MAX_CITY; i++)
		arr[i] = new int[MAX_CITY];

	if (f == NULL){
		printf("���� ���߽��ϴ�. \n");
		return 0;
	}
	for (int i = 0; i < MAX_CITY; i++) {
		fgets(buff, sizeof(buff), f);
		token = strtok(buff, deli);
		j = 0;
		while (token != NULL) {
			/*fprintf(f2, "%s ", token);*/
			arr[i][j] = atoi(token);
			token = strtok(NULL, deli);
			j++;
		}
	}
	fclose(f);
	int min = 1000;
	int min2 = 0;
	int x = 0;
	int y = 0;
	int li = 0;//  line array�� ���� ����
	int next1 = 0;
	int temp = 0;
	int visited[MAX_CITY] = { 0 };
	int line1[499] = { 0 };
	int line2[499] = { 0 };
	int final[1001] = { 0 };
	int count = 0;
	int end = 0;// �߰� ������ �Ǵ� 500��° ��
	////1��° ���� �̹� �湮
	visited[0] = 1;

	for (y = 0; y < MAX_CITY; y++)
	{
		if (0 < arr[x][y] && arr[x][y] < min)
		{
			min2 = min;
			min = arr[x][y];
		}
	} ///// 0,y�� �ּڰ� ã��
	y = 0;
	////min = min2 �ΰ��
	if (min2 == min)
	{
		while (count < 2)
		{
			if (arr[x][y] == min && visited[y] == 0) /// ������ min�� 0�� �ƴϱ� ������ x,y���� ������ �ƴ��� �񱳾��ص� ��
			{
				next1 = y;   // �ι�° min
				visited[y] = 1;
				count++;
				if (count == 2){
					line2[li] = y + 1;
				}
				y++;
			}
			else if (arr[x][y] != min)
				y++;
			if (count == 1){
				temp = next1;  // ù��° min
				line1[li] = y + 1;
			}
		}
		count = 0;
		y = 0;
	}
	////min != min2 �� ��� (min = next1 �� min2 = temp�� �������ش�!)
	else if (min2 != min)
	{
		while (count == 0)
		{
			if (arr[x][y] == min && visited[y] == 0)
			{
				next1 = y;
				line1[li] = y + 1;
				visited[y] = 1;
				count++;
				y++;
			}
			else if (arr[x][y] != min)
				y++;
		}
		//////�ʱ�ȭ
		count = 0;
		y = 0;
		//////�ʱ�ȭ
		while (count == 0)
		{
			if (arr[x][y] == min2 && visited[y] == 0)
			{
				temp = y;
				line2[li] = y + 1;
				visited[y] = 1;
				count++;
				y++;
			}
			else if (arr[x][y] != min2)
				y++;
		}
	}
	/////////ù��° �ܰ� ��
	li++;
	///���� li �� 1�̴�.
	/////////�״����� �ݺ��ؼ� ã�ٰ� 999���� �Ǹ� finish �Ѵ�.
	int k = 3;
	while (k < 999)
	{
		//////�ʱ�ȭ
		y = 0;
		min = 1000;
		count = 0;
		/////
		x = next1;// ���� �����ش�. �״��� temp ���� �����ش�.
		for (y = 0; y < MAX_CITY; y++)
		{
			if (0 < arr[x][y] && arr[x][y] < min && visited[y] == 0){
				min = arr[x][y];
				next1 = y;
				line1[li] = y + 1;
			}
		}
		visited[next1] = 1;
		//////�ʱ�ȭ
		y = 0;
		min = 1000;
		count = 0;
		//////

		x = temp;
		for (y = 0; y < MAX_CITY; y++)
		{
			if (arr[x][y] > 0 && arr[x][y] < min && visited[y] == 0){
				min = arr[x][y];
				temp = y;
				line2[li] = y + 1;
			}
		}
		visited[temp] = 1;
		//////�ʱ�ȭ
		k++;
		k++;
		li++;
	}
	///1�־��ֱ�

	int c = 0;
	final[c] = 0;
	/// ù ���� 1
	////line1 �Է�
	for (int q = 0; q < 499; q++)
	{
		final[q + 1] = line1[q] - 1;
	}

	///�߰� ������ �Է�
	for (y = 0; y < MAX_CITY; y++)
	{
		if (visited[y] == 0)
		{
			end = y;
			final[500] = end;
			break;
		}
	}
	c = 501;
	////line2 �Է�
	for (int q = 498; q >= 0; q--)
	{
		final[c] = line2[q] - 1;
		c++;
	}
	final[MAX_CITY] = 0;
	int total_length = 0;
	for (int o = 0; o < MAX_CITY; o++){
		total_length = total_length + arr[final[o]][final[o + 1]];
	}

	int first_distance = 0;	//�ϼ��� path�� �� length
	int best_distance;
	int local_distance;
	int swap_first = 0;
	int swap_last = MAX_CITY;
	int noMoreFlag = 0;
	int invertPathlength;
	int swap_first_index;
	int swap_last_index;
	int temp2;
	first_distance = total_length;
	best_distance = first_distance;
	while (noMoreFlag == 0) {
		noMoreFlag = 1;
		local_distance = first_distance;
		for (int i = 1; i < MAX_CITY - 1; i++) {
			for (int j = i + 1; j < MAX_CITY; j++) {
				local_distance = local_distance - arr[final[i - 1]][final[i]] + arr[final[i - 1]][final[j]]
					- arr[final[j]][final[j + 1]] + arr[final[i]][final[j + 1]];
				if (local_distance < best_distance) {
					best_distance = local_distance;
					swap_first = final[i];
					swap_last = final[j];
					swap_first_index = i;
					swap_last_index = j;
					noMoreFlag = 0;
				}
				local_distance = first_distance;
			}
		}
		if (noMoreFlag == 0) {
			invertPathlength = swap_last_index - swap_first_index + 1;
			//final array ������
			for (int k = 0; k < invertPathlength / 2; k++) {
				temp2 = final[swap_last_index - k];
				final[swap_last_index - k] = final[swap_first_index + k];
				final[swap_first_index + k] = temp2;
			}
		}
		first_distance = 0;
		for (int i = 0; i < MAX_CITY; i++)
			first_distance += arr[final[i]][final[i + 1]];
	}

	for (int c = 0; c < 1001; c++)
	{
		fprintf(f2, "%d\n", final[c] + 1);
	}
	/*
	printf("%d", first_distance);
	*/
	return 0;
}