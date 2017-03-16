#pragma once
#include <iostream>
#include <vector>
#include <utility>
#include <armadillo>
#include "error_homography.h"
#include "get_consensus_set.h"
using namespace std;
using namespace arma;

pair< mat ,bool * > get_consensus_set(mat X, mat Theta_hat,
														double T_noise_squared,int N) {
 /*
	DESCRIPTION:
	select the consensus set for a given parameter vector

 	OUTPUT:
 	A pair of vectors
 	E =  Double Array containing the error values of all the data points
 	CS = Boolean Array for the consensus Set, True for inliers, False for outliers

	INPUT:
	X                 = input data. The data id provided as a matrix that has
                    	dimensions 4 x N ,N is the number of elements. 
	Theta_hat         = estimated homography parameter vector. An array of 
						doubles of size 9 x 1.   
	T_noise_squared   = noise threshold
	N                 = no of data elements
 */

 // calculate the errors over the entire dataset
	//cerr << "In get consensus set " << endl;

 	mat E = error_homography(Theta_hat,X,N);

	// find the points within the error threshold
	bool *CS = new bool[N];
	for(int i = 0; i < N;i++) 
	{
		CS[i] = false;
		if(E(i,0) <= T_noise_squared)
			CS[i] = true;
 	}
	//cerr << "In get consensus set 1" << endl;

 	pair<mat, bool *> returnValue = make_pair(E,CS);
 	return returnValue;

}
