#ifndef INVALID_IMAGE_EXCEPTION
#define INVALID_IMAGE_EXCEPTION

#include "stdafx.h"

class InvalidParamsException : public std::exception
{
public:
	InvalidParamsException(const char* message);
	InvalidParamsException();
};

#endif // !INVALID_IMAGE_EXCEPTION
