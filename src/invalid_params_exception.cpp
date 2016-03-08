#include "invalid_params_exception.h"

InvalidParamsException::InvalidParamsException()
	:exception()
{

}

#if defined(WIN32) || defined(_WIN32)
InvalidParamsException::InvalidParamsException(const char* message)
#else
InvalidParamsException::InvalidParamsException(const char*& message)
#endif
	:exception(message)
{

}