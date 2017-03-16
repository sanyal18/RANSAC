#pragma once
#include <iostream>
#include <vector>
#include <cmath>
#include <armadillo>
#include "chi2inv.cpp"
using namespace std;
using namespace arma;


void generate_chi2inv_LUT() {
	double v_LUT[8];
	for(int i = 0;i < 8;i++) {
		v_LUT[i] = i + 1;
	}
    vec p_LUT = linspace<vec>(0.5, 1 - exp(1), 512);

	mat x_LUT(512,8);

	for(int h = 0;h < 512; ++h) {
		for(int k = 0;k < 8; ++k) {
			x_LUT(h,k) = chi2inv(p_LUT[h], v_LUT[k]);
		}
	}
	//We Also need to save all these arrays;
}

