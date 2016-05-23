#ifndef KNN_MATCHER_H
#define KNN_MATCHER_H
#include <vector>
#include <string>
#include <memory>
#include <opencv2/core.hpp>
#include <opencv2/xfeatures2d.hpp>
#include <opencv2/features2d.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>

using namespace std;
using namespace cv;

class KnnMatcher
{
public:
	KnnMatcher();
	~KnnMatcher();

	void detectKeypoints(const Mat& image, vector<KeyPoint>& keypoints);
	void extractDescriptors(const Mat& image, vector<KeyPoint>& keypoints, Mat& descriptor);  

	void bestMatch(const Mat& queryDescriptor, Mat& trainDescriptor, vector<DMatch>& matches); 
	void knnMatch(const Mat& queryDescriptor, Mat& trainDescriptor, vector<vector<DMatch>>& matches, int k);

private:
	Ptr<FeatureDetector> _detector;
	Ptr<DescriptorExtractor> _extractor;
	Ptr<DescriptorMatcher> _matcher;
};

#endif // !KNN_MATCHER_H