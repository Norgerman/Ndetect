#ifndef FACE_MODIFIER_H
#define FACE_MODIFIER_H

#include <opencv2/objdetect.hpp>
#include <opencv2/imgproc.hpp>
#include "output_modifier.h"
#include "invalid_params_exception.h"


class FaceModifier : public OutputModifier
{
public:
	FaceModifier();
	virtual bool modifier(shared_ptr<Mat> picture) const;
private:
	shared_ptr<CascadeClassifier> _faceClassifier;
};

#endif // !FACE_MODIFIER_H