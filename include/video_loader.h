#ifndef VIDEO_LOADER_H
#define VIDEO_LOADER_H

#define INC 10

#include <memory>
#include <opencv2/videoio.hpp>
#include <make_unique.hpp>

using namespace std;
using namespace cv;

class VideoLoader
{
public:
	VideoLoader(const string& filename);
	VideoLoader(const char* filename);

	bool next(Mat& img);
	bool next(Mat& img, double& framePosition);
	bool next(Mat& img, double& framePosition, double& millisecondPosition);
private:
	unique_ptr<VideoCapture> _videoCapture;
	double _framePosition;
	double _totalFrames;
	const double INCREASEMENT;

	void initlization();
};

#endif // !VIDEO_LOADER_H