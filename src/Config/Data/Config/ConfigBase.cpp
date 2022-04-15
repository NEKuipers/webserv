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

#include <iostream>
ConfigResponse* ConfigBase::GetResponse(const ConfigRequest& Request, ConfigErrorReasons& ErrorReasons) const
{
	ConfigurationState::ValidRequestReason Reason = ConfigurationState::ValidRequestReason_Valid;
	if (ChecksConfiguration())
		Reason = Configuration.IsValidWithRequest(Request);
	
	if (Reason == ConfigurationState::ValidRequestReason_Valid)
		return GetBaseResponse(Request, ErrorReasons);
	else if (WouldHaveResponded(Request))
	{
		ErrorReasons.AddAllowedMethods(Configuration.AcceptedMethods);
		if (Reason & ConfigurationState::ValidRequestReason_WrongMethod)	ErrorReasons.Err_WrongMethod();
		if (Reason & ConfigurationState::ValidRequestReason_BodyTooBig)	ErrorReasons.Err_BodyTooBig();
	}
	return NULL;
}

bool ConfigBase::WouldHaveResponded(const ConfigRequest& Request) const
{
	(void)Request;
	return false;
}

ConfigResponse* ConfigBase::GetIteratorResponse(std::vector<ConfigBase*>::const_iterator& It, const std::vector<ConfigBase*>::const_iterator& ItEnd, const ConfigRequest& Request, ConfigErrorReasons& ErrorReasons) const
{
	It++;
	(void)ItEnd;
	return GetResponse(Request, ErrorReasons);
}

bool ConfigBase::ChecksConfiguration() const
{
	return true;
}
