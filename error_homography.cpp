#pragma once
#include <iostream>
#include <vector>
#include <armadillo>
#include "error_homography.h"
#include "mapping_homography.h"

using namespace std;
using namespace arma;

mat error_homography(mat Theta, mat X, int N) {

/*
	DESCRIPTION:
	Estimate the squared symmetric transfer error due to the 
	homographic constraint.		

	INPUT:
	Theta	= Estimated homography parameter vector.Double 
		  	  Array of size 9 x 1.
	X 		= Input data. The data id provided as a matrix that 
			  has dimensions 4 x N ,N is the number of elements. 
	N       = Number of elements.

	OUTPUT:
	E 		= Squared Symmetric transfer error.		  	  			  
*/

 //compute the squared symmetric reprojection error
	mat E1(N,1);
	mat E2(N, 1);
	//mat E(N,1);
	mat X12(2,N);
//	cerr << "In error homography" << endl;

	vec source_x(N);
	vec source_y(N); 

	for (int i = 0; i < N; i++)
	{
		source_x(i) = X(0, i);
		source_y(i) = X(1, i);
	}

    X12 = mapping_homography(source_x, source_y, true, Theta, N);
	mat X21(2,N);
//	cerr << "In error homography#" << endl;
	for(int i = 0;i < N;i++) 
	{
		source_x(i) = X(2,i);
		source_y(i) = X(3,i);
	}
//	cerr << "In error homography1" << endl;
	X21 = mapping_homography(source_x, source_y, false, Theta, N);
//	cerr << "In error homography2" << endl;
	for(int i = 0;i < N;i++) 
	{
		E1(i,0) = pow(X(0,i) - X21(0,i), 2) + pow(X(1,i) - X21(1,i),
				 	2);
	} 
	//cerr << "In error homography3" << endl;
	for(int i = 0; i < N; ++i)
	{
		E2(i,0) =  pow(X(2,i) - X12(0,i), 2) + pow(X(3,i) - 
						   X12(1,i), 2);
	}
	mat E;
	E = E1 + E2;
	//cerr << "*********************************" << endl;
//	cerr << E.t() << endl;
	//cerr << "*********************************" << endl;
//	cerr << "In error homography4" << endl;
	return E;
}