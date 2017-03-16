#include <iostream>
#include <vector>
#include <cmath>
#include "homo2cart.h"
#include "cart2homo.h"
#include "mapping_homography.h"
mat mapping_homography(vec xs, vec ys,bool mode,mat Theta,int N) {

/*
	DESC:
	Apply the homography mapping to the source points (xs, ys)

	INPUT:
	xs, ys		= source points
	mode		= true for forward mapping, false for backward mapping
	Theta		= parameters of the mapping function.

	OUTPUT:
	Xd 			= Destination points(row(0) = xd ,row(1) = yd)	
*/


	////////////////////////////////////////////////////////
	// preliminary operations
	///////////////////////////////////////////////////////
	//cerr << "In mapping Homography" << endl;
	//cerr << Theta << endl;
    mat H = Theta;
    H.reshape(3,3);
	mat Xs(2,N);
	//cerr << "In mapping Homography1" << endl;
	for (int i = 0; i < N; ++i)
	{
		Xs(0,i) = xs(i);
		Xs(1,i) = ys(i);
	}
	//cerr << "In mapping Homography2" << endl;

	///////////////////////////////////////////////////////
	// mapping
	///////////////////////////////////////////////////////
	mat Xd(2,N);
	if (mode) {
		// forwrd mapping
		//cerr << "*********************************" << endl;
		//cerr << "before"<<Xs.cols(1,20)<< endl;
		Xd = homo2cart(H*cart2homo(Xs,N,0));
		//cerr << "after" << Xd.cols(1,20) << endl;
		//cerr << "*********************************" << endl;

		//cerr << "In mapping Homography3" << endl;
	}
	else {
		// backward mapping
		H =inv(H);
		Xd = homo2cart(H*cart2homo(Xs,N,0));
	}

	return Xd;

}

