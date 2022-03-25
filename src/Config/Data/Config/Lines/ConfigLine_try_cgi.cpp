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
	if (!Configuration.AcceptsMethod("GET"))
		throw MethodException("try_cgi", "GET");

	// We only accept GET requests
	Configuration.AcceptedMethods.clear();
	Configuration.AcceptedMethods.push_back("GET");
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
		std::string cgi = Configuration.InterperetEnvVariable(*It, &Request);
		cgi = Configuration.Root + "/" + cgi;	// Isn't there a utility function that combines paths?
		
		// Check if the cgi is inside the Root directory, Small problem: If you symlink outside, it still fails, why is checked? Well, you dont want someone asking for cgi "../../Makecgi" or whatever other cgi
		char ActualPath[PATH_MAX+1];	// Not sure how i feel about allocating 1025 bytes on the stack, also can't be static, thread safety ya know?
		char* ptr = realpath(cgi.c_str(), ActualPath);
		if (!ptr)
			continue;	// The cgi does not exist

		if (strncmp(ActualPath, Configuration.Root.c_str(), Configuration.Root.length()))
		{
			std::cerr << Request << ": Asked for '" << cgi << "', But was not inside the root directory: '" << Configuration.Root << "'!" << std::endl;
			continue;	// The cgi path did NOT start with root, this is not what we want!
		}

		return new CgiResponse(ActualPath);
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