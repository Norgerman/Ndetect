#include <video_loader.h>

VideoLoader::VideoLoader(const string& filename): INCREASEMENT(INC)
{
	_videoCapture = ::make_unique<VideoCapture>(filename);
	initlization();
}

VideoLoader::VideoLoader(const char* filename): INCREASEMENT(INC)
{
	_videoCapture = ::make_unique<VideoCapture>(filename);
	initlization();
}

void VideoLoader::initlization()
{
	_framePosition = 0;
	_totalFrames = _videoCapture->get(CAP_PROP_FRAME_COUNT);
}

bool VideoLoader::next(Mat& img, double& framePosition, double& millisecondPosition)
{
	if (_framePosition < _totalFrames)
	{
		_videoCapture->set(CAP_PROP_POS_FRAMES, _framePosition);
		*_videoCapture >> img;
		framePosition = _framePosition;
		millisecondPosition = _videoCapture->get(CAP_PROP_POS_MSEC);
		_framePosition += INCREASEMENT;
	}
	else
	{
		return false;
	}
}

bool VideoLoader::next(Mat& img)
{
	double framePosition = 0;
	double millisecondPosition = 0;
	return next(img, framePosition, millisecondPosition);
}

bool VideoLoader::next(Mat& img, double& framePosition)
{
	double millisecondPosition = 0;
	return next(img, framePosition, millisecondPosition);
}