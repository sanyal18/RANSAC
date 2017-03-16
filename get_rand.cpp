#include <iostream>
#include <vector>
#include <utility>
#include <iterator>
#include <algorithm>
#include "get_rand.h"
#include <armadillo>
using namespace std;
using namespace arma;

bool * get_rand(int k,int N,ll &seed) {
	bool *mask = new bool [N];
	for(int i = 0;i < N; ++i) {
			mask[i] = false;
	}
	//std::srand(2242);
	//srand(seed);
	//seed++;
	for (int i = 0; i < k; ++i) {
		int z = rand() % N;
		while (mask[z] == true) {
			z = rand() % N;
		}
		mask[z] = true;
		//cout << z << " " << endl;
	}
	//random_shuffle( &mask[0], &mask[N]);
	
	return mask;
}