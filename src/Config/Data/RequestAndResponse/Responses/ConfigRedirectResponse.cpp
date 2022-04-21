#include "ConfigRedirectResponse.hpp"
#include "Request.hpp"
#include "ToString.hpp"

ConfigRedirectResponse::ConfigRedirectResponse(int Code, const std::string& NewPath, const ConfigErrorReasons& ErrorReasons) : ConfigResponse(ErrorReasons), Code(Code), NewPath(NewPath)
{
	
}
ConfigRedirectResponse::~ConfigRedirectResponse()
{
	
}

int ConfigRedirectResponse::GetCode() const { return Code; }
const std::string& ConfigRedirectResponse::GetNewPath() const { return NewPath; }

std::ostream& operator<<(std::ostream& Stream, const ConfigRedirectResponse& ConfigRedirectResponse)
{
	ConfigRedirectResponse.Print(Stream);
	return Stream;
}

void ConfigRedirectResponse::Print(std::ostream& PrintStream) const
{
	PrintStream << "ConfigRedirectResponse " << Code << " " << NewPath;
}
