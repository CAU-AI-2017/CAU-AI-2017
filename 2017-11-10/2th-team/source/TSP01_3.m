tic;
load("tsp_exp.txt");
best_tour = []; best_dist = inf;

% whole while start
while(toc<=25)
    % set initial tour using greedy
    % second city will be decided by random
    %
    us = [2:1000];
    initial_tour(1,1) = 1;
    initial_dist = 0;
    % set second city based on random
    tidx2 = round(rand()*(length(us)-1)+1);
    initial_tour(2,1) = us(tidx2);
    us(tidx2)=[];


    % set tour(3 to 1000) based on least cost
    for k = 3:1000    
        % find least cost next city
        initial_cost = inf;
        initial_idx = 0;
        for i = 1: length(us)
           if(initial_cost>tsp_exp(initial_tour(k-1,1),us(i)))
              initial_cost = tsp_exp(initial_tour(k-1,1),us(i));
              initial_idx = i;
           end
        end
        % find least cost next city end
       
        % update dist and tour
       initial_dist = initial_dist + initial_cost;
       initial_tour(k,1) =  us(initial_idx);
       us(initial_idx) = [];
    end
    % tour(10001) should be 1
    initial_tour(1001,1)=1;
    initial_dist = initial_dist + tsp_exp(initial_tour(1000,1),1);

    if(initial_dist<best_dist)
        best_tour = initial_tour;
        best_dist = initial_dist;
    end
% set initial tour using greedy end

% init temperature as 1000
temperature = 1000;

temp_tour1 = initial_tour;
temp_dist1 = initial_dist;
us = [2:1000];
temp_tour2 = temp_tour1;
temp_dist2 = 0;
    
% SA start
    while(toc<=25 && temperature>10^-6)
        % find max cost city
        max = 0;
        max_idx=0;
        for i = 2:1000
            if(max<tsp_exp(temp_tour2(i-1,1),temp_tour2(i,1)) + tsp_exp(temp_tour2(i,1),temp_tour2(i+1,1)))
                max_idx = i;
                max = tsp_exp(temp_tour2(i-1,1),temp_tour2(i,1)) + tsp_exp(temp_tour2(i,1),temp_tour2(i+1,1));
            end
        end
        % find max cost city end
        
        tidx1 = max_idx;
        % get random city idx
        tidx2 = round(rand()*(length(us)-1)+1);
        % swap two cities
        temp_city = temp_tour2(tidx1,1);
        temp_tour2(tidx1,1) = temp_tour2(tidx2,1);
        temp_tour2(tidx2,1) = temp_city;    
    
        % calculate dist
        for k =1:1000
            temp_dist2 = temp_dist2 + tsp_exp(temp_tour2(k,1),temp_tour2(k+1,1));
        end
        % check if can be accepted
        if(rand() < exp((best_dist - temp_dist2)/temperature))
            temp_tour1 = temp_tour2;
            temp_dist1 = temp_dist2;
        end
        % temperature down
        temperature = temperature * 0.95;
    
        if(temp_dist1<best_dist)
            best_tour = temp_tour1;
            best_dist = temp_dist1;
        end
    end
% SA end
end
% whole while end

dlmwrite("TSP01_3.txt",best_tour);
toc