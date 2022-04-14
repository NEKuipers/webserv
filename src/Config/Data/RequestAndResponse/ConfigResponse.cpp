#include "ConfigResponse.hpp"

ConfigResponse::ConfigResponse(const ConfigCombinedResponse& CombinedResponse) : AllowedMethods(CombinedResponse.GetAllowedMethods()) {}
ConfigResponse::~ConfigResponse()
{
	
}

bool ConfigResponse::RequiresBody() const
{
	return false;
}

const std::vector<std::string>& ConfigResponse::GetAllowedMethods()
{
	return AllowedMethods;
}