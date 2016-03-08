#ifndef DETECTOR_H
#define DETECTOR_H

#include "stdafx.h"

using namespace cv;
using namespace std;

class Detector 
{
public:
	Detector(const char* filename);
	Detector(const string& filename);
	Detector(const Mat& img) throw(InvalidParamsException);
	Detector(const Detector& other);

	vector<Rect> detect(Size winStride = Size(8, 8), Size padding = Size(32, 32)) const;

	const shared_ptr<Mat> getImage() const;
	
	const shared_ptr<Mat> markOnSource(vector<Rect>& foundLocations) const;

	~Detector();

private:

	void initlize();
	
	vector<Rect> filterLocations(vector<Rect>& foundLocations) const;


	unique_ptr<HOGDescriptor> _peopleDectectHog;
	shared_ptr<Mat> _image;
};

#endif // !DETECTOR_H
