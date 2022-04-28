#include "ConfigurationState.hpp"
#include "ConvertException.hpp"
#include "ConfigBase.hpp"
#include "ConfigErrorResponse.hpp"
#include "ConfigDirectoryResponse.hpp"
#include "PathUtils.hpp"

#include <stdlib.h>	// realpath
#include <iostream>
#include "ToString.hpp"
#include <cstring>	// Linux strncmp
#include <cassert>	// linux assert()
#include <limits.h>	// Linux PATH_MAX

ConfigurationState::ConfigurationState() : AcceptedMethods(), ErrorPath(""), MaxBodySize(DEFAULT_MAX_BODY_SIZE), Root(""), LocationRoot(""), DirectoryListing(false), RedirectBase(NULL) { UpdateCombinedRoot(); }
ConfigurationState::ConfigurationState(ConfigBase* RedirectBase) : AcceptedMethods(), ErrorPath(""), MaxBodySize(DEFAULT_MAX_BODY_SIZE), Root(""), LocationRoot(""), DirectoryListing(false), RedirectBase(RedirectBase) { UpdateCombinedRoot(); }

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
	ErrorPath = From.ErrorPath;
	MaxBodySize = From.MaxBodySize;
	RedirectBase = From.RedirectBase;
	AcceptedMethods = From.AcceptedMethods;
	DirectoryListing = From.DirectoryListing;

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

ConfigurationState::ValidRequestReason ConfigurationState::IsValidWithRequest(const ConfigRequest& Request) const
{
	if (Request.GetContentLength() > MaxBodySize)
		return ValidRequestReason_BodyTooBig;
	if (!AcceptsMethod(Request.GetMethod()))
		return ValidRequestReason_WrongMethod;

	return ValidRequestReason_Valid;
}

bool ConfigurationState::EatLine(const ConfigLine& Line)
{
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
	else if (Args.at(0) == "error_path")
	{
		if (Args.size() > 2)
			throw ConvertException("ConfigLine", "error_path", "too many arguments");

		ErrorPath = Args.size() > 1 ? Args.at(1) : "";
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
	else if (Args.at(0) == "directory_listing")
	{
		if (Args.size() != 2)
			throw ConvertException("ConfigLine", "directory_listing", "too manny arguments");
		const std::string& Arg = Args.at(1);

		if (Arg != "on" && Arg != "off")
			throw ConvertException("ConfigLine", "directory_listing", "argument was not 'on' or 'off'");
		
		DirectoryListing = Arg == "on";
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

ConfigResponse* ConfigurationState::Redirect(const ConfigRequest& Request, std::string Path, ConfigErrorReasons& ErrorReasons) const
{
	if (!RedirectBase)
		return NULL;

	ConfigRequest* NewRequest = Request.RedirectPath(Path);
	if (!NewRequest)
		return NULL;

	ConfigResponse* Response = RedirectBase->GetResponse(*NewRequest, ErrorReasons);
	delete NewRequest;

	return Response;
}

ConfigResponse* ConfigurationState::GetDirectoryResponse(const ConfigRequest& Request, ConfigErrorReasons& ErrorReasons) const
{
	if (Request.GetMethod() != "GET")
		return NULL;
	if (Request.GetPath().size() > 0 && Request.GetPath().at(Request.GetPath().length()-1) != '/')
		return NULL;	// Quick return

	std::string NewDirectory = PathUtils::combine(GetCombinedRoot(), RemoveLocationRoot(Request.GetPath()));
	if (IsPathValid(NewDirectory, Request, NULL) != PathType_ValidFile || PathUtils::pathType(NewDirectory) != PathUtils::DIRECTORY)
		return NULL;
	
	ErrorReasons.AddAllowedMethod("GET");
	return new ConfigDirectoryResponse(NewDirectory, ErrorReasons);
}

ConfigResponse* ConfigurationState::Error(const ConfigRequest& Request, ConfigErrorReasons& ErrorReasons) const
{
	if (DirectoryListing)
	{
		ConfigResponse* Response = GetDirectoryResponse(Request, ErrorReasons);
		if (Response)
			return Response;
	}

	ErrorReasons.Err_ErrorPage();
	
	if (ErrorPath != "")
		return Redirect(Request, ErrorPath, ErrorReasons);
	return new ConfigErrorResponse(ErrorReasons);
}

const std::string& ConfigurationState::GetRoot() const { return Root; }
const std::string& ConfigurationState::GetCombinedRoot() const { return CombinedRoot; }
const std::string& ConfigurationState::GetRawLocationRoot() const { return RawLocationRoot; }
const std::string& ConfigurationState::GetLocationRoot() const { return LocationRoot; }

std::string ConfigurationState::RemoveLocationRoot(const std::string& Path) const
{
	if (RawLocationRoot.size() == 0)
		return Path;

	//std::cout << "Removing " << RawLocationRoot << " From PATH: " << Path << std::endl;
	std::string RemovedSlash = RawLocationRoot.substr(1);
	if (Path == RemovedSlash)
		return "";

	assert(Path.rfind(RemovedSlash + "/", 0) != std::string::npos);	// Remove first slash, and move it to the end
	return Path.substr(RawLocationRoot.size());
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

ConfigurationState::PathType ConfigurationState::RawIsPathValid(const std::string& Path, const ConfigRequest& Request, std::string* ErrorPath) const
{
	ConfigurationState::PathType Ret = PathType_ValidFile;

	// Check if the cgi is inside the Root directory, Small problem: If you symlink outside, it still fails, why is checked? Well, you dont want someone asking for cgi "../../Makecgi" or whatever other cgi
	char buff[PATH_MAX];
	char* ptr = realpath(Path.c_str(), buff);

	if (!ptr)
	{
		if (ErrorPath) *ErrorPath = std::string(buff);	// Note: This may override the value of Path
		Ret = (ConfigurationState::PathType)(Ret | PathType_ExactFileNonExistent);	// Why can't i 'Ret |= PathType_ExactFileNonExistent', Why must it be casted, why C++, WHY!?
	}

	if (strncmp(buff, CombinedRoot.c_str(), CombinedRoot.length()))
	{
		std::cerr << Request << ": Asked for '" << Path << "', But was not inside the combined root directory: '" << CombinedRoot << "'!" << std::endl;
		Ret = (ConfigurationState::PathType)(Ret | PathType_Invalid);
	}

	return Ret;
}

ConfigurationState::PathType ConfigurationState::IsPathValid(const std::string& Path, const ConfigRequest& Request, std::string* ErrorPath) const
{
	(void)Path;
	(void)Request;
	(void)ErrorPath;
	return PathType_ValidFile;
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

std::string ConfigurationState::InterperetEnvVariableUserVariables(const std::string& String, const ConfigRequest& Request, bool& MustValidate) const
{
	std::string Copy = InterperetEnvVariable(String);

	MustValidate |= ReplaceAll(Copy, "$apath", Request.GetPath());
	MustValidate |= ReplaceAll(Copy, "$path", RemoveLocationRoot(Request.GetPath()));

	return Copy;
}