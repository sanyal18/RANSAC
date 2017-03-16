#pragma once
#include <iostream>
#include <armadillo>
using namespace std;
using namespace arma;

double get_consensus_set_rank(bool * CS, mat E, bool mode,
	double T_noise_squared);
