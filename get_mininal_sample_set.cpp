
#include <iostream>
#include <vector>
#include <utility>
#include "get_mininal_sample_set.h"
#include "get_rand.h"
#include "validateMSS_homography.h"
#include "estimate_homography.h"
using namespace std;

pair< vector<int>, mat> get_minimal_sample_set(int k, mat X,vector<int> ind_tabu,ll &seed)  {
	/* 
	DESC:
	select the minimal sample set using different sampling strategies
	
	% INPUT:
	% k                 = minimal sample set cardinality
	% X                 = input data
	% P_s               = sampling probabilities
	
	%
	% ind_tabu          = indices of elements excluded from the sample set
	%
	% OUTPUT:
	% s                 = minimal sample set
	% Theta_hat         = estimated parameter vector on s
	*/
	//cerr << "In minimal sample set" << endl;
	int N = X.n_cols;

	bool *ind = new bool [N];
	for(int i = 0;i < N;i++) {
		ind[i] = true;
	}
	int NN = N;

	if(ind_tabu.size() != 0) {
		for(int i = 0;i < ind_tabu.size(); ++i) {
			ind[ind_tabu[i]] = false;
			NN--;
		}
	}

	while(true) {
		bool *mask = get_rand(k, N,seed);
		std::vector<int> s;
		//cout << "-----------------" << endl;
		for(int i = 0;i < N;i++) {
			if (mask[i] == true) {
				s.push_back(i);
				//cout << i << endl;
			}
		}
		//cout << "-----------------" << endl;

		if(!validateMSS_homography(X,s))
			continue;
		pair<mat, int> val = estimate_homography(X,s);
		mat Theta_hat = val.first;

		if(Theta_hat.n_rows != 0 && Theta_hat.n_cols != 0) {
			//cerr << "return minimal sample set 1" << endl;

			return make_pair(s,Theta_hat);
		}

	}

	vector<int> empty;
	mat empty1;
	//cerr << "return minimal sample set empty" << endl;

	return make_pair(empty,empty1);

}