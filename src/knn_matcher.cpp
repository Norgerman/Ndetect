#include "knn_matcher.h"

KnnMatcher::KnnMatcher()
{
	_detector = make_shared<xfeatures2d::SIFT>();
	_extractor = make_shared<xfeatures2d::SIFT>();
	_matcher = make_shared<FlannBasedMatcher>();
}

KnnMatcher::~KnnMatcher()
{

}

void KnnMatcher::detectKeypoints(const Mat& image, std::vector<KeyPoint>& keypoints)
{
	assert(image.type() == CV_8UC1);

	keypoints.clear();
	_detector->detect(image, keypoints);

}



void KnnMatcher::extractDescriptors(const Mat& image, std::vector<KeyPoint>& keypoints, Mat& descriptor)
{
	assert(image.type() == CV_8UC1);

	_extractor->compute(image, keypoints, descriptor);

}


void KnnMatcher::bestMatch(const Mat& queryDescriptor, Mat& trainDescriptor, std::vector<DMatch>& matches)
{
	assert(!queryDescriptor.empty());
	assert(!trainDescriptor.empty());

	matches.clear();

	_matcher->add(vector<Mat>(1, trainDescriptor));
	_matcher->train();
	_matcher->match(queryDescriptor, matches);

}


void KnnMatcher::knnMatch(const Mat& queryDescriptor, Mat& trainDescriptor, std::vector<std::vector<DMatch>>& matches, int k)
{
	assert(k > 0);
	assert(!queryDescriptor.empty());
	assert(!trainDescriptor.empty());

	matches.clear();

	_matcher->add(std::vector<Mat>(1, trainDescriptor));
	_matcher->train();
	_matcher->knnMatch(queryDescriptor, matches, k);

}



void KnnMatcher::saveKeypoints(const Mat& image, const vector<KeyPoint>& keypoints, const string& saveFileName)
{
	assert(!saveFileName.empty());

	Mat outImage;
	cv::drawKeypoints(image, keypoints, outImage, Scalar(255, 255, 0), DrawMatchesFlags::DRAW_RICH_KEYPOINTS);

	//  
	string saveKeypointsImgName = saveFileName + "_flann" + ".jpg";
	imwrite(saveKeypointsImgName, outImage);

}



void KnnMatcher::saveMatches(const Mat& queryImage,
	const vector<KeyPoint>& queryKeypoints,
	const Mat& trainImage,
	const vector<KeyPoint>& trainKeypoints,
	const vector<DMatch>& matches,
	const string& saveFileName)
{
	assert(!saveFileName.empty());

	Mat outImage;
	cv::drawMatches(queryImage, queryKeypoints, trainImage, trainKeypoints, matches, outImage,
		Scalar(255, 0, 0), Scalar(0, 255, 255), vector<char>(), DrawMatchesFlags::NOT_DRAW_SINGLE_POINTS);

	//  
	string saveMatchImgName = saveFileName + "_knn" + ".jpg";
	imwrite(saveMatchImgName, outImage);
}