#include "invalid_params_exception.h"

InvalidParamsException::InvalidParamsException()
	:exception()
{

}

InvalidParamsException::InvalidParamsException(const char* message)
	:exception(message)
{

}