#include "face_modifier.h"

FaceModifier::FaceModifier()
{
	_faceClassifier = make_shared<CascadeClassifier>();
	if (!_faceClassifier->load(CASECADE_NAME_1))
	{
		throw InvalidParamsException(1000, "Casecade file not found", __func__, __FILE__, __LINE__);
	}
}

bool FaceModifier::modifier(shared_ptr<Mat> picture) const
{
	std::vector<Rect> faces;
	Mat pic_gray;

	cvtColor(*picture, pic_gray, CV_BGR2GRAY);
	equalizeHist(pic_gray, pic_gray);

	_faceClassifier->detectMultiScale(pic_gray, faces, 1.1, 2, 0 | CV_HAAR_SCALE_IMAGE, Size(30, 30));

	if (faces.size() > 0)
	{
		*picture = (*picture)(faces[0]);
		return true;
	}
	else
	{
		return false;
	}
}