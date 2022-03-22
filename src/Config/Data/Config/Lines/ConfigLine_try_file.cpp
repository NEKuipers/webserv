#include "ConfigLine_try_file.hpp"

#include <fstream>
#include <iostream>
#include "FileResponse.hpp"
#include "MethodException.hpp"

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

ConfigResponse* ConfigLine_try_file::GetBaseResponse(const ConfigRequest& Request) const
{
	for (std::vector<std::string>::const_iterator It = Files.begin(); It != Files.end(); It++)
	{
		std::string File = Configuration.InterperetEnvVariable(*It, &Request);
		File = Configuration.Root + "/" + File;	// Isn't there a utility function that combines paths?
		
		// Check if the File is inside the Root directory, Small problem: If you symlink outside, it still fails, why is checked? Well, you dont want someone asking for file "../../Makefile" or whatever other file
		char ActualPath[PATH_MAX+1];	// Not sure how i feel about allocating 1025 bytes on the stack, also can't be static, thread safety ya know?
		char* ptr = realpath(File.c_str(), ActualPath);
		if (!ptr)
			continue;	// The file does not exist

		if (strncmp(ActualPath, Configuration.Root.c_str(), Configuration.Root.length()))
		{
			std::cerr << Request << ": Asked for '" << File << "', But was not inside the root directory: '" << Configuration.Root << "'!" << std::endl;
			continue;	// The File path did NOT start with root, this is not what we want!
		}

		std::ifstream* Stream = new std::ifstream(ActualPath);
		if (Stream->is_open())
			return new FileResponse(File, Stream);
		delete Stream;	// Well, realpath said the file exists, but we can't open it? Well, whatever, just continue
	}
	return NULL;
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