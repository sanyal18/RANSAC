#include "validateTheta_homography.h"
bool validateTheta_homography(mat X,mat Theta,vector<int>s) {

	/*

	DESC:
	Validates the parameter vector

	INPUT:
	X			= Input data
	Theta 		= Estimated parameter vector
	s			= Indices of the MSS

	OUTPUT:
	flag		= true if the Theta Parameter is valid

	*/

	//Condition number threshold;
	double T_K = pow(10,12);
	// Perform here to check on the parameter vector Theta
    mat H; H = reshape(Theta,3,3);
	double K = cond(H);

	if (K > T_K) {
	//	cerr << "Validate homography false returned " << endl;
		return false;
	}
//	cerr << "Validate homography  true returned this time" << endl;
	return true;
}