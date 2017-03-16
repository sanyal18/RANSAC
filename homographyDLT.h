#pragma once
#include <iostream>
#include <vector>
#include <utility>
#include <armadillo>
using namespace std;
using namespace arma;
mat get_A_MZ(mat X1, mat X2);
pair<mat, mat> HomographyDLT(mat X1, mat X2, bool normalization);