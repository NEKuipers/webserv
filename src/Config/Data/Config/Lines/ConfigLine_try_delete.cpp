#include "ConfigLine_try_delete.hpp"

#include <fstream>
#include <iostream>
#include "ConfigDeleteResponse.hpp"
#include "MethodException.hpp"
#include "PathUtils.hpp"

#include <stdlib.h>	// realpath

ConfigLine_try_delete::ConfigLine_try_delete() : Files() { }
ConfigLine_try_delete::ConfigLine_try_delete(const ConfigLine_try_delete& From) : Files()
{
	this->operator=(From);
}
ConfigLine_try_delete::ConfigLine_try_delete(const std::vector<std::string>& Files, const ConfigurationState& _Configuration) : ConfigBase(_Configuration), Files(Files)
{
	if (!Configuration.AcceptsMethod("DELETE"))
		throw MethodException("try_delete", "DELETE");

	// We only accept DELETE requests
	Configuration.AcceptedMethods.clear();
	Configuration.AcceptedMethods.push_back("DELETE");
}

ConfigLine_try_delete::~ConfigLine_try_delete()
{

}

ConfigLine_try_delete& ConfigLine_try_delete::operator = (const ConfigLine_try_delete& From)
{
	static_cast<ConfigBase*>(this)->operator=(From);
	Files = From.Files;

	// return the existing object so we can chain this operator
	return *this;
}

std::ostream& operator<<(std::ostream& Stream, const ConfigLine_try_delete& ConfigLine_try_delete)
{
	ConfigLine_try_delete.Print(Stream);
	return Stream;
}

void ConfigLine_try_delete::Print(std::ostream& Stream) const
{
	Stream << "Try delete files: ";
	for (std::vector<std::string>::const_iterator It = Files.begin(); It != Files.end(); It++)
	{
		if (It != Files.begin())
			Stream << " ";
		Stream << *It;
	}
}

// I dont really like how there are 2 locations where it loops through the files, and i also dont really like the 'AddErrorReasonsIfNoResponse', the name is too long, but anything shorter and its not descriptive enough
ConfigResponse* ConfigLine_try_delete::GetBaseResponse(const ConfigRequest& Request, ConfigErrorReasons& ErrorReasons) const
{
	for (std::vector<std::string>::const_iterator It = Files.begin(); It != Files.end(); It++)
	{
		bool MustValidate = false;
		std::string File = Configuration.InterperetEnvVariableUserVariables(*It, &Request, MustValidate);
		File = Configuration.GetCombinedRoot() + "/" + File;	// Isn't there a utility function that combines paths?

		if (!PathUtils::IsFile(File) || (MustValidate && Configuration.IsPathValid(File, Request, NULL) != ConfigurationState::PathType_ValidFile))
			continue;

		return new ConfigDeleteResponse(File, ErrorReasons);
	}
	return NULL;
}

bool ConfigLine_try_delete::WouldHaveResponded(const ConfigRequest& Request) const
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

ConfigLine_try_delete* ConfigLine_try_delete::TryParse(const ConfigLine& Line, const ConfigurationState& Configuration)
{
	std::vector<std::string> Args = Line.GetArguments();
	if (Args.at(0) != "try_deletes" && Args.at(0) != "try_delete")
		return NULL;

	// Remove the first arg, the rest are files as arguments
	std::vector<std::string> New;
	New.insert(New.begin(), Args.begin() + 1, Args.end());
	return new ConfigLine_try_delete(New, Configuration);
}