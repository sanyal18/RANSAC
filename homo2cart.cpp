#include <iostream>
#include <vector>
#include <utility>
#include <armadillo>
#include "homo2cart.h"
using namespace std;
using namespace arma;

mat homo2cart(mat Q_homo) {

	/*
	DESC:
	Converts from cartesian coordinates to homogenious ones
	
	INPUT:
	Q_homo 		= homogeneous coordinates of the points
	
	OUTPUT:
	Q_cart 		= cartesian products of the points

	*/
    int N = Q_homo.n_cols;
	int l = Q_homo.n_rows;
    
	int lambda = Q_homo(2,0);

	if(lambda == 0.0)
	{
		//cerr << "We are here" << endl;
		lambda = 1;
	}
	//cerr << "IN homo2 cart" << endl;
	mat Q_cart(l-1,N);

	for(int i = 0;i < N;i++) {
		for(int j = 0;j < l-1;j++) {
			Q_cart(j,i) = Q_homo(j,i)/Q_homo.row(2)(0,i);
		}
	}
	//cerr << "***********************************" << endl;
	//cerr << "lambda is" << lambda << endl;
	//cerr << "testing here" << Q_homo.row(2) << endl;
	//cerr << "before" << Q_homo.cols(1, 10) << endl;
	//cerr << "after" << Q_cart.cols(1, 10) << endl;
	//cerr << "***********************************" << endl;

	return Q_cart;
}
