#include "ConfigBase.hpp"

ConfigBase::ConfigBase() : Configuration() {}
ConfigBase::ConfigBase(const ConfigurationState& Configuration) : Configuration(Configuration) {}
ConfigBase::ConfigBase(const ConfigBase& From)
{
	this->operator=(From);
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

ConfigResponse* ConfigBase::GetResponse(const ConfigRequest& Request) const
{
	if (Configuration.IsValidWithRequest(Request))
		return GetBaseResponse(Request);
	return NULL;
}