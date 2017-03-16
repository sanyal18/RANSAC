#pragma once
#include <vector>
#include <utility>
#include <ctime>
#include <armadillo>
#include "estimate_homography.h"
#include "validateTheta_homography.h"
#include "error_threshold.h"
#include "get_mininal_sample_set.h"
#include "get_consensus_set.h"
#include "get_q_RANSAC.h"
#include "get_iter_RANSAC.h"
#include "get_consensus_set_rank.h"
using namespace std;
using namespace arma;
struct results RANSAC1(mat X, struct options option);

struct results
{
	mat Theta;
	mat E;
	bool *CS;
	long long iter;
	double time;
	double r;
};

struct options
{
	double sigma;
	double P_inlier;
	double epsilon;
	double T_noise_squared;
	long long max_iters;
	long long min_iters;
	long long max_no_updates;
	bool mode;

};
