#ifndef INVALID_PARAMS_EXCEPTION_H
#define INVALID_PARAMS_EXCEPTION_H

#include <opencv2/core.hpp>

using cv::String;
using cv::Exception;

class InvalidParamsException : public Exception
{
public:
	InvalidParamsException(int code, const String& err, const String& func, const String& file, int line);
	InvalidParamsException();
};

#endif // !INVALID_PARAMS_EXCEPTION_H
