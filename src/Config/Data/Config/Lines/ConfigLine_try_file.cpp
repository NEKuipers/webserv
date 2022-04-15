#include "ConfigLine_try_file.hpp"

#include <fstream>
#include <iostream>
#include "FileResponse.hpp"
#include "MethodException.hpp"
#include "PathUtils.hpp"

#include <stdlib.h>	// realpath

ConfigLine_try_file::ConfigLine_try_file() : Files() { }
ConfigLine_try_file::ConfigLine_try_file(const ConfigLine_try_file& From) : Files()
{
	this->operator=(From);
}
ConfigLine_try_file::ConfigLine_try_file(const std::vector<std::string>& Files, const ConfigurationState& _Configuration) : ConfigBase(_Configuration), Files(Files)
{
	if (!Configuration.AcceptsMethod("GET"))
		throw MethodException("try_file", "GET");

	// We only accept GET requests
	Configuration.AcceptedMethods.clear();
	Configuration.AcceptedMethods.push_back("GET");
}

ConfigLine_try_file::~ConfigLine_try_file()
{

}

ConfigLine_try_file& ConfigLine_try_file::operator = (const ConfigLine_try_file& From)
{
	static_cast<ConfigBase*>(this)->operator=(From);
	Files = From.Files;

	// return the existing object so we can chain this operator
	return *this;
}

std::ostream& operator<<(std::ostream& Stream, const ConfigLine_try_file& ConfigLine_try_file)
{
	ConfigLine_try_file.Print(Stream);
	return Stream;
}

void ConfigLine_try_file::Print(std::ostream& Stream) const
{
	Stream << "Try files: ";
	for (std::vector<std::string>::const_iterator It = Files.begin(); It != Files.end(); It++)
	{
		if (It != Files.begin())
			Stream << " ";
		Stream << *It;
	}
}

// I dont really like how there are 2 locations where it loops through the files, and i also dont really like the 'AddErrorReasonsIfNoResponse', the name is too long, but anything shorter and its not descriptive enough
ConfigResponse* ConfigLine_try_file::GetBaseResponse(const ConfigRequest& Request, ConfigErrorReasons& ErrorReasons) const
{
	for (std::vector<std::string>::const_iterator It = Files.begin(); It != Files.end(); It++)
	{
		bool MustValidate = false;
		std::string File = Configuration.InterperetEnvVariableUserVariables(*It, &Request, MustValidate);
		File = Configuration.GetCombinedRoot() + "/" + File;	// Isn't there a utility function that combines paths?

		if (!PathUtils::IsFile(File) || (MustValidate && Configuration.IsPathValid(File, Request, NULL) != ConfigurationState::PathType_ValidFile))
			continue;

		std::ifstream* Stream = new std::ifstream(File.c_str());	// Annoyingly, linux does not have a string contructor
		if (Stream->is_open() && Stream->good())
		{
			ErrorReasons.AddAllowedMethods(Configuration.AcceptedMethods);
			return new FileResponse(File, Stream, ErrorReasons);
		}
		delete Stream;	// Well, realpath said the file exists, but we can't open it? Well, whatever, just continue
	}
	return NULL;
}

bool ConfigLine_try_file::WouldHaveResponded(const ConfigRequest& Request) const
{
	for (std::vector<std::string>::const_iterator It = Files.begin(); It != Files.end(); It++)
	{
		bool MustValidate = false;
		std::string File = Configuration.InterperetEnvVariableUserVariables(*It, &Request, MustValidate);
		File = Configuration.GetCombinedRoot() + "/" + File;	// Isn't there a utility function that combines paths?
		if (MustValidate && Configuration.IsPathValid(File, Request, NULL) != ConfigurationState::PathType_ValidFile)
			continue;
		
		return true;
	}
	return false;
}

ConfigLine_try_file* ConfigLine_try_file::TryParse(const ConfigLine& Line, const ConfigurationState& Configuration)
{
	std::vector<std::string> Args = Line.GetArguments();
	if (Args.at(0) != "try_files" && Args.at(0) != "try_file")
		return NULL;

	// Remove the first arg, the rest are files as arguments
	std::vector<std::string> New;
	New.insert(New.begin(), Args.begin() + 1, Args.end());
	return new ConfigLine_try_file(New, Configuration);
}