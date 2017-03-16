#include <iostream>
#include <vector>
#include <utility>
#include <armadillo>
using namespace std;
using namespace arma;

pair<mat, mat> normalize_points(mat x) {

	/*
		
	DESC:
	normalize a set of points using the procedure desribed
	in the book by Hartley and Zisserman

	INPUT:
	x			= points to be normalized

	OUTPUT:
	Xn			= normalized set of points
	T 			= transformation matrix such that Xn = T * X

	*/

	// Compute the translation
	//cerr << "IN normalize points " << endl;
	//cerr << "***********************" << endl;
	mat x_bar;
	x_bar = mean(x, 1);
	//cerr << "mean variable is  " << endl << x_bar << endl;
	//cerr << x.n_rows << endl; 
  //  x_bar = 0.0;
	//for (int i = 0; i < x_bar.n_rows; ++i) {
		//x_bar(i,0) = 0;
	//}
//	for(int i = 0;i < x.n_cols;i++) {
		// Sum along dimension X
	//	x_bar(0,0) += x(0,i);
		// Sum along dimension Y
		//x_bar(1,0) += x(1,i);
	//}
//	x_bar(0) = x_bar(0)/x.n_cols;
	//x_bar(1) = x_bar(1)/x.n_cols;
//	cerr << "IN normalize points 1" << endl;

	// center the points
	mat xc(x.n_rows,x.n_cols);

	//for(int i = 0;i < x.n_rows; ++i) {
		xc.row(0) = x.row(0) - x_bar(0);
		xc.row(1) = x.row(1) - x_bar(1);
	//}
//	cerr << "IN normalize points 2" << endl;

		//cerr << "xc is finally " << endl << xc << endl;
	//
	double rho_bar = 0;
	vec rho_temp(x.n_cols);
	for(int i = 0;i < x.n_cols;++i) {
		rho_temp(i)= sqrt( pow(xc(0,i),2) + pow(xc(1,i),2) );
	}
//	cerr << "rho temp is" << endl << rho_temp << endl;
	rho_bar = mean(rho_temp);
	//cerr << rho_bar << endl;
//	cerr << "*********************************" << endl;
	//for (int i = 0; i < rho_temp.size(); ++i) {
		//rho_bar += rho_temp(i);
	//}
	//rho_bar /= rho_temp.size();

	//compute the scale factor
	double s = sqrt(2)/rho_bar;

	//scale the points
	mat Xn = s*xc;

	//cerr << "IN normalize points 3" << endl;

	//compute the transformation matrix
	mat T(3,3);

	T(0,0) = s;
	T(0,1) = 0;
	T(0,2) = -s*x_bar(0,0);
	T(1,0) = 0;
	T(1,1) = s;
	T(1,2) = -s*x_bar(1,0);
	T(2,0) = 0;
	T(2,1) = 0;
	T(2,2) = 1;
//	cerr << "IN normalize points 4" << endl;

	return make_pair(Xn,T);
}