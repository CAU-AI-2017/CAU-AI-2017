#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <vector>
#include <algorithm>
#include <time.h>

using namespace std;

#define Input "tsp_exp.txt"
#define Output "tsp_result.txt"
#define Inf 100000

vector<vector<int> > cost;    // cost map
vector<int> result_path;      // result path

int result_sum_cost = Inf;
int result_start_node;
int result_path_index = 0;

int tsp(int start_node);
int tsp_main(int start_node);
int opt(int count);
int calc_cost();

int main(void){
  FILE *fin = NULL;
  FILE *fout = NULL;
  float taken_time;
  time_t start = 0, end = 0;

  int i, j;

  /* file open */
  if ((fin = fopen(Input, "r")) == NULL){
    fprintf(stdout, "Cannot open input file : %s", Input);
    return 0;
  }
  fout = fopen(Output, "w");

  /* initialize */
  cost.assign(1000, vector<int> (1000, 0));
  result_path.assign(1000, -1);

  for (i = 0; i < 1000; i++){
    for (j = 0; j < 1000; j++){
      fscanf(fin, "%d,", &cost[i][j]);
    }
    fscanf(fin, "\n");
  }

  /* TSP exercise */
  start = clock();
  for (i = 0; i < 1000; i++/* 연산 바꿔서 */){
    tsp(i);
  }
  tsp_main(result_start_node);
  opt(8000);
  result_sum_cost = 0;
  calc_cost();
  end = clock();
  taken_time = (float)(end - start) / (CLOCKS_PER_SEC);

  fprintf(stdout, "taken time : %fs\n", taken_time);
  fprintf(stdout, "cost : %d\n", result_sum_cost);
  /* file write */
  /*  start to 0
      change the path   */
  int temp;
  while(result_path[0] != 0){
    temp = result_path[0];
    result_path.erase(result_path.begin());
    result_path.push_back(temp);
  }

  for (i = 0; i < 1000; i++){
    result_path[i]++;
    fprintf(fout,"%d\n", result_path[i]);
  }
  fprintf(fout, "1\n");

  fclose(fin);
  fclose(fout);
  return 0;
}

int tsp_main(int start_node){
  int i;      // iterative variables
  vector<int> temp_path;

  int visited_index = -1;
  int temp_path_erase_index = -1;
  int min_cost = Inf;

  int current_node = start_node;

  for (i = 0; i < 1000; i++){
    temp_path.push_back(i);
  }

  temp_path.erase(temp_path.begin() + start_node);
  result_path[result_path_index++] = start_node;

  while (temp_path.size() != 0){
    for (i = 0; i < temp_path.size(); i++){
      if (cost[current_node][temp_path[i]] != 0 && min_cost > cost[current_node][temp_path[i]]){
        visited_index = temp_path[i];
        min_cost = cost[current_node][temp_path[i]];
        temp_path_erase_index = i;
      }
    }

    result_path[result_path_index++] = visited_index;
    current_node = visited_index;
    temp_path.erase(temp_path.begin() + temp_path_erase_index);

    temp_path_erase_index = -1;
    visited_index = -1;
    min_cost = Inf;
  }

  temp_path.clear();
  return 0;
}

int tsp(int start_node){
  int i;      // iterative variables
  vector<int> temp_path;
  int temp_result_sum_cost = 0;

  int visited_index = -1;
  int temp_path_erase_index = -1;
  int min_cost = Inf;

  int current_node = start_node;

  for (i = 0; i < 1000; i++){
    temp_path.push_back(i);
  }

  temp_path.erase(temp_path.begin() + start_node);

  while (temp_path.size() != 0){
    for (i = 0; i < temp_path.size(); i++){
      if (cost[current_node][temp_path[i]] != 0 && min_cost > cost[current_node][temp_path[i]]){
        min_cost = cost[current_node][temp_path[i]];
        visited_index = temp_path[i];
        temp_path_erase_index = i;
      }
    }

    temp_result_sum_cost += min_cost;
    current_node = visited_index;
    temp_path.erase(temp_path.begin() + temp_path_erase_index);

    temp_path_erase_index = -1;
    min_cost = Inf;
  }

  temp_result_sum_cost += cost[current_node][start_node];

  /* cost swap */
  if (temp_result_sum_cost < result_sum_cost){
    result_start_node = start_node;
    result_sum_cost = temp_result_sum_cost;
  }

  temp_path.clear();
  return 0;
}

int opt(int count){
  int i, j;
  int max_change = 1;
  int index_i = -1, index_j = -1;
  int change = 0;

  while (count > 0 && max_change > 0){
    max_change = 0;
    for (i = 0; i < result_path.size() - 3; i++){
      for (j = i + 2; j < result_path.size() - 1; j++){
        change = cost[result_path[i]][result_path[i+1]] + cost[result_path[j]][result_path[j+1]]
        - cost[result_path[i]][result_path[j]] - cost[result_path[i+1]][result_path[j+1]];
        if (change > 0 && max_change < change){
          max_change = change;
          index_i = i;
          index_j = j;
        }
      }
    }

    /* change path */
    if (index_i != -1 && index_j != -1 && max_change != 0){
      int temp = result_path[index_j];
      result_path[index_j] = result_path[index_i+1];
      result_path[index_i+1] = temp;
    }

    index_i = -1;
    index_j = -1;
    count--;
  }

  return 0;
}

int calc_cost(){
  int i;
  for (i = 0; i < result_path.size() - 1; i++){
    result_sum_cost += cost[result_path[i]][result_path[i+1]];
  }

  result_sum_cost += cost[result_path[i]][result_path[0]];

  return 0;
}
