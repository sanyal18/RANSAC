#include <iostream>
#include"RANSAC.h"
long long RANSAC_SEED;
long long RANSAC_SEED_UPDATED;
struct results RANSAC1(mat X, struct options option) {

	cerr << "Starting RANSAC" << endl;

	//Start timer
	clock_t t;
	t = clock();

	// get minimal subset size and model codimension
	int k = 4;

	cout << "Minimal sample set dimension = "<< k << endl;
	int N = X.n_cols;

	if(N < k) {
		cout << "RANSACToolbox : dataSetTooSmall "<<endl;
		results empty;
		return empty;
	}
	double T_noise_squared; double d = 0;
	// calculate the probability for the inlier detection
	if(option.T_noise_squared == 0) {
		// get the noise threshold via Chi Squared distribution;
		pair<double, int> threshold_pair = error_threshold(option.sigma,
							 option.P_inlier);

		T_noise_squared = threshold_pair.first;
		d = threshold_pair.second;
	}
	else {
		T_noise_squared = option.T_noise_squared;
	}

	cout << "\n Squared noise Threshold is "<<T_noise_squared << endl;

	//------------------------------------------------------------------------
	// Randomization
	//------------------------------------------------------------------------
	long long seed = time(NULL);
	//std::srand(seed);
	arma_rng::set_seed(seed);

	RANSAC_SEED = seed;
	RANSAC_SEED_UPDATED = seed;

	//------------------------------------------------------------------------
	// Main Loop
	//------------------------------------------------------------------------
	int N_I_star = 0;
	double r_star = -pow(2,31);
	
	bool *CS_star = new bool[N];
	for (int i = 0; i < N; ++i) {
		CS_star[i] = false;
	}
	std::vector<int> ind_CS_star;
	mat Theta_star(9,1);
	mat E_star(1,N);

	long long iter = 0;
	long long T_iter = option.max_iters;
	cerr << "T iter as initalised is  " << T_iter << endl;
	long long no_updates = 0;

	while( (iter <= option.min_iters) ||
		  ( ( iter <= T_iter) && (iter <= option.max_iters) &&
		  	  (no_updates <= option.max_no_updates) ) ) {
		
		iter++;
		bool notify = false;
		bool update_sets = false;
		bool update_T_iter = false;
		
		// Hpothesize ---------------------------------------------------------

		// select MSS
		std::vector<int> ind_tabu;
		//cerr << "RANSAC loop starts here" << endl;
		pair< vector<int>, mat> minimal_set_pair = get_minimal_sample_set(k,X, ind_tabu,RANSAC_SEED);
		//cerr << "IN RANSAC 1" << endl;
		std::vector<int> MSS = minimal_set_pair.first;
        mat Theta(9,1); Theta = minimal_set_pair.second;

		// validate the parameter vector Theta
		if(!validateTheta_homography(X, Theta, MSS))
			continue;
		//cerr << "IN RANSAC 2" << endl;

		// Test ---------------------------------------------------------------
		pair<mat, bool*> consensus_pair = get_consensus_set(X, Theta, T_noise_squared,N);
        mat E(1,N); E = consensus_pair.first;
		bool *CS = consensus_pair.second;
		//cerr << "IN RANSAC 3" << endl;

		// get the ranking of the CS
		double r = get_consensus_set_rank(CS, E, option.mode, T_noise_squared);

		std::vector<int> ind_CS;
		ind_CS.clear();
		//cerr << "IN RANSAC 4" << endl;

		// get the indices
		for(int i = 0;i < N;i++) {
			if(CS[i])
				ind_CS.push_back(i);
		}
		
		//get the estimated no of inliers
		int N_I = ind_CS.size();
		r = -(double)(N - N_I)*T_noise_squared / (double)N;
		//cerr << "No of inliers " << N_I << endl;
		// Update---------------------------------------------------------------

		// if we found a larger inlier set update both the inlier set and the 
		// number of iterations

		if( (N_I >= N_I_star)  && (r > r_star) ) {
			notify = true;
			r_star = r;

			update_sets = true;
			if(N_I > N_I_star)
				update_T_iter = true;
		}
		//cerr << "IN RANSAC 5" << endl;

		// update the sets

		if(update_sets) {
		//	cerr << "updating here" << endl;
			Theta_star.clear();
			E_star.clear();
			ind_CS_star.clear();
			Theta_star 		= Theta;
			CS_star 		= CS;
			E_star			= E;
			N_I_star		= N_I;
			ind_CS_star		= ind_CS;
			

			no_updates = 0;
		}
		else
			no_updates++;
	//	cerr << "size of ind_CS_star is " << ind_CS_star.size() << endl;
		// update the number of iterations
		//cerr << "IN RANSAC 6" << endl;

		if(update_T_iter) {
			double q = get_q_RANSAC(N, N_I, k);
			double eps = pow(2,-52);
			if (q > eps) {
				T_iter = get_iter_RANSAC(option.epsilon, q);
				//cerr << "------------------------------------" << endl;
				//cerr << "T iter updated is" << T_iter << endl;
				//cerr << "------------------------------------" << endl;

			}
		}
	//	cerr << "IN RANSAC 7" << endl;

		if(update_T_iter == true || update_sets == true || notify == true) {
			cout << "Iteration = "<<iter<<"/" << T_iter<<endl;
			cout << "Inliers = "<<N_I_star<<"/" << N << " rank is "<<r_star<<endl; 
		}
	//	cerr << "IN RANSAC 8" << endl;
	//	cerr << iter << endl;
		if (ind_CS_star.size() == N) {
			cerr << "break it off " << endl;
			break;
		}
	}
	cerr << " iter" << iter << "   T_iter is " << T_iter << endl;
//	cerr << "IN RANSAC 9" << endl;

	struct results res;
	std::vector<int> inlier;
	for(int i = 0;i < N; ++i) {
		if(CS_star[i]) {
			inlier.push_back(i);
		}
	}
	//cerr << "Here brother pucker" << endl;
	Theta_star = estimate_homography(X, inlier).first;
	//cerr << "Here brother pucker2" << endl;

	pair<mat, bool*> consensus_pair = get_consensus_set(X, Theta_star, T_noise_squared,N);
    E_star = consensus_pair.first;
	CS_star = consensus_pair.second;
	//cerr << "Here brother pucker3" << endl;

		// get the ranking of the CS
	r_star = get_consensus_set_rank(CS_star, E_star, option.mode, T_noise_squared);
	cerr << "Theta Star is " << Theta_star << endl;
	res.Theta = Theta_star;
	res.E = E_star;
	res.CS = CS_star;
	cout << "\nFinal Number of inliers "<<inlier.size()<<endl;
	res.time = clock()-t;
	cout << "\n Converged in "<<iter << " iterations ("<<res.time<<") secs"<<endl;

	return res;
}