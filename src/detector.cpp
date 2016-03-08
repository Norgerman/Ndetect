#include "detector.h"

Detector::Detector(const char* filename)
	:Detector(imread(filename))
{
}

Detector::Detector(const string& filename)
	:Detector(imread(filename))
{
}

Detector::Detector(const Mat& img) throw(InvalidParamsException)
{
	if (!img.data)
	{
		throw InvalidParamsException("Invalid image");
	}
	_image = make_shared<Mat>(img);
	initlize();
}

Detector::Detector(const Detector& other)
{
	_image = other._image;
	initlize();
}

vector<Rect> Detector::detect(Size winStride, Size padding) const
{
	vector<Rect> foundLocations;

	_peopleDectectHog->detectMultiScale(*_image, foundLocations, 0, winStride, padding, 1.05, 2);

	return filterLocations(foundLocations);
}

Detector::~Detector()
{

}

const shared_ptr<Mat> Detector::getImage() const
{
	return _image;
}

const shared_ptr<Mat> Detector::markOnSource(vector<Rect>& foundLocations) const
{
	shared_ptr<Mat> image = make_shared<Mat>(_image->clone());
	
	for (auto& location : foundLocations)
	{
		rectangle(*image, location.tl(), location.br(), Scalar(0, 0, 255), 2);
	}

	return image;
}

void Detector::initlize()
{
	_peopleDectectHog = ::make_unique<HOGDescriptor>();
	_peopleDectectHog->setSVMDetector(HOGDescriptor::getDefaultPeopleDetector());
}

vector<Rect> Detector::filterLocations(vector<Rect>& foundLocations) const
{
	vector<Rect> foundLocationsFiltered;

	for (auto i = foundLocations.begin(); i < foundLocations.end(); i++)
	{
		auto j = foundLocations.begin();
		for (; j < foundLocations.end(); j++)
		{
			if (j != i && ((*i) & (*j)) == *i)
				break;
		}
		if (j == foundLocations.end())
		{
			foundLocationsFiltered.push_back(*i);
		}
	}

	return foundLocationsFiltered;
}