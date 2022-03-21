#include "ConfigLine_try_file.hpp"

#include <fstream>
#include <iostream>
#include "FileResponse.hpp"

ConfigLine_try_file::ConfigLine_try_file() : Files() { }
ConfigLine_try_file::ConfigLine_try_file(const ConfigLine_try_file& From) : Files()
{
	this->operator=(From);
}
ConfigLine_try_file::ConfigLine_try_file(const std::vector<std::string>& Files, const ConfigurationState& Configuration) : ConfigBase(Configuration), Files(Files) { }

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
	(void)Request;

	for (std::vector<std::string>::const_iterator It = Files.begin(); It != Files.end(); It++)
	{
		std::string File = *It;
		// TODO: We also need to parse env strings, eg: replace $uri with Request.Uri

		File = Configuration.Root + "/" + File;	// Isn't there a utility function that combines paths?

		// TODO: I dont like allocating just to see if it is successfull, but FileResponse takes in a pointer, since you can't copy a stream, and this is OLD cpp, so doing some move magic isn't an option either
		std::ifstream* Stream = new std::ifstream(File);
		if (Stream->is_open())
			return new FileResponse(File, Stream);
		delete Stream;
	}
	return NULL;
}

ConfigLine_try_file* ConfigLine_try_file::TryParse(const ConfigLine& Line, const ConfigurationState& Configuration)
{
	std::vector<std::string> Args = Line.GetArguments();
	if (Args.at(0) != "try_files")
		return NULL;
	
	// Remove the first arg, the rest are files as arguments
	std::vector<std::string> New;
	New.insert(New.begin(), Args.begin() + 1, Args.end());
	return new ConfigLine_try_file(New, Configuration);
}