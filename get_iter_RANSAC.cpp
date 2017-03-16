#pragma once
#include <iostream>
#include <vector>
#include <cmath>
#include <utility>
#include <armadillo>
#include "get_iter_RANSAC.h"
using namespace std;
using namespace arma;


long long get_iter_RANSAC(double epsilon, double q) {

	/*
	DESC:
	Estimates the number of iterations for RANSAC
	
	INPUT:
	epsilon				= false alarm rate
	q					= probability	

	OUTPUT:
	T_iter 				= Number of iterations

	*/
	long long T_iter = 0;
	if((1-q) > pow(10,-12)){
		T_iter = (log(epsilon) / log(1-q));
	}
	
	return T_iter; 
}