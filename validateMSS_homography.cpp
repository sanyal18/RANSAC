
#include "validateMSS_homography.h"
bool sign(double x) {
	if (x > 0)
		return true;
	return false;
}

bool validateMSS_homography(mat X, vector<int> s) {

//	cout << "IN validate MSS " << endl;
	//cout << X.n_rows << endl;
	//cout << "s vector is " << endl;

	//for (int i = 0; i < s.size(); ++i) {
	//	cout << s[i] << " ";
	//}
	//cout << "---------------------------------" << endl;
    int ind[] = {0, 1};

	int mu[2];
	
	for(int h = 0;h < 2; h++) {	
		mu[0] = 0;
		mu[1] = 0;	
		//cout<< h <<" ***************"<<endl;
		for(int i = 0;i < s.size();i++) {
			//cout << ind[0] << endl;
			mu[0] += X(ind[0],s[i]);
			mu[1] += X(ind[1],s[i]);
			//cout << X(ind[0],s[i]) << " "<< X(ind[1],s[i]) <<endl;
		}
		mu[0] = mu[0]/s.size();
		mu[1] = mu[1]/s.size();

		mat Xzm(2, s.size());
		for(int i = 0;i < s.size(); ++i) {
			Xzm(0,i) = X(ind[0],s[i]) - mu[0];
			Xzm(1,i) = X(ind[1],s[i]) - mu[1];  
		}
	//	cout << "IN validate MSS1 " << endl;

        mat t1,t2;
        t1 = Xzm.row(0); t2 = Xzm.row(1);
		double C1 = as_scalar(t1*trans(t1));
		double C2 = as_scalar(Xzm.row(0)*trans(Xzm.row(1)));
		double C3 = as_scalar(Xzm.row(1)*trans(Xzm.row(1)));
	//	cout << "IN validate MSS 2 " << endl;

		double alpha = C1 + C3;
		double beta = (C1 - C3)*(C1 - C3);
		double gamma = 4*C2*C2;
		double delta = sqrt(beta + gamma);
		double kappa = (alpha + delta)/(alpha - delta);

		if(kappa > pow(10,9)) {
		//	cout << "return validate MSS false " << endl;
			return false;
		}

		ind[0] = ind[0] + 2;
		ind[1] = ind[1] + 2;
	}

	mat newInd(3,3);
	//cout << "IN validate MSS 3" << endl;

	newInd(0,0) = s[1] ;newInd(0,1) = s[2] ;newInd(0,2) = s[3];
	newInd(1,0) = s[2] ;newInd(1,1) = s[1] ;newInd(1,2) = s[1];
	newInd(2,0) = s[3] ;newInd(2,1) = s[3] ;newInd(2,2) = s[2];
//	cout << "IN validate MSS 4" << endl;

	bool flag = false;

	for(int l = 0;l < 3; ++l) {
		double n1 = X(1,newInd(0,l)) - X(1,s[0]);
		double n2 = X(0,s[0]) - X(0,newInd(0,l));

		double p1 = n1*( X(0,newInd(1,l))) - X(0,s[0]) +
					n2*( X(1,newInd(1,l)) ) - X(1,s[0]);

		double p2 = n1*( X(0,newInd(2,l))) - X(0,s[0]) +
					n2*( X(2,newInd(2,l)) ) - X(1,s[0]);

		if( sign(p1) == sign(p2))
			continue;
		//cout << "IN validate MSS 5 " << endl;

		double np1 = X(3,newInd(0,l)) - X(3,s[0]);
    	double np2 = X(2,s[0]) - X(2,newInd(0,l));
		//cout << "IN validate MSS 6 " << endl;

    	// get the projection of the other two corresponding points
    	// 6 summations, 4 multiplications
    	double pp1 = np1*( X(2,newInd(1,l)) - X(2,s[0])) +
       				 np2*( X(3,newInd(1,l)) - X(3,s[0]) );
		//cout << "IN validate MSS 7 " << endl;

   		double pp2 = np1*(  X( 2,newInd(2,l) ) - X( 2,s[0] ) )  +
        				np2*( X(3,newInd(2,l)) - X(3,s[0]) );
	//	cout << "IN validate MSS 8 " << endl;

        flag = (sign(p1) == sign(pp1)) && (sign(p2) == sign(pp2));					
					
	}
	//cout << "return validate MSS "<<flag << endl;

	return flag;

} 