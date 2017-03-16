#include <iostream>
#include <vector>
#include <cmath>
#include <armadillo>
#include "get_consensus_set_rank.h"
using namespace std;
using namespace arma;

double get_consensus_set_rank(bool * CS, mat E, bool mode,
		double T_noise_squared) {

	int N = E.n_cols;
	double r = 0;

	for (int i = 0; i < N; i++) {
		if (E(i,0) <= T_noise_squared)
			r += T_noise_squared;
	}
	r = -r / N;

	//if(mode) {
		//for(int i = 0;i < N; ++i) {
			//if(CS[i])
				//r++;
		//}
	//}
	//else {
		//for(int i = 0;i  < N; ++i) {
			//if(E(i,0) >= T_noise_squared) {
				//r += T_noise_squared;
			//}
		//}
		//r = -r/N;
	//}

	return r;
}