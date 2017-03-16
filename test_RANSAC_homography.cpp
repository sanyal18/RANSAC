#include "test_RANSAC_homography.h"

int main() { 

	cv::Mat img_object = imread("C:\\Users\\Anurag\\Desktop\\Standard.jpeg", IMREAD_GRAYSCALE);
	cv::Mat img_scene = imread("C:\\Users\\Anurag\\Desktop\\Scan4.jpeg", IMREAD_GRAYSCALE);

	 // to half size or even smaller
																		//namedWindow("Matches", CV_WINDOW_AUTOSIZE);
																		//imshow("Matches", img_matches1);
																		//Syntax-> hconcat(source1,source2,destination);


	//cv::Mat img_object;
//	cv::Mat img_scene;
//	cv::resize(img_object, img_object,Size(800,600));
	//cv::resize(img_scene, dst2, Size(800, 600));

	//imwrite("resized1.jpeg", img_object);
	//imwrite("original1.jpeg", img_object);

	//cv::resize(img_object, img_object, img_object.size(), 0, 0, cv::INTER_LANCZOS4);
	//imshow("resized image", img_object);

	if (!img_object.data || !img_scene.data)
	{
		std::cout << " --(!) Error reading images " << std::endl; return -1;
	}

	//-- Step 1: Detect the keypoints and extract descriptors using SURF
	cout << "Computing Feature in images..... " << endl;
	int minHessian = 8000;
	Ptr<SURF> detector = SURF::create(minHessian);
	std::vector<KeyPoint> keypoints_object, keypoints_scene;
	cv::Mat descriptors_object, descriptors_scene;
	detector->detectAndCompute(img_object, cv::Mat(), keypoints_object, descriptors_object);
	detector->detectAndCompute(img_scene, cv::Mat(), keypoints_scene, descriptors_scene);

	cout << "No of features found" << keypoints_object.size() << endl;
	cout << "_______________________________________________________" << endl;
	cout << "Matching Features....." << endl;
	//-- Step 2: Matching descriptor vectors using FLANN matcher
	FlannBasedMatcher matcher;
	std::vector< DMatch > matches;
	matcher.match(descriptors_object, descriptors_scene, matches);
	double max_dist = 0; double min_dist = 100;
	//-- Quick calculation of max and min distances between keypoints
	for (int i = 0; i < descriptors_object.rows; i++)
	{
		double dist = matches[i].distance;
		if (dist < min_dist) min_dist = dist;
		if (dist > max_dist) max_dist = dist;
	}
	printf("-- Max dist : %f \n", max_dist);
	printf("-- Min dist : %f \n", min_dist);
	//-- Draw only "good" matches (i.e. whose distance is less than 3*min_dist )
	std::vector< DMatch > good_matches;
	for (int i = 0; i < descriptors_object.rows; i++)
	{
		if (matches[i].distance < 3 * min_dist)
		{
			good_matches.push_back(matches[i]);
		}
	}

	cv::Mat img_matches;
	drawMatches(img_object, keypoints_object, img_scene, keypoints_scene,
		good_matches, img_matches, Scalar::all(-1), Scalar::all(-1),
		std::vector<char>(), DrawMatchesFlags::NOT_DRAW_SINGLE_POINTS);

	//cout << " Test matches "<< good_matches[0].queryIdx << endl;
	std::vector< DMatch > final_good_matches;
	for (int i = 0; i < good_matches.size(); i++) {
			cv::Mat img_matches1;
			std::vector< DMatch > good_matches1;
			good_matches1.push_back(good_matches[i]);

			drawMatches(img_object, keypoints_object, img_scene, keypoints_scene,
				good_matches1, img_matches1, Scalar::all(-1), Scalar::all(-1),
				std::vector<char>(), DrawMatchesFlags::NOT_DRAW_SINGLE_POINTS);
			
			std::vector<Point2f> obj;
			std::vector<Point2f> scene;
			obj.push_back(keypoints_object[good_matches[i].queryIdx].pt);
			scene.push_back(keypoints_scene[good_matches[i].trainIdx].pt);
			scene[0].x += img_object.size().width;
									
			line(img_matches1, obj[0] , scene[0], Scalar(0, 255, 0), 4);
		imwrite("Matches1.jpeg", img_matches1);
		int x;
		cout << "Enter now" << endl;
		cin >> x;
		if (x == 1) {
			final_good_matches.push_back(good_matches[i]);
		}
		else if (x == 2) {
			continue;
		}
		else if (x == 3) {
			for (int j = i; j < good_matches.size(); j++) {
				final_good_matches.push_back(good_matches[j]);
			}
			break;
		}
	}




	//-- Localize the object
	std::vector<Point2f> obj;
	std::vector<Point2f> scene;
	for (size_t i = 0; i < final_good_matches.size(); i++)
	{
		//-- Get the keypoints from the good matches
		obj.push_back(keypoints_object[final_good_matches[i].queryIdx].pt);
		scene.push_back(keypoints_scene[final_good_matches[i].trainIdx].pt);
	}
	cout << "So we are going to match " << obj.size() << " features by RANSAC " << endl;

	//-------------------------------------------------------------------
	// Parameters
	//------------------------------------------------------------------- 
	// number of points
	int N = obj.size();
	// inliers percentage
	double p = 0.25;
	// noise
	double sigma = 1;

	mat temp1(2, N);
	mat temp2(2, N);

	for (int i = 0; i < N; ++i) {
		temp1(0, i) = obj[i].x;
		temp1(1, i) = obj[i].y;
	}

	for (int i = 0; i < N; ++i) {
		temp2(0, i) = scene[i].x;
		temp2(1, i) = scene[i].y;
	}
	mat temp;
	temp = join_vert(temp1, temp2);

	options option;
	option.epsilon = pow(10,-6);
	option.P_inlier = 1 - pow(10,-4);
	option.sigma = sigma;
	option.mode = 1;
	option.min_iters = 10000;
	option.max_iters = pow(2,31);
	option.max_no_updates = pow(2,31);
	option.T_noise_squared = 73.563766;

	results result = RANSAC1(temp, option);
	//option.reestimate = true;

	//-----------------------------------------------------------------
	// Data Generation
	//------------------------------------------------------------------
	//cv::Mat H = findHomography(obj, scene, RANSAC);
	mat H1 = reshape(result.Theta, 3, 3);
	cv::Mat H(3,3, CV_64F);
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++) {
			H.at<double>(i, j) = H1(i, j);
		}
	}
	//cerr << "Here brother 4" << endl;
	cv::Mat output;
	warpPerspective(img_scene, output, H, img_scene.size());
	resize(output, output, Size(output.cols/4, output.rows/4)); // to half size or even smaller
	imshow("warped image", output);
	imwrite("result6666.jpeg", output);
	std::vector<Point2f> obj_corners(4);
	obj_corners[0] = cvPoint(0, 0); obj_corners[1] = cvPoint(img_object.cols, 0);
	obj_corners[2] = cvPoint(img_object.cols, img_object.rows); obj_corners[3] = cvPoint(0, img_object.rows);
	std::vector<Point2f> scene_corners(4);
	perspectiveTransform(obj_corners, scene_corners, H);
	//-- Draw lines between the corners (the mapped object in the scene - image_2 )
	line(img_matches, scene_corners[0] + Point2f(img_object.cols, 0), scene_corners[1] + Point2f(img_object.cols, 0), Scalar(0, 255, 0), 4);
	line(img_matches, scene_corners[1] + Point2f(img_object.cols, 0), scene_corners[2] + Point2f(img_object.cols, 0), Scalar(0, 255, 0), 4);
	line(img_matches, scene_corners[2] + Point2f(img_object.cols, 0), scene_corners[3] + Point2f(img_object.cols, 0), Scalar(0, 255, 0), 4);
	line(img_matches, scene_corners[3] + Point2f(img_object.cols, 0), scene_corners[0] + Point2f(img_object.cols, 0), Scalar(0, 255, 0), 4);
	//-- Show detected matches
	resize(img_matches, img_matches, Size(img_matches.cols/4, img_matches.rows/4)); // to half size or even smaller
	namedWindow("Good Matches & Object detection", CV_WINDOW_AUTOSIZE);
	imshow("Good Matches & Object detection", img_matches);

	waitKey(0);
	return 0;

}