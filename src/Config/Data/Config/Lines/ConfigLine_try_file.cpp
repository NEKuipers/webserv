#include "ConfigLine_try_file.hpp"
#include "ConfigFileResponse.hpp"

static std::string* AcceptedMethods[] = {
	new std::string("GET"),
	NULL
};

ConfigLine_try_file::ConfigLine_try_file() { }
ConfigLine_try_file::ConfigLine_try_file(const ConfigLine_try_file& From)
{
	this->operator=(From);
}
ConfigLine_try_file::ConfigLine_try_file(const std::vector<std::string>& Files, const ConfigurationState& _Configuration) : ConfigLine_base_try_file(Files, "try_file", AcceptedMethods, _Configuration) { }

ConfigLine_try_file::~ConfigLine_try_file() { }

ConfigLine_try_file& ConfigLine_try_file::operator = (const ConfigLine_try_file& From)
{
	static_cast<ConfigLine_base_try_file*>(this)->operator=(From);

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
	Stream << "Try files ";
	static_cast<const ConfigLine_base_try_file*>(this)->Print(Stream);
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

ConfigResponse* ConfigLine_try_file::GetResponseForFile(const ConfigRequest& Request, const std::string& FullPath, const ConfigErrorReasons &ErrorReasons) const
{
	(void)Request;
	
	std::ifstream* Stream = new std::ifstream(FullPath.c_str());	// Annoyingly, linux does not have a string contructor
	if (!Stream->is_open() || !Stream->good())
	{
		delete Stream;	// Well, realpath said the file exists, but we can't open it? Well, whatever, just continue
		return NULL;
	}
	
	return new ConfigFileResponse(FullPath, Stream, ErrorReasons);
}