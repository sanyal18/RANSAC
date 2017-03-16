#pragma once
#include <iostream>
#include <vector>
#include <utility>
#include <armadillo>
typedef long long ll;
using namespace arma;
using namespace std;
pair< vector<int>, mat> get_minimal_sample_set(int k, mat X, vector<int> ind_tabu,ll &seed);