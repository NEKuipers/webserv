#include "ConfigurationState.hpp"
#include "ConvertException.hpp"
#include "ConfigBase.hpp"

#include <stdlib.h>	// realpath
#include <iostream>

ConfigurationState::ConfigurationState() : AcceptedMethods(), Root(""), ExpectedRootExtension(""), ErrorUri(""), MaxBodySize(DEFAULT_MAX_BODY_SIZE), RedirectBase(NULL) { }
ConfigurationState::ConfigurationState(ConfigBase* RedirectBase) : AcceptedMethods(), Root(""), ExpectedRootExtension(""), ErrorUri(""), MaxBodySize(DEFAULT_MAX_BODY_SIZE), RedirectBase(RedirectBase) {}

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
	ExpectedRootExtension = From.ExpectedRootExtension;
	ErrorUri = From.ErrorUri;
	MaxBodySize = From.MaxBodySize;
	RedirectBase = From.RedirectBase;
	AcceptedMethods = From.AcceptedMethods;

	// return the existing object so we can chain this operator
	return *this;
}

bool ConfigurationState::AcceptsMethod(const std::string& Method) const
{
	if (AcceptedMethods.size() == 0)
		return true;

	for (std::vector<std::string>::const_iterator It = AcceptedMethods.begin(); It != AcceptedMethods.end(); It++)
		if (*It == Method)
			return true;

	return false;
}

bool ConfigurationState::IsValidWithRequest(const ConfigRequest& Request) const
{
	if (Request.GetContentLength() > MaxBodySize)
		return false;
	if (!AcceptsMethod(Request.GetMethod()))
		return false;

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
	else if (Args.at(0) == "expected_root")
	{
		if (Args.size() > 2)
			throw ConvertException("ConfigLine", "expected_root", "too many arguments, Expected 2, but got " + std::to_string(Args.size()));

		ExpectedRootExtension = "/" + InterperetEnvVariable(Args.size() > 1 ? Args.at(1) : "", NULL);

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
	else if (Args.at(0) == "accepted_methods")
	{
		AcceptedMethods.clear();
		AcceptedMethods.insert(AcceptedMethods.end(), Args.begin() + 1, Args.end());
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

bool ConfigurationState::IsFileValid(const std::string& FilePath, const ConfigRequest& Request) const
{
	// Check if the cgi is inside the Root directory, Small problem: If you symlink outside, it still fails, why is checked? Well, you dont want someone asking for cgi "../../Makecgi" or whatever other cgi
	char ActualPath[PATH_MAX+1];	// Not sure how i feel about allocating 1025 bytes on the stack, also can't be static, thread safety ya know?
	char* ptr = realpath(FilePath.c_str(), ActualPath);
	if (!ptr)
		return false;

	std::string Combined = Root + ExpectedRootExtension;
	if (strncmp(ActualPath, Combined.c_str(), Combined.length()))
	{
		std::cerr << Request << ": Asked for '" << FilePath << "', But was not inside the expected root directory: '" << Combined << "'!" << std::endl;
		return false;
	}

	return true;
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

	ReplaceAll(Copy, "$root", Root);
	ReplaceAll(Copy, "$expected_root", ExpectedRootExtension);
	
	if (Request)
	{
		ReplaceAll(Copy, "$uri", Request->GetUri());
	}

	return Copy;
}