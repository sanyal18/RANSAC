#pragma once
#include <iostream>
#include <vector>
#include <armadillo>
#include "chi2inv_LUT.h"
#include "chi2inv.h"
using namespace std;
using namespace arma;

double chi2inv_LUT(double p, int v) {


	double* v_LUT= new double[8];
	for (int i = 0; i < 8; i++) {
		v_LUT[i] = i + 1;
	}
	vec p_LUT = linspace<vec>(0.5, 1 - pow(10,1), 512);

	mat x_LUT(512, 8);

	for (int h = 0; h < 512; ++h) {
		for (int k = 0; k < 8; ++k) {
			x_LUT(h, k) = chi2inv(p_LUT[h], v_LUT[k]);
		}
	}
	//We Also need to save all these arrays;



	int *delta_v = new int[8];
	for(int i = 0;i < 8;++i) {
		delta_v[i] = v - v_LUT[i];
		if(delta_v[i] < 0)
			delta_v[i] = delta_v[i]*(-1); 
	}

	double *delta_p = new double[512];
	for(int i = 0;i < 512;++i) {
		delta_p[i] = p - p_LUT[i];
		if(delta_p[i] < 0)
			delta_p[i] = delta_p[i]*(-1); 
	}

	vec v_copy(8);
	vec p_copy(512);

	for (int i = 0; i < 8; ++i) {
		v_copy(i) = delta_v[i];
	}

	for (int i = 0; i < 512; ++i) {
		p_copy(i) = delta_p[i];
	}


	uvec  ind_v_sorted = sort_index(v_copy);
	vec  delta_v_sorted = sort(v_copy);
	//uvec indices = sort_index(q);
	uvec ind_p_sorted = sort_index(p_copy);
	vec delta_p_sorted = sort(p_copy);

 	//vector<int> ind_v_sorted = sort_indexes(v_LUT, v_LUT + 8);
 	//vector<int> ind_p_sorted = sort(p_LUT, p_LUT + 512);

 	double v1 = v_LUT[ind_v_sorted(0)];
 	double v2 = v_LUT[ind_v_sorted(1)];
 	double p1 = p_LUT[ind_p_sorted(0)];
 	double p2 = p_LUT[ind_p_sorted(1)];

 	double x_11 = x_LUT(ind_p_sorted(0),ind_v_sorted(0));
	double x_12 = x_LUT(ind_p_sorted(0), ind_v_sorted(1));
	double x_21 = x_LUT(ind_p_sorted(1), ind_v_sorted(0));
 	double x_22 = x_LUT(ind_p_sorted(1),ind_v_sorted(1));


 	double f1 = (p2 - p)/(p2-p1);
 	double f2 = (p-p1)/(p2-p1);
 	double x_1 = f1*x_11 + f2*x_21;
 	double x_2 = f1*x_12 + f2*x_22;
 	double x = (v2 - v)/(v2 - v1)*x_1 + (v-v1)/(v2-v1)*x_2;
 	
 	return x;


}

