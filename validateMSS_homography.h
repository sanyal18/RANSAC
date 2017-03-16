#pragma once
#include <iostream>
#include <vector>
#include <armadillo>
#include"RANSAC.h"
typedef long long ll;
using namespace std;
using namespace arma;

bool sign(double x);
bool validateMSS_homography(mat X, vector<int> s);