#include "ConfigLine_try_cgi.hpp"

#include <fstream>
#include <iostream>
#include "ConfigCgiResponse.hpp"
#include "MethodException.hpp"
#include "PathUtils.hpp"

#include <stdlib.h>	// realpath

static std::string* AcceptedMethods[] = {
	new std::string("GET"),
	new std::string("POST"),
	NULL
};

ConfigLine_try_cgi::ConfigLine_try_cgi() { }
ConfigLine_try_cgi::ConfigLine_try_cgi(const ConfigLine_try_cgi& From)
{
	this->operator=(From);
}
ConfigLine_try_cgi::ConfigLine_try_cgi(const std::vector<std::string>& cgis, const ConfigurationState& _Configuration) : ConfigLine_base_try_file(cgis, "try_cgi", AcceptedMethods, _Configuration) { }

ConfigLine_try_cgi::~ConfigLine_try_cgi()
{

}

ConfigLine_try_cgi& ConfigLine_try_cgi::operator = (const ConfigLine_try_cgi& From)
{
	static_cast<ConfigBase*>(this)->operator=(From);

	// return the existing object so we can chain this operator
	return *this;
}

std::ostream& operator<<(std::ostream& Stream, const ConfigLine_try_cgi& ConfigLine_try_cgi)
{
	ConfigLine_try_cgi.Print(Stream);
	return Stream;
}

void ConfigLine_try_cgi::Print(std::ostream& Stream) const
{
	Stream << "Try cgis: ";
	static_cast<const ConfigLine_base_try_file*>(this)->Print(Stream);
}

/*
ConfigResponse* ConfigLine_try_cgi::GetBaseResponse(const ConfigRequest& Request, ConfigErrorReasons& ErrorReasons) const
{
	for (std::vector<std::string>::const_iterator It = cgis.begin(); It != cgis.end(); It++)
	{
		bool MustValidate = false;
		std::string FullPath = Configuration.InterperetEnvVariableUserVariables(*It, &Request, MustValidate);
		FullPath = Configuration.GetCombinedRoot() + "/" + FullPath;	// Isn't there a utility function that combines paths?
		
		std::string PartialPath;
		ConfigurationState::PathType PathType = Configuration.IsPathValid(FullPath, Request, &PartialPath);
		if (!PathUtils::IsFile(FullPath) || (MustValidate && (PathType & ConfigurationState::PathType_Invalid)))
			continue;
		
		if (!(PathType & ConfigurationState::PathType_ExactFileNonExistent))
			PartialPath = FullPath;

		ErrorReasons.AddAllowedMethods(Configuration.AcceptedMethods);
		return new ConfigCgiResponse(PartialPath, FullPath, ErrorReasons);
	}
	return NULL;
}

bool ConfigLine_try_cgi::WouldHaveResponded(const ConfigRequest& Request) const
{
	for (std::vector<std::string>::const_iterator It = cgis.begin(); It != cgis.end(); It++)
	{
		bool MustValidate = false;
		std::string CgiFile = Configuration.InterperetEnvVariableUserVariables(*It, &Request, MustValidate);
		CgiFile = Configuration.GetCombinedRoot() + "/" + CgiFile;	// Isn't there a utility function that combines paths?
		if (MustValidate && (Configuration.IsPathValid(CgiFile, Request, NULL) & ConfigurationState::PathType_Invalid))
			continue;
		
		return true;
	}

	return false;
}
*/

ConfigLine_try_cgi* ConfigLine_try_cgi::TryParse(const ConfigLine& Line, const ConfigurationState& Configuration)
{
	std::vector<std::string> Args = Line.GetArguments();
	if (Args.at(0) != "try_cgis" && Args.at(0) != "try_cgi")
		return NULL;
	
	// Remove the first arg, the rest are cgis as arguments
	std::vector<std::string> New;
	New.insert(New.begin(), Args.begin() + 1, Args.end());
	return new ConfigLine_try_cgi(New, Configuration);
}


bool ConfigLine_try_cgi::AcceptsPartialPath() const
{
	return true;
}

ConfigResponse* ConfigLine_try_cgi::GetResponseForFile(const std::string& FullPath, const std::string& PartialPath, const ConfigErrorReasons &ErrorReasons) const
{
	return new ConfigCgiResponse(PartialPath, FullPath, ErrorReasons);
}