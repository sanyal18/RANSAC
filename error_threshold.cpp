#include <iostream>
#include <vector>
#include <utility>
#include <armadillo>
#include "error_threshold.h"
#include "chi2inv_LUT.h"
using namespace std;
using namespace arma;

pair<double ,int> error_threshold(double sigma, double P_inlier) {

/*

	DESCRIPTION:
	estimate the error threshold for homography

	INPUT :
	sigma			= noise std
	P_inlier		= Chi squared probability threshold for inliers. If 0 
			  	  then use directly sigma

	OUTPUT :
	T_noise_squared = squared noise threshold
	d 				= degrees of freedom of the error distribution			  

*/
    int d = 0;

	double T_noise_squared;
	if (P_inlier == 0) {
		T_noise_squared = 0;
	}
	else {
		// Assume the errors are normally distributed. Hence the sum of 
		// their squares is Chi distributed (with 4 DOF since the symmetric
		// distance contributes for two terms and the dimensionality is 2)
        d = 4;
		// Compute the inverse probability
		T_noise_squared = pow(sigma,2) * chi2inv_LUT(P_inlier, d); 
	}

	pair<double, int> returnValue = make_pair(T_noise_squared,d); 
	return returnValue;
}