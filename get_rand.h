#pragma once
#include <iostream>
#include <vector>
#include <utility>
#include <iterator>
#include <algorithm>
typedef long long ll;

#include "RANSAC.h"
#include <armadillo>

using namespace std;
using namespace arma;
bool * get_rand(int k, int N,ll &seed);