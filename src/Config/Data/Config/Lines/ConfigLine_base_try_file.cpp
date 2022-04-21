#include "ConfigLine_base_try_file.hpp"

#include <fstream>
#include <iostream>
#include "ConfigDeleteResponse.hpp"
#include "MethodException.hpp"
#include "PathUtils.hpp"

#include <stdlib.h>	// realpath

ConfigLine_base_try_file::ConfigLine_base_try_file() : Files() { }
ConfigLine_base_try_file::ConfigLine_base_try_file(const ConfigLine_base_try_file& From) : Files()
{
	this->operator=(From);
}
ConfigLine_base_try_file::ConfigLine_base_try_file(const std::vector<std::string>& Files, const std::string& ConfigLineName, const std::string* const* AcceptedMethods, const ConfigurationState& _Configuration) : ConfigBase(_Configuration), Files(Files)
{
	std::vector<std::string> FilteredAcceptedMethods;

	for (int i = 0; AcceptedMethods[i]; i++)
		if (Configuration.AcceptsMethod(*AcceptedMethods[i]))
			FilteredAcceptedMethods.push_back(*AcceptedMethods[i]);

	if (FilteredAcceptedMethods.size() == 0)
	{
		std::string ErrorMethods = "";
		for (int i = 0; AcceptedMethods[i]; i++)
		{
			if (i != 0)
				ErrorMethods += "/";
			ErrorMethods += *AcceptedMethods[i];
		}
		throw MethodException(ConfigLineName, ErrorMethods);
	}

	// We only accept the filtered methods
	Configuration.AcceptedMethods = FilteredAcceptedMethods;
}

ConfigLine_base_try_file::~ConfigLine_base_try_file()
{

}

ConfigLine_base_try_file& ConfigLine_base_try_file::operator = (const ConfigLine_base_try_file& From)
{
	static_cast<ConfigBase*>(this)->operator=(From);
	Files = From.Files;

	// return the existing object so we can chain this operator
	return *this;
}

std::ostream& operator<<(std::ostream& Stream, const ConfigLine_base_try_file& ConfigLine_base_try_file)
{
	ConfigLine_base_try_file.Print(Stream);
	return Stream;
}

void ConfigLine_base_try_file::Print(std::ostream& Stream) const
{
	Stream << "base files: ";
	for (std::vector<std::string>::const_iterator It = Files.begin(); It != Files.end(); It++)
	{
		if (It != Files.begin())
			Stream << ", ";
		Stream << *It;
	}
}

ConfigResponse* ConfigLine_base_try_file::GetBaseResponse(const ConfigRequest& Request, ConfigErrorReasons& ErrorReasons) const
{
	for (std::vector<std::string>::const_iterator It = Files.begin(); It != Files.end(); It++)
	{
		bool MustValidate = false;
		std::string File = PathUtils::combine(Configuration.GetCombinedRoot(), Configuration.InterperetEnvVariableUserVariables(*It, Request, MustValidate));
		if (!PathUtils::IsFile(File))
			continue;

		if (AcceptsPartialPath())
		{
			ConfigurationState::PathType PathType = Configuration.IsPathValid(File, Request, &File);
			//std::cout << "Get '" << File << "' => " << PartialPath << ", With pathtype: " << PathType << std::endl;
			
			if (MustValidate && (PathType & ConfigurationState::PathType_Invalid))
				continue;
		}
		else
		{
			if (MustValidate && Configuration.IsPathValid(File, Request, NULL) != ConfigurationState::PathType_ValidFile)
				continue;
		}

		ConfigResponse* Response = GetResponseForFile(Request, File, ErrorReasons);
		if (Response)
		{
			ErrorReasons.AddAllowedMethods(Configuration.AcceptedMethods);
			return Response;
		}
	}
	return NULL;
}

bool ConfigLine_base_try_file::WouldHaveResponded(const ConfigRequest& Request) const
{
	for (std::vector<std::string>::const_iterator It = Files.begin(); It != Files.end(); It++)
	{
		bool MustValidate = false;
		std::string File = Configuration.InterperetEnvVariableUserVariables(*It, Request, MustValidate);
		File = Configuration.GetCombinedRoot() + "/" + File;	// Isn't there a utility function that combines paths?

		if (MustValidate)
		{
			ConfigurationState::PathType PathType = Configuration.IsPathValid(File, Request, NULL);

			if (AcceptsPartialPath()
				? (PathType & ConfigurationState::PathType_Invalid)
				: (PathType != ConfigurationState::PathType_ValidFile))
				continue;
		}

		return true;
	}
	return false;
}


bool ConfigLine_base_try_file::AcceptsPartialPath() const
{
	return false;
}