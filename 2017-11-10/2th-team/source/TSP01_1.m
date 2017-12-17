tic;
load("tsp_exp.txt");
best_tour = []; best_dist = inf;
while(toc<=25)
    us = [2:1000];
    temp_tour(1,1) = 1;
    % set tour(2) be random
    tidx = round(rand()*(length(us)-1)+1);
    temp_tour(2,1) = us(tidx);
    us(tidx)=[];
    
    temp_dist = tsp_exp(temp_tour(1,1),temp_tour(2,1));
    % set tour(3 to 1000) based on least cost
    for k = 3:1000    
        % find least cost next city
        temp_cost = inf;
        temp_idx = 0;
 
        for i = 1: length(us)
           if(temp_cost>tsp_exp(temp_tour(k-1,1),us(i)))
              temp_cost = tsp_exp(temp_tour(k-1,1),us(i));
              temp_idx = i;
           end
        end
        % find least cost next city end
       
        % update dist and tour
       temp_dist = temp_dist + temp_cost;
       temp_tour(k,1) =  us(temp_idx);
       us(temp_idx) = [];
    end
    % tour(10001) should be 1
    temp_tour(1001,1)=1;
    temp_dist = temp_dist + tsp_exp(temp_tour(1000,1),1);
    
    % compare best dist
    if(temp_dist<best_dist)
        best_dist = temp_dist;
        best_tour = temp_tour;
    end
end

dlmwrite("TSP01_1.txt",best_tour);
toc