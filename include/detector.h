#ifndef DETECTOR_H
#define DETECTOR_H

#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/objdetect.hpp>
#include <memory>
#include <sstream>
#include <boost/filesystem.hpp>
#include "invalid_params_exception.h"

using namespace cv;
using namespace std;
namespace fs = boost::filesystem;

class Detector 
{
public:
	Detector(const char* filename);
	Detector(const string& filename);
	Detector(const Mat& img);
	Detector(const Detector& other);

	vector<Rect> detect(Size winStride = Size(8, 8), Size padding = Size(32, 32)) const;

	const Mat getImage() const;
	
	const Mat markOnSource(vector<Rect>& foundLocations) const;

	bool cut(vector<Rect>& foundLocations, vector<Mat>& resultSet) const;

	bool cut(vector<Rect>& foundLocations, fs::path& saveDir) const;

	~Detector();

private:

	void initlize();
	
	vector<Rect> filterLocations(vector<Rect>& foundLocations) const;


	unique_ptr<HOGDescriptor> _peopleDectectHog;
	Mat _image;
};

#endif // !DETECTOR_H
