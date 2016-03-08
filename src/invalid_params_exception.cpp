#include "invalid_params_exception.h"

InvalidParamsException::InvalidParamsException()
	:Exception()
{

}

InvalidParamsException::InvalidParamsException(int code, const String& err, const String& func, const String& file, int line)
	: Exception(code, err, func, file, line)
{

}