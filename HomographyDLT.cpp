#include <iostream>
#include <vector>
#include <utility>
#include <armadillo>
#include "homographyDLT.h"
#include "normalize_points.h"
using namespace std;
using namespace arma;


mat get_A_MZ(mat X1, mat X2) {
    int N = X1.n_cols;
	//cerr << "--------------------" << endl;
	//cerr << "In get_A_MZ" << endl;
	//cout << X1<< endl << X2 << endl;
    mat A(2*N,9);
	for (int i = 0; i < A.n_rows; ++i) {
		for (int j = 0; j < A.n_cols; ++j) {
			A(i, j) = 0;
		}
	}
    int row = 0;
    
    for(int h = 0;h< N;++h) {
        A(row,0) = X1(0,h);  A(row,1) = 0;
        A(row,2) = -X1(0,h)*X2(0,h);  A(row,3) = X1(1,h);
        A(row,4) = 0;  A(row,5) = -X1(1,h)*X2(0,h);
        A(row,6) = 1; A(row,7) = 0; A(row,8) = -X2(0,h);
        
        A(row + 1,0) = 0; A(row + 1,1) = X1(0,h);
        A(row + 1,2) = -X1(0, h)*X2(1, h); A(row + 1, 3) = 0;
        A(row + 1,4) = X1(1, h); A(row + 1, 5) = -X1(1, h)*X2(1,h);
        A(row + 1,6) = 0; A(row +1,7) = 1; A(row + 1,8) = -X2(1,h);
        
        row = row + 2;
    }
//	cout << "A is  " << endl << A << endl;;
	//cerr << "---------------------"<< endl;

    return A;
}



pair<mat, mat> HomographyDLT(mat X1, mat X2,bool normalization) {

/*
	DESC:
	Computes the homography between the points pairs X1, X2

	INPUT:
	X1, X2			= points matches (cartesian coordinates)
	mode			= 'HZ'-> Hartley Zisserman formulation
					  'MZ' -> Zulian formulation (default)
	normalization	= true or false to enable/disable point
					  normalization				  
	
	OUTPUT:
	H 				= homography
	A				= homogeneous linear system matrix

*/
	int N = X1.n_cols;
    mat T2,T1;
//	cerr << "In homography DLT" << endl;

	//////////////////////////////////////////////////////////////////////
	// normalize the input
	//////////////////////////////////////////////////////////////////////
	if (normalization){
	//	cerr << "before" << endl;
		//cerr << X1 << endl;
		pair<mat, mat> normal_mat1 = normalize_points(X1);
		X1 = normal_mat1.first; T1 = normal_mat1.second;
		//cerr << "after" << endl;
		//cerr << X1 << endl;
		pair<mat, mat> normal_mat2 = normalize_points(X2);
        X2 = normal_mat2.first; T2 = normal_mat2.second;
	}
//	cerr << "In homography DLT 1" << endl;

	mat A = get_A_MZ(X1, X2);

	mat U;
	vec s;
	mat V;
	svd(U,s,V,A);
//	cerr << "In homography DLT 2" << endl;

	mat h(1,9);
	h = V.col(8);

//	for(int i = 0;i < 9;i++) {
	//	h(0,i) = V(i,8);
	//}
	mat H;;
	H = reshape(h,3,3);

	///////////////////////////////////////////////////////////////////////
	// de-normalize the parameters
	//////////////////////////////////////////////////////////////////////
//	cerr << "In homography DLT 3" << endl;

	if (normalization) {
		T2 = inv(T2);
		H = T2*H*T1;
	}

	///////////////////////////////////////////////////////////////////////
	// re-normalize the parameters
	//////////////////////////////////////////////////////////////////////	
	
	H = H/norm(vectorise(H));
//	cerr << "In homography DLT 4" << endl;

	return make_pair(H,A);

}

// Zuliani's formulation
