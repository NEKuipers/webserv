#include "ConfigUploadFileResponse.hpp"
#include <map>
#include <cassert>	// linux assert()

ConfigUploadFileResponse::ConfigUploadFileResponse(const std::string& FileName, const ConfigErrorReasons& ErrorReasons) : ConfigResponse(ErrorReasons), FileName(FileName)
{

}
ConfigUploadFileResponse::~ConfigUploadFileResponse()
{
	
}

std::ostream& operator<<(std::ostream& Stream, const ConfigUploadFileResponse& ConfigUploadFileResponse)
{
	ConfigUploadFileResponse.Print(Stream);
	return Stream;
}

void ConfigUploadFileResponse::Print(std::ostream& PrintStream) const
{
	PrintStream << "UploadFileResponse '" << FileName;
}

const std::string& ConfigUploadFileResponse::GetFileName()
{
	return FileName;
}

bool ConfigUploadFileResponse::RequiresBody() const
{
	return true;
}