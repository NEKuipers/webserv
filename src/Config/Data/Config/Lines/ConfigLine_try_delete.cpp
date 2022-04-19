#include "ConfigLine_try_delete.hpp"

#include <fstream>
#include <iostream>
#include "ConfigDeleteResponse.hpp"
#include "MethodException.hpp"
#include "PathUtils.hpp"

#include <stdlib.h>	// realpath

static std::string* AcceptedMethods[] = {
	new std::string("DELETE"),
	NULL
};

ConfigLine_try_delete::ConfigLine_try_delete() { }
ConfigLine_try_delete::ConfigLine_try_delete(const ConfigLine_try_delete& From) 
{
	this->operator=(From);
}
ConfigLine_try_delete::ConfigLine_try_delete(const std::vector<std::string>& Files, const ConfigurationState& _Configuration) : ConfigLine_base_try_file(Files, "try_delete", AcceptedMethods, _Configuration) { }

ConfigLine_try_delete::~ConfigLine_try_delete()
{

}

ConfigLine_try_delete& ConfigLine_try_delete::operator = (const ConfigLine_try_delete& From)
{
	static_cast<ConfigLine_base_try_file*>(this)->operator=(From);

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
	Stream << "Try delete ";
	static_cast<const ConfigLine_base_try_file*>(this)->Print(Stream);
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

ConfigResponse* ConfigLine_try_delete::GetResponseForFile(const std::string& FullPath, const std::string& PartialPath, const ConfigErrorReasons &ErrorReasons) const
{
	(void)PartialPath;
	return new ConfigDeleteResponse(FullPath, ErrorReasons);
}