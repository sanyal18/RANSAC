#pragma once
#include <iostream>
#include <vector>
#include <utility>
#include <armadillo>
using namespace std;
using namespace arma;

pair<mat, mat> normalize_points(mat x);