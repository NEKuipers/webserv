#include "ConfigListBase.hpp"
#include "PrefixStreambuf.hpp"

ConfigListBase::ConfigListBase() : ConfigBase() { }
ConfigListBase::ConfigListBase(const ConfigurationState& Configuration) : ConfigBase(Configuration) { }

ConfigListBase::~ConfigListBase()
{
	// TODO: Implement destructor
}

ConfigResponse* ConfigListBase::GetBaseResponse(const ConfigRequest& Request) const
{
	bool ErrorIfNoResult;

	EnterResult Result = Enters(Request);
	switch (Result)
	{
		case EnterResult_No:
			return NULL;
		case EnterResult_Enter:
			ErrorIfNoResult = false;
			break;
		case EnterResult_EnterAndError:
			ErrorIfNoResult = true;
			break;
		default:
			throw "Unknown EnterResult was returned!";	
	}

	for (std::vector<ConfigBase*>::const_iterator It = Children.begin(); It != Children.end(); It++)
	{
		const ConfigBase* Child = *It;

		ConfigResponse* Response = Child->GetResponse(Request);
		if (Response)
			return Response;
	}

	if (ErrorIfNoResult)
		return Configuration.Error(Request);
	return NULL;
}

bool ConfigListBase::ChecksConfiguration() const
{
	return false;
}

std::ostream& operator<<(std::ostream& Stream, const ConfigListBase& ConfigListBase)
{
	ConfigListBase.Print(Stream);
	return Stream;
}

void ConfigListBase::Print(std::ostream& Stream) const
{
	Stream << "{\n";
	PrefixStreambuf Prefix(Stream, "  ");
	for (std::vector<ConfigBase*>::const_iterator It = Children.begin(); It != Children.end(); It++)
	{
		(*It)->Print(Stream);
		Stream << "\n";
	}
	Prefix.End();
	Stream << "}";
}