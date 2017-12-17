tic;
load("tsp_exp.txt");
best_tour = []; best_dist = inf;


% set each tour to index number
while(toc<=25)
    us = [2:1000];
    temp_tour(1,1) = 1;
    % set tour(2) be random
    for k=2:1000
        tidx = round(rand()*(length(us)-1)+1);
        temp_tour(k,1) = us(tidx);
        us(tidx)=[];
    end
    % set last node to 1 for loop
    temp_tour(1001,1) = 1;
    %loc == tour index

    loc = 1;
    temp_dist = 0;
    
    while(loc < 1000)
        
        before_dist = inf;
        loc2 = loc+1;
        res = 0;
        while(loc2<1001)
            % loc2 == loc+1 to 1000
            % after_dist = cost for [loc] -> [loc2]
            after_dist = tsp_exp(temp_tour(loc,1),temp_tour(loc2,1));
            
            % find loc2 making the shorttest dist
            if(before_dist > after_dist)
                res = loc2;
                before_dist = after_dist;
            end
            loc2 = loc2+1;
        end
        %swap [loc+1] <-> [res]
        if(res ~= 0)
            tmp = temp_tour(loc+1,1);
            temp_tour(loc+1,1) = temp_tour(res,1);
            temp_tour(res,1) = tmp;
        end
        % loc++
        loc = loc +1;
    end
    
    for k=1:1000
        temp_dist = temp_dist+tsp_exp(temp_tour(k,1),temp_tour(k+1,1));
    end
    if(temp_dist<best_dist)
        best_tour = temp_tour;
        best_dist = temp_dist;
    end
    %toc
end
dlmwrite("TSP01_2.txt",best_tour);




