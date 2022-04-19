#include "ConfigDeleteResponse.hpp"
#include "Request.hpp"
#include "ToString.hpp"

ConfigDeleteResponse::ConfigDeleteResponse(const std::string& DeleteFile, const ConfigErrorReasons& ErrorReasons) : ConfigResponse(ErrorReasons), DeleteFile(DeleteFile)
{
	
}
ConfigDeleteResponse::~ConfigDeleteResponse()
{
	
}

const std::string& ConfigDeleteResponse::GetDeleteFile() const { return DeleteFile; }

std::ostream& operator<<(std::ostream& Stream, const ConfigDeleteResponse& ConfigDeleteResponse)
{
	ConfigDeleteResponse.Print(Stream);
	return Stream;
}

void ConfigDeleteResponse::Print(std::ostream& PrintStream) const
{
	PrintStream << "ConfigDeleteResponse " << DeleteFile;
}
