#include "knn_matcher.h"

KnnMatcher::KnnMatcher()
{
	_detector = ORB::create();
	_extractor = ORB::create();
	_matcher = DescriptorMatcher::create("BruteForce");
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

	_matcher->match(queryDescriptor, trainDescriptor, matches);

}


void KnnMatcher::knnMatch(const Mat& queryDescriptor, Mat& trainDescriptor, std::vector<std::vector<DMatch>>& matches, int k)
{
	assert(k > 0);
	assert(!queryDescriptor.empty());
	assert(!trainDescriptor.empty());

	matches.clear();

	_matcher->knnMatch(queryDescriptor, trainDescriptor, matches, k);
}