#include "ConfigurationState.hpp"
#include "ConvertException.hpp"
#include "ConfigBase.hpp"

#include <stdlib.h>	// realpath

ConfigurationState::ConfigurationState() : Root(""), ErrorUri(""), MaxBodySize(DEFAULT_MAX_BODY_SIZE), RedirectBase(NULL)  { }
ConfigurationState::ConfigurationState(ConfigBase* RedirectBase) : Root(""), ErrorUri(""), MaxBodySize(DEFAULT_MAX_BODY_SIZE), RedirectBase(RedirectBase) {}

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
	RedirectBase = From.RedirectBase;

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
			throw ConvertException("ConfigLine", "root", "too many arguments, Expected 2, but got " + std::to_string(Args.size()));

		Root = InterperetEnvVariable(Args.size() > 1 ? Args.at(1) : "", NULL);

		// Now convert it to the real path
		char ActualPath[PATH_MAX+1];
		char* ptr = realpath(Root.c_str(), ActualPath);
		if (!ptr)
			throw ConvertException("ConfigLine", "root", "Root does not exist!");

		Root = std::string(ActualPath);

		return true;
	}
	else if (Args.at(0) == "error_uri")
	{
		if (Args.size() > 2)
			throw ConvertException("ConfigLine", "error_uri", "too manny arguments");

		ErrorUri = Args.size() > 1 ? Args.at(1) : "";
		return true;
	}
	else if (Args.at(0) == "client_max_body_size")
	{
		if (Args.size() > 2)
			throw ConvertException("ConfigLine", "client_max_body_size", "too manny arguments");

		char* End;
		MaxBodySize = std::strtoul(Args.at(1).c_str(), &End, 10);
		// Check if the string contained more than just a number
		if (End != &Args.at(1).c_str()[Args.at(1).length()])
			throw ConvertException("ConfigLine", "client_max_body_size", "size contained more than just a number");
		return true;
	}

	return false;
}

ConfigResponse* ConfigurationState::Redirect(const ConfigRequest& Request, std::string Uri) const
{
	if (!RedirectBase)
		return NULL;

	ConfigRequest* NewRequest = Request.RedirectUri(Uri);
	if (!NewRequest)
		return NULL;
	
	ConfigResponse* Response = RedirectBase->GetResponse(*NewRequest);
	delete NewRequest;

	return Response;
}

ConfigResponse* ConfigurationState::Error(const ConfigRequest& Request) const
{
	if (ErrorUri != "")
		return Redirect(Request, ErrorUri);
	return NULL;
}

static void ReplaceAll(std::string& Str, const std::string& Find, const std::string& Replace)
{
	while (true) {
		size_t Found = Str.find(Find, 0);
		if (Found == std::string::npos)
			break;

		Str.replace(Found, Find.length(), Replace);
	}
}

std::string ConfigurationState::InterperetEnvVariable(const std::string& String, const ConfigRequest* Request) const
{
	std::string Copy = String;

	if (Request)
	{
		ReplaceAll(Copy, "$uri", Request->GetUri());
	}
	ReplaceAll(Copy, "$root", Root);

	return Copy;
}