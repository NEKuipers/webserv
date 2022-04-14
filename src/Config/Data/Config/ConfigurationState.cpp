#include "ConfigurationState.hpp"
#include "ConvertException.hpp"
#include "ConfigBase.hpp"
#include "ErrorResponse.hpp"

#include <stdlib.h>	// realpath
#include <iostream>
#include "ToString.hpp"
#include <cstring>	// Linux strncmp

ConfigurationState::ConfigurationState() : AcceptedMethods(), ErrorUri(""), MaxBodySize(DEFAULT_MAX_BODY_SIZE), Root(""), LocationRoot(""), RedirectBase(NULL) { UpdateCombinedRoot(); }
ConfigurationState::ConfigurationState(ConfigBase* RedirectBase) : AcceptedMethods(), ErrorUri(""), MaxBodySize(DEFAULT_MAX_BODY_SIZE), Root(""), LocationRoot(""), RedirectBase(RedirectBase) { UpdateCombinedRoot(); }

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
	CombinedRoot = From.CombinedRoot;
	RawLocationRoot = From.RawLocationRoot;
	LocationRoot = From.LocationRoot;
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
			throw ConvertException("ConfigLine", "root", "too many arguments, Expected 2, but got " + to_string(Args.size()));

		Root = InterperetEnvVariable(Args.size() > 1 ? Args.at(1) : "");

		// Now convert it to the real path
		char* ptr = realpath(Root.c_str(), NULL);
		if (!ptr)
			throw ConvertException("ConfigLine", "root", "Root does not exist!");

		Root = std::string(ptr);
		free(ptr);
		UpdateCombinedRoot();

		return true;
	}
	else if (Args.at(0) == "location_root")
	{
		if (Args.size() > 2)
			throw ConvertException("ConfigLine", "location_root", "too many arguments, Expected 2, but got " + to_string(Args.size()));

		LocationRoot = "/" + InterperetEnvVariable(Args.size() > 1 ? Args.at(1) : "");
		UpdateCombinedRoot();

		return true;
	}
	else if (Args.at(0) == "error_uri")
	{
		if (Args.size() > 2)
			throw ConvertException("ConfigLine", "error_uri", "too many arguments");

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

void ConfigurationState::AppendLocationRoot(const std::string& Location)
{
	RawLocationRoot += "/" + Location;
	LocationRoot += "/" + Location;
	UpdateCombinedRoot();
}

ConfigResponse* ConfigurationState::Redirect(const ConfigRequest& Request, std::string Uri, ConfigCombinedResponse& CombinedResponse) const
{
	if (!RedirectBase)
		return NULL;

	ConfigRequest* NewRequest = Request.RedirectUri(Uri);
	if (!NewRequest)
		return NULL;

	ConfigResponse* Response = RedirectBase->GetResponse(*NewRequest, CombinedResponse);
	delete NewRequest;

	return Response;
}

ConfigResponse* ConfigurationState::Error(const ConfigRequest& Request, ConfigCombinedResponse& CombinedResponse) const
{
	if (ErrorUri != "")
		return Redirect(Request, ErrorUri, CombinedResponse);
	return new ErrorResponse(CombinedResponse);
}

const std::string& ConfigurationState::GetRoot() const { return Root; }
const std::string& ConfigurationState::GetCombinedRoot() const { return CombinedRoot; }
const std::string& ConfigurationState::GetRawLocationRoot() const { return RawLocationRoot; }
const std::string& ConfigurationState::GetLocationRoot() const { return LocationRoot; }

std::string ConfigurationState::RemoveLocationRoot(const std::string& Uri) const
{
	if (RawLocationRoot.size() == 0)
		return Uri;

	//std::cout << "Removing " << RawLocationRoot << " From URI: " << Uri << std::endl;
	assert(Uri.rfind(RawLocationRoot.substr(1) + "/", 0) != std::string::npos);	// Remove first slash, and move it to the end
	return Uri.substr(RawLocationRoot.size());
}

void ConfigurationState::UpdateCombinedRoot()
{
	if (Root.length() != 0)
		CombinedRoot = Root + LocationRoot;
	else if (LocationRoot.length() != 0)
		CombinedRoot = "." + LocationRoot.substr(1);
	else
		CombinedRoot = ".";	// Both are empty, default to working directory
	//std::cout << "CombinedRoot '" << CombinedRoot << "' = '" << Root << "' + '" << LocationRoot << "'" << std::endl;
}

bool ConfigurationState::IsFileValid(const std::string& FilePath, const ConfigRequest& Request) const
{
	// Check if the cgi is inside the Root directory, Small problem: If you symlink outside, it still fails, why is checked? Well, you dont want someone asking for cgi "../../Makecgi" or whatever other cgi
	char* ptr = realpath(FilePath.c_str(), NULL);
	if (!ptr)
		return false;

	if (strncmp(ptr, CombinedRoot.c_str(), CombinedRoot.length()))
	{
		std::cerr << Request << ": Asked for '" << FilePath << "', But was not inside the combined root directory: '" << CombinedRoot << "'!" << std::endl;
		free(ptr);
		return false;
	}

	free(ptr);
	return true;
}

static bool ReplaceAll(std::string& Str, const std::string& Find, const std::string& Replace)
{
	bool ReplacedSomething = false;

	while (true) {
		size_t Found = Str.find(Find, 0);
		if (Found == std::string::npos)
			break;

		Str.replace(Found, Find.length(), Replace);
		ReplacedSomething = true;
	}
	return ReplacedSomething;
}

std::string ConfigurationState::InterperetEnvVariable(const std::string& String) const
{
	std::string Copy = String;

	ReplaceAll(Copy, "$root", Root);
	ReplaceAll(Copy, "$combined_root", CombinedRoot);
	ReplaceAll(Copy, "$location_root", LocationRoot);

	return Copy;
}

std::string ConfigurationState::InterperetEnvVariableUserVariables(const std::string& String, const ConfigRequest* Request, bool& MustValidate) const
{
	std::string Copy = InterperetEnvVariable(String);

	MustValidate |= ReplaceAll(Copy, "$auri", Request->GetUri());
	MustValidate |= ReplaceAll(Copy, "$uri", RemoveLocationRoot(Request->GetUri()));

	return Copy;
}