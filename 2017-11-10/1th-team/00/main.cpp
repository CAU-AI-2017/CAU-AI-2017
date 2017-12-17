#include "tsp.h"
#include "tour.h"

int  main() {

	tsp algo("tsp_exp.txt");
	
	algo.run(false);

	return 0;
}