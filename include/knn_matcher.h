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
	void knnMatch(const Mat& queryDescriptor, Mat& trainDescriptor, vector<vector<DMatch>>& matches, int k);   // K����ƥ��  

	void saveKeypoints(const Mat& image, const vector<KeyPoint>& keypoints, const string& saveFileName = "");  // ����������  
	void saveMatches(const Mat& queryImage,
		const vector<KeyPoint>& queryKeypoints,
		const Mat& trainImage,
		const vector<KeyPoint>& trainKeypoints,
		const vector<DMatch>& matches,
		const string& saveFileName = "");

private:
	shared_ptr<FeatureDetector> _detector;
	shared_ptr<DescriptorExtractor> _extractor;
	shared_ptr<DescriptorMatcher> _matcher;
};

#endif // !KNN_MATCHER_H