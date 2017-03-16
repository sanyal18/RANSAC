#pragma once
#include <iostream>
#include <vector>
#include <armadillo>
#include "HomographyDLT.h"

using namespace std;
using namespace arma;


pair<mat, int>estimate_homography(mat X, std::vector<int> s);