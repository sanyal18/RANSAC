#pragma once
#include <iostream>
#include <vector>
#include <cmath>
#include <armadillo>
using namespace std;
using namespace arma;

mat mapping_homography(vec xs, vec ys, bool mode, mat Theta, int N);
