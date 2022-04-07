#include "ConfigResponse.hpp"

ConfigResponse::ConfigResponse() {}
ConfigResponse::~ConfigResponse()
{
	
}

bool ConfigResponse::RequiresBody()
{
	return false;
}