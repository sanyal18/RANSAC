#include <iostream>
#include <vector>
#include <armadillo>
#include "get_q_RANSAC.h"
using namespace std;
using namespace arma;

double get_q_RANSAC(int N, int N_I, int k) {

	/*
	DESC:
	Calculates the probability q

	INPUT:
	N 				= Number of Elements
	N_I 			= Number of inliers
	k 				= Cardinality of the MSS
	
	OUTPUT:
	q 				= probability
	*/

	double q = 0;
	if(N == N_I)
		return 1;

	long long num = 1;
	for(long long i = N_I; i<= N_I-k+1;--i){
		num *= i;
	}

	long long prod = 1;
	for(long long i = N_I; i<= N_I-k+1;--i){
		prod *= i;
	}

	q = (double)num/(double)prod; 
	return q;
}