#include "ConfigResponse.hpp"

ConfigResponse::ConfigResponse() {}
ConfigResponse::~ConfigResponse()
{
	
}

bool ConfigResponse::RequiresBody() const
{
	return false;
}