#include "Source.h"
/* @function main */
int main(int argc, char** argv)
{

	cv::Mat img_1 = imread("C:\\Users\\Anurag\\Desktop\\Standard.jpeg", IMREAD_GRAYSCALE);
	cv::Mat img_2 = imread("C:\\Users\\Anurag\\Desktop\\Scan1.jpeg", IMREAD_GRAYSCALE);
	if (!img_1.data || !img_2.data)
	{
		std::cout << " --(!) Error reading images " << std::endl; return -1;
	}
	//-- Step 1: Detect the keypoints using SIFT Detector
	//int num = 1000;
	//int minHessian = 50;
	cout << "Herre" << endl;
	std::vector<KeyPoint> features_object, features_scene;

	//	FAST(img_1, features_object, 25);
	//FAST(img_2, features_scene, 25);


	int nfeatures = 1000;
	float scaleFactor = 1.2f;
	int nlevels = 8;
	int edgeThreshold = 31; // Changed default (31);
	int firstLevel = 0;
	int WTA_K = 2;
	int scoreType = ORB::HARRIS_SCORE;
	int patchSize = 31;
	int fastThreshold = 10;


	Ptr<ORB> detector = ORB::create(
		nfeatures,
		scaleFactor,
		nlevels,
		edgeThreshold,
		firstLevel,
		WTA_K,
		scoreType,
		patchSize,
		fastThreshold);

	detector->detect(img_1, features_object);
	detector->detect(img_2, features_scene);

	cout << "-----------------------------" << endl;
	cout << features_object.size() << endl;
	cout << "-----------------------------" << endl;

	cout << features_object.size() << endl;
	cv::Mat descriptors_object, descriptors_object1, descriptors_scene1, descriptors_scene;
	Ptr<SIFT> detector2 = SIFT::create(100);

	detector2->compute(img_1, features_object, descriptors_object);
	detector2->compute(img_2, features_scene, descriptors_scene);

	cout << "SIFT done" << endl;
	Ptr<SURF> detector1 = SURF::create(6000);

	detector1->compute(img_1, features_object, descriptors_object1);
	detector1->compute(img_2, features_scene, descriptors_scene1);



	FlannBasedMatcher matcher;
	std::vector< DMatch > matches, matches1;
	matcher.match(descriptors_object, descriptors_scene, matches);
	matcher.match(descriptors_object1, descriptors_scene1, matches1);

	cout << matches1.size() << endl;
	cout << matches.size() << endl;

	std::vector<Point2f> obj_sift;
	std::vector<Point2f> scene_sift;

	std::vector< DMatch > good_matches, good_matches1;
	for (int i = 0; i < descriptors_object.rows; i++)
	{
		good_matches.push_back(matches[i]);
	}
	for (int i = 0; i < descriptors_object1.rows; i++)
	{
		good_matches1.push_back(matches1[i]);
	}

	for (size_t i = 0; i < matches.size(); i++)
	{
		//-- Get the keypoints from the good matches
		obj_sift.push_back(features_object[good_matches[i].queryIdx].pt);
		scene_sift.push_back(features_scene[good_matches[i].trainIdx].pt);
	}

	std::vector<Point2f> obj_surf;
	std::vector<Point2f> scene_surf;



	for (size_t i = 0; i < good_matches.size(); i++)
	{
		//-- Get the keypoints from the good matches
		obj_sift.push_back(features_object[good_matches[i].queryIdx].pt);
		scene_sift.push_back(features_scene[good_matches[i].trainIdx].pt);
	}

	for (size_t i = 0; i < good_matches1.size(); i++)
	{
		//-- Get the keypoints from the good matches
		obj_surf.push_back(features_object[good_matches1[i].queryIdx].pt);
		scene_surf.push_back(features_scene[good_matches1[i].trainIdx].pt);
	}

	std::vector<Point2f> final_obj;
	std::vector<Point2f> final_scene;

	std::vector< DMatch > common_good_matches;

	for (int i = 0; i < obj_surf.size(); i++) {
		if (scene_surf[i].x == scene_sift[i].x && scene_surf[i].y == scene_sift[i].y) {
			//	final_obj.push_back(obj_surf[i]);
			//	final_scene.push_back(scene_surf[i]);

			common_good_matches.push_back(good_matches[i]);
		}
	}

	cout << "common good matches size  " << common_good_matches.size() << endl;


	double max_dist = 0; double min_dist = 100;
	//-- Quick calculation of max and min distances between keypoints
	for (int i = 0; i < common_good_matches.size(); i++)
	{
		double dist = common_good_matches[i].distance;
		if (dist < min_dist) min_dist = dist;
		if (dist > max_dist) max_dist = dist;
	}
	printf("-- Max dist : %f \n", max_dist);
	printf("-- Min dist : %f \n", min_dist);
	//-- Draw only "good" matches (i.e. whose distance is less than 3*min_dist )
	std::vector< DMatch > final_matches;
	for (int i = 0; i < common_good_matches.size(); i++)
	{
		if (common_good_matches[i].distance < 3 * min_dist)
		{
			final_matches.push_back(common_good_matches[i]);
		}
	}

	cout << "final good matches size" << final_matches.size() << endl;
	cv::Mat img_matches1;

	std::vector< DMatch > final_good_matches;
	for (int i = 0; i < final_matches.size(); i++) {
		std::vector< DMatch > good_matches1;
		good_matches1.push_back(final_matches[i]);
		good_matches1.push_back(final_matches[i + 1]);
		good_matches1.push_back(final_matches[i + 2]);
		good_matches1.push_back(final_matches[i + 3]);
		good_matches1.push_back(final_matches[i + 4]);
		good_matches1.push_back(final_matches[i + 5]);

		drawMatches(img_1, features_object, img_2, features_scene,
			good_matches1, img_matches1, Scalar::all(-1), Scalar::all(-1),
			std::vector<char>(), DrawMatchesFlags::NOT_DRAW_SINGLE_POINTS);

		std::vector<Point2f> obj;
		std::vector<Point2f> scene;
		obj.push_back(features_object[final_matches[i].queryIdx].pt);
		scene.push_back(features_scene[final_matches[i].trainIdx].pt);
		scene[0].x += img_1.size().width;

		line(img_matches1, obj[0], scene[0], Scalar(0, 255, 0), 4);
		imwrite("Matches1.jpeg", img_matches1);
		int x;
		cout << "Enter now" << endl;
		cin >> x;
		if (x == 1) {
			final_good_matches.push_back(final_matches[i]);
		}
		else if (x == 2) {
			continue;
		}
		else if (x == 3) {
			for (int j = i; j < final_matches.size(); j++) {
				final_good_matches.push_back(final_matches[j]);
			}
			break;
		}
	}


	cout <<"After your sorting it is of size" <<final_good_matches.size() << endl;


	for (size_t i = 0; i < final_good_matches.size(); i++)
	{
		//-- Get the keypoints from the good matches
		final_obj.push_back(features_object[final_matches[i].queryIdx].pt);
		final_scene.push_back(features_scene[final_matches[i].trainIdx].pt);
	}



	cout << final_scene.size() << endl;


	int N = final_scene.size();
	// inliers percentage
	double p = 0.25;
	// noise
	double sigma = 1;

	mat temp1(2, N);
	mat temp2(2, N);

	for (int i = 0; i < N; ++i) {
		temp1(0, i) = final_obj[i].x;
		temp1(1, i) = final_obj[i].y;
	}

	for (int i = 0; i < N; ++i) {
		temp2(0, i) = final_scene[i].x;
		temp2(1, i) = final_scene[i].y;
	}
	mat temp;
	temp = join_vert(temp1, temp2);

	options option;
	option.epsilon = pow(10, -6);
	option.P_inlier = 1 - pow(10, -4);
	option.sigma = sigma;
	option.mode = 1;
	option.min_iters = 10000;
	option.max_iters = pow(2, 31);
	option.max_no_updates = pow(2, 31);
	option.T_noise_squared = 73.563766;

	results result = RANSAC1(temp, option);
	//option.reestimate = true;

	//-----------------------------------------------------------------
	// Data Generation
	//---------final---------------------------------------------------------
	cv::Mat H1 = findHomography(final_scene, final_obj,noArray(),CV_RANSAC,1000.0);

	std::vector<Point2f> objAffine;
	std::vector<Point2f> sceneAffine;

	for (int i = 5; i < 8 ; i++) {
		objAffine.push_back(final_obj[i]);
		sceneAffine.push_back(final_scene[i]);
	}

	cv::Mat warp_mat(2, 3, CV_32FC1);
	warp_mat = getAffineTransform(objAffine, sceneAffine);

	cv::Mat warp_dst;

	warp_dst = cv ::Mat::zeros(img_1.rows, img_1.cols, img_1.type());
	
	warpAffine(img_2, warp_dst, warp_mat, warp_dst.size());
	cout << "Affine transformation done " << endl;

	resize(img_matches1, img_matches1, Size(img_matches1.cols / 4, img_matches1.rows / 4)); // to half size or even smaller
	imshow("imgae matches image", img_matches1);
	imwrite("result_matches.jpeg", img_matches1);


	//resize(warp_dst, warp_dst, Size(warp_dst.cols / 4, warp_dst.rows / 4)); // to half size or even smaller
	//imshow("Affine image", warp_dst);
	//imwrite("result_Affine.jpeg", warp_dst);
	//cv::Mat aff = getAffineTransform(final_obj, final_scene);
	mat H = reshape(result.Theta, 3, 3);
	cv::Mat H(3, 3, CV_64F);
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++) {
			H.at<double>(i, j) = H1(i, j);
		}
	}
	//cerr << "Here brother 4" << endl;
	cv::Mat output,output_Affine;
	warpPerspective(img_2, output, H1, img_2.size());
	warpPerspective(img_2, output, H, img_2.size());

	//warpAffine(img_2, output_Affine, aff,img_2.size());
	resize(output, output, Size(output.cols / 4, output.rows / 4)); // to half size or even smaller
	imshow("warped image", output);
	imwrite("result_common.jpeg", output);

	//resize(output_Affine, output_Affine, Size(output_Affine.cols / 4, output_Affine.rows / 4)); // to half size or even smaller
	//imshow("affine image", output_Affine);
	//imwrite("result_affine.jpeg", output_Affine);

	//Mat H = findHomography(final_obj, final_scene, RANSAC);
	waitKey(0);
}
/* @function readme */
