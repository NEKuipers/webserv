#include "ConfigResponse.hpp"

ConfigResponse::ConfigResponse(const ConfigErrorReasons& ErrorReasons) : ErrorReasons(ErrorReasons) {}
ConfigResponse::~ConfigResponse()
{
	
}

bool ConfigResponse::RequiresBody() const
{
	return false;
}

const ConfigErrorReasons& ConfigResponse::GetErrorReasons()
{
	return ErrorReasons;
}