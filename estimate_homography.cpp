#include "estimate_homography.h"

pair<mat, int>estimate_homography(mat X, std::vector<int> s) {

	/*
	
	DESC:
	Estimate the parameters of an homography using the normalized
	DLT Algorithm.Note that Theta = H(:)

	INPUT:
	X			= 2D point correspondences 4 x N
	s 			= Indices of the points used to estimate the 
				  parameter vector. If empty all points are
				  used

	OUTPUT:
	Theta		= Estimated parameter Vector Theta = H(:)
	K			= Dimension of the minimal Subset			  

	*/

	int k = 4;
//	cout << "In estimate Homography" << endl;
	if(X.is_empty()) {
		mat Theta(0,0);
		return make_pair(Theta,k);
	}
	mat Xnew(X.n_rows, s.size());
	if(s.size() != 0) {
		int k = 0;
		for(int i = 0;i < s.size();i++) {
			for(int j = 0;j < X.n_rows; ++j) {
				Xnew(j,k) = X(j,s[i]);
			//	cerr << k << " ";
			}
			//cerr << endl;
			k++;
		}
		//X = Xnew;
	}
//	cerr << "In estimate Homography1" << endl;
	//cerr << Xnew.n_rows << " " << Xnew.n_cols << endl;
	//mat X1(2,X.n_cols);
	//mat X2(2,X.n_cols);

	//for(int i = 0;i < X.n_cols; ++i) {
	//	X1(0,i)   =  Xnew(0,i);
	//	X1(1,i)   =  Xnew(1,i);
	//	X2(0,i)  =   Xnew(2,i);
	//	X2(1,i)  =   Xnew(3,i);   
	//}
//	cerr << "In estimate Homography2" << endl;

	pair<mat, mat> returnVal = HomographyDLT(Xnew.rows(0,1),Xnew.rows(2,3),true);
	mat H = returnVal.first;
	mat Theta;
//	cerr << "In estimate Homography3" << endl;

    Theta = vectorise(H);
//	cerr << "In estimate Homography4" << endl;

	return make_pair(Theta, k);
}