#include "ConfigBase.hpp"

ConfigBase::ConfigBase() : Configuration(this) {}
ConfigBase::ConfigBase(const ConfigurationState& Configuration) : Configuration(Configuration) {}
ConfigBase::ConfigBase(const ConfigBase& From) : Configuration(From.Configuration)
{
	//this->operator=(From);
}

ConfigBase& ConfigBase::operator = (const ConfigBase& From)
{
	this->Configuration = From.Configuration;

	// return the existing object so we can chain this operator
	return *this;
}

ConfigBase::~ConfigBase()
{
	
}

ConfigResponse* ConfigBase::GetResponse(const ConfigRequest& Request, ConfigCombinedResponse& CombinedResponse) const
{
	if (!ChecksConfiguration() || Configuration.IsValidWithRequest(Request))
		return GetBaseResponse(Request, CombinedResponse);
	else
		AddCombinedResponseIfNoResponse(Request, CombinedResponse);
	return NULL;
}

void ConfigBase::AddCombinedResponseIfNoResponse(const ConfigRequest& Request, ConfigCombinedResponse& CombinedResponse) const
{
	(void)Request;
	(void)CombinedResponse;
}

ConfigResponse* ConfigBase::GetIteratorResponse(std::vector<ConfigBase*>::const_iterator& It, const std::vector<ConfigBase*>::const_iterator& ItEnd, const ConfigRequest& Request, ConfigCombinedResponse& CombinedResponse) const
{
	It++;
	(void)ItEnd;
	return GetResponse(Request, CombinedResponse);
}

bool ConfigBase::ChecksConfiguration() const
{
	return true;
}
