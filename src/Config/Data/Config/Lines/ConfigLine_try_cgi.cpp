#include "ConfigLine_try_cgi.hpp"
#include "ConfigCgiResponse.hpp"

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

ConfigResponse* ConfigLine_try_cgi::GetResponseForFile(const ConfigRequest& Request, const std::string& FullPath,const ConfigErrorReasons &ErrorReasons) const
{
	// TODOOO: Better way to find where the path stops and where the PATH_INFO starts
	size_t loc = FullPath.find(".cgi");
	if (loc == std::string::npos) loc = FullPath.find(".py");
	if (loc == std::string::npos) return NULL;
	std::string PartialPath = FullPath.substr(0, loc + 4);
	return new ConfigCgiResponse(PartialPath, FullPath, Request.GetPath(), ErrorReasons);
}