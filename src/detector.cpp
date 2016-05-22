#include "detector.h"
#include "make_unique.hpp"

Detector::Detector(const char* filename)
	:Detector(imread(filename))
{
}

Detector::Detector(const string& filename)
	:Detector(imread(filename))
{
}

Detector::Detector(const Mat& img)
{
	if (!img.data)
	{
		throw InvalidParamsException(1000, "Invalid image", __func__, __FILE__, __LINE__);
	}
	_image = img.clone();
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

	_peopleDectectHog->detectMultiScale(_image, foundLocations, 0, winStride, padding, 1.05, 2);

	return filterLocations(foundLocations);
}

bool Detector::cut(vector<Rect>& foundLocations, vector<Mat>& resultSet) const
{
	for (auto& i : foundLocations)
	{
		resultSet.push_back(_image(i));
	}
	return true;
}

bool Detector::cut(vector<Rect>& foundLocations, fs::path& saveDir) const
{
	stringstream ss;

	if (!fs::exists(saveDir))
	{
		fs::create_directories(saveDir);
	}

	for (size_t i = 0; i < foundLocations.size(); i++)
	{
		const Mat& img = _image(foundLocations[i]);
		fs::path filePath = saveDir;
		ss << i << ".jpg";
		filePath.append(ss.str());
		ss.clear();
		ss.str("");
		imwrite(filePath.string(), img);
	}
	return true;
}

Detector::~Detector()
{

}

const Mat Detector::getImage() const
{
	return _image;
}

const Mat Detector::markOnSource(vector<Rect>& foundLocations) const
{
	Mat image = _image.clone();
	
	for (auto& location : foundLocations)
	{
		rectangle(image, location.tl(), location.br(), Scalar(0, 0, 255), 2);
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