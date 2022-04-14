#include "ConfigCombinedResponse.hpp"

ConfigCombinedResponse::ConfigCombinedResponse() : AllowedMethods() {}
ConfigCombinedResponse::ConfigCombinedResponse(const ConfigCombinedResponse& From) : AllowedMethods()
{
	this->operator=(From);
}

ConfigCombinedResponse::~ConfigCombinedResponse()
{
	// TODO: Implement destructor
}

ConfigCombinedResponse& ConfigCombinedResponse::operator = (const ConfigCombinedResponse& From)
{
	AllowedMethods = From.AllowedMethods;

	// return the existing object so we can chain this operator
	return *this;
}

std::ostream& operator<<(std::ostream& Stream, const ConfigCombinedResponse& ConfigCombinedResponse)
{
	Stream << "ConfigCombinedResponse: Allowed methods:";
	for (std::vector<std::string>::const_iterator it = ConfigCombinedResponse.AllowedMethods.begin(); it != ConfigCombinedResponse.AllowedMethods.end(); it++)
		Stream << " " << *it;
	return Stream;
}

const std::vector<std::string>& ConfigCombinedResponse::GetAllowedMethods() const { return AllowedMethods; }
void ConfigCombinedResponse::AddAllowedMethod(const std::string& Method)
{
	for (std::vector<std::string>::const_iterator it = AllowedMethods.begin(); it != AllowedMethods.end(); it++)
		if (*it == Method)
			return;
	AllowedMethods.push_back(Method);
}

void ConfigCombinedResponse::AddAllowedMethods(const std::vector<std::string>& Methods)
{
	for (std::vector<std::string>::const_iterator it = Methods.begin(); it != Methods.end(); it++)
		AddAllowedMethod(*it);
}