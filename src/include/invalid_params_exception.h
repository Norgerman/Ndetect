#ifndef INVALID_IMAGE_EXCEPTION
#define INVALID_IMAGE_EXCEPTION

#include "stdafx.h"

class InvalidParamsException : public std::exception
{
public:
#if defined(WIN32) || defined(_WIN32)
	InvalidParamsException(const char* message);
#else
	InvalidParamsException(const char*& message);
#endif

	InvalidParamsException();
};

#endif // !INVALID_IMAGE_EXCEPTION
