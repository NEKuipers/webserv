#include "ConfigLine_try_cgi.hpp"

#include <fstream>
#include <iostream>
#include "CgiResponse.hpp"
#include "MethodException.hpp"

#include <stdlib.h>	// realpath

ConfigLine_try_cgi::ConfigLine_try_cgi() : cgis() { }
ConfigLine_try_cgi::ConfigLine_try_cgi(const ConfigLine_try_cgi& From) : cgis()
{
	this->operator=(From);
}
ConfigLine_try_cgi::ConfigLine_try_cgi(const std::vector<std::string>& cgis, const ConfigurationState& _Configuration) : ConfigBase(_Configuration), cgis(cgis)
{
	bool AcceptsGet = Configuration.AcceptsMethod("GET");
	bool AcceptsPost = Configuration.AcceptsMethod("POST");
	if (!AcceptsGet && !AcceptsPost)
		throw MethodException("try_cgi", "GET/POST");

	// We only accept those 2 requests
	Configuration.AcceptedMethods.clear();
	if (AcceptsGet)
		Configuration.AcceptedMethods.push_back("GET");
	if (AcceptsPost)
		Configuration.AcceptedMethods.push_back("POST");
}

ConfigLine_try_cgi::~ConfigLine_try_cgi()
{

}

ConfigLine_try_cgi& ConfigLine_try_cgi::operator = (const ConfigLine_try_cgi& From)
{
	static_cast<ConfigBase*>(this)->operator=(From);
	cgis = From.cgis;

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
	for (std::vector<std::string>::const_iterator It = cgis.begin(); It != cgis.end(); It++)
	{
		if (It != cgis.begin())
			Stream << " ";
		Stream << *It;
	}
}

ConfigResponse* ConfigLine_try_cgi::GetBaseResponse(const ConfigRequest& Request) const
{
	for (std::vector<std::string>::const_iterator It = cgis.begin(); It != cgis.end(); It++)
	{
		bool MustValidate = false;
		std::string cgi = Configuration.InterperetEnvVariableUserVariables(*It, &Request, MustValidate);
		cgi = Configuration.Root + "/" + cgi;	// Isn't there a utility function that combines paths?
		
		if (MustValidate && !Configuration.IsFileValid(cgi, Request))
			continue;

		return new CgiResponse(cgi);
	}
	return NULL;
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