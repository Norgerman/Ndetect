#include "output_modifier.h"

bool DefaultOutputModifier::modifier(shared_ptr<Mat> mat) const
{
	return true;
}