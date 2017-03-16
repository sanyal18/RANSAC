#include <iostream>
#include <vector>
#include <armadillo>
#include "cart2homo.h"
using namespace std;
using namespace arma;

mat cart2homo(mat Q_cart,int N,int lambda) {

/*
	DESC:
	Converts from homogeneous coordinates to cartesian ones
	
	INPUT:
	Q_cart		= cartesian coordinates of the points
	lambda		= scale factor (optional)

	OUTPUT:
	Q_homo		= homogeneous coordinates of the points
*/

//	cerr << "in cart 2 homo" << endl;


	if(lambda == 0)
		lambda = 1;

	int l = Q_cart.n_rows;
	int n = Q_cart.n_cols;

	mat Q_homo(l + 1,n);
	for (int i = 0; i < n; ++i)
	{
		for(int j = 0;j < l;j++) {
			Q_homo(j,i) = (1/lambda) * Q_cart(j,i);
		}
		Q_homo(l,i) = lambda;
	}
//	cerr << "return cart 2 homo" << endl;
	
	return Q_homo;
}

