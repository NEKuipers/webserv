#include "ConfigDirectoryResponse.hpp"
#include <map>
#include <cassert>	// linux assert()

ConfigDirectoryResponse::ConfigDirectoryResponse(const std::string& Directory, const ConfigErrorReasons& ErrorReasons) : ConfigResponse(ErrorReasons), Directory(Directory)
{

}
ConfigDirectoryResponse::~ConfigDirectoryResponse()
{
	
}

std::ostream& operator<<(std::ostream& Stream, const ConfigDirectoryResponse& ConfigDirectoryResponse)
{
	ConfigDirectoryResponse.Print(Stream);
	return Stream;
}

void ConfigDirectoryResponse::Print(std::ostream& PrintStream) const
{
	PrintStream << "DirectoryResponse '" << Directory;
}

const std::string& ConfigDirectoryResponse::GetDirectory()
{
	return Directory;
}