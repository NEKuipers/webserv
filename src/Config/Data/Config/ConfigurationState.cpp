#include "ConfigurationState.hpp"
#include "ConvertException.hpp"

ConfigurationState::ConfigurationState() : Root(""), ErrorUri(""), MaxBodySize(DEFAULT_MAX_BODY_SIZE) {}

ConfigurationState::ConfigurationState(const ConfigurationState& From)
{
	this->operator=(From);
}

ConfigurationState::~ConfigurationState()
{
	
}

ConfigurationState& ConfigurationState::operator = (const ConfigurationState& From)
{
	Root = From.Root;
	ErrorUri = From.ErrorUri;
	MaxBodySize = From.MaxBodySize;

	// return the existing object so we can chain this operator
	return *this;
}

bool ConfigurationState::IsValidWithRequest(const ConfigRequest& Request) const
{
	if (Request.GetContentLength() > MaxBodySize)
		return false;
	// TODO: Check accepted methods

	return true;
}

bool ConfigurationState::EatLine(const ConfigLine& Line)
{
	// TODO: No if/elseif/elseif stuff, its ugly!
	std::vector<std::string> Args = Line.GetArguments();
	if (Args.at(0) == "root")
	{
		if (Args.size() > 2)
			throw ConvertException("Cannot read 'root' line, too manny arguments!");

		Root = Args.size() > 1 ? Args.at(1) : "";
		return true;
	}
	else if (Args.at(0) == "error_uri")
	{
		if (Args.size() > 2)
			throw ConvertException("Cannot read 'error_uri' line, too manny arguments!");

		ErrorUri = Args.size() > 1 ? Args.at(1) : "";
		return true;
	}
	else if (Args.at(0) == "client_max_body_size")
	{
		if (Args.size() > 2)
			throw ConvertException("Cannot read 'client_max_body_size' line, too manny arguments!");

		char* End;
		MaxBodySize = std::strtoul(Args.at(1).c_str(), &End, 10);
		// Check if the string contained more than just a number
		if (End != &Args.at(1).c_str()[Args.at(1).length()])
			throw ConvertException("Reading size from 'client_max_body_size' contained more than just a number!");
		return true;
	}

	return false;
}

ConfigResponse* ConfigurationState::Redirect(std::string Uri) const
{
	(void)Uri;
	// TODO: Redirects (Save Config as ConfigBase* Base, And call Base->GetResponse() with a new request with the new Uri)

	return NULL;
}

ConfigResponse* ConfigurationState::Error() const
{
	return Redirect(ErrorUri);
}