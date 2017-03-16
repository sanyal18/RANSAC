#pragma once
#include <armadillo>
#include <iostream>
using namespace arma;
using namespace std;
pair< mat, bool * > get_consensus_set(mat X, mat Theta_hat,
	double T_noise_squared, int N);