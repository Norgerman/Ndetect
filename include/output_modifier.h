#ifndef OUTPUT_MODIFIER_H
#define OUTPUT_MODIFIER_H

#include <memory>
#include <opencv2/core.hpp>

using namespace std;
using namespace cv;

class OutputModifier
{
public:
	virtual bool modifier(shared_ptr<Mat> picture) const = 0;
	virtual ~OutputModifier() {};
};

class DefaultOutputModifier : public OutputModifier
{
public:
	virtual bool modifier(shared_ptr<Mat> picture) const;
};

#endif // !OUTPUT_MODIFIER_H