#pragma once
#include <iostream>
#include <vector>
#include <armadillo>
using namespace std;
using namespace arma;
bool validateTheta_homography(mat X, mat Theta, vector<int>s);